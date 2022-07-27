// Header
#include "world_system.hpp"
#include "physics_system.hpp"
#include "ai_system.hpp"
#include "battle_system.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

// Game configuration
const size_t MAX_CHESTS = 5;
const size_t MAX_SUBBOSSES = 4;
const size_t MAX_BOSS = 1;
const size_t MAX_ITEMS = 3;
const int WorldSystem::GRANULARITY = 5; //size of the nodes the map is divided into, for pathfinding purposes.

// Map configuration
const int NUM_TILES_ACROSS = ceil(window_width_px / TILE_BB_WIDTH);
const int NUM_TILES_DOWN = ceil(window_height_px / TILE_BB_HEIGHT);

const size_t HORIZONTAL = 1;
const size_t VERTICAL = 0;
const size_t MAX_ITERATIONS = 1000;

// Animation configuration
const size_t ANIMATION_SPEED = 93.75f;
const size_t TIMER_ANIMATION_SPEED = 1000;

// Timer configurations
int MAZE_TIMER_MAX = 90;
int MAZE_TIMER_CURRENT = 0;

// Game Over configuration
const float gameOverTime = 3000.f;
float gameOverCount = 0.f;

// Create the world
WorldSystem::WorldSystem()
	: previous_direction(CHARACTER_DIRECTION::DOWN)
	, current_direction(CHARACTER_DIRECTION::DOWN)
	, map(NUM_TILES_ACROSS * NUM_TILES_DOWN)
	, maze(NUM_TILES_ACROSS * NUM_TILES_DOWN) {
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());

	// Initialzing world hub
	hub = {
		1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
		1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
		1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
		1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
		1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
	};
}

WorldSystem::~WorldSystem() {
	// Destroy music components
	if (background_music != nullptr)
		Mix_FreeMusic(background_music);
	if (character_teleport_sound != nullptr)
		Mix_FreeChunk(character_teleport_sound);
	Mix_CloseAudio();

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	glfwDestroyWindow(window);
}

// Debugging
namespace {
	void glfw_err_cb(int error, const char* desc) {
		fprintf(stderr, "%d: %s", error, desc);
	}
}

// World initialization
// Note, this has a lot of OpenGL specific things, could be moved to the renderer
GLFWwindow* WorldSystem::create_window() {
	///////////////////////////////////////
	// Initialize GLFW
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW");
		return nullptr;
	}

	//-------------------------------------------------------------------------
	// If you are on Linux or Windows, you can change these 2 numbers to 4 and 3 and
	// enable the glDebugMessageCallback to have OpenGL catch your mistakes for you.
	// GLFW / OGL Initialization
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// Create the main window (for rendering, keyboard, and mouse input)
	window = glfwCreateWindow(window_width_px, window_height_px, "Island of Lost Souls", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to glfwCreateWindow");
		return nullptr;
	}

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_key(_0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_move({ _0, _1 }); };
	auto mouse_button_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_button(_0, _1, _2); };
	glfwSetKeyCallback(window, key_redirect);
	glfwSetCursorPosCallback(window, cursor_pos_redirect);
	glfwSetMouseButtonCallback(window, mouse_button_redirect);

	//////////////////////////////////////
	// Loading music and sounds with SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Failed to initialize SDL Audio");
		return nullptr;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
		fprintf(stderr, "Failed to open audio device");
		return nullptr;
	}

	background_music = Mix_LoadMUS(audio_path("forest.wav").c_str());
	character_teleport_sound = Mix_LoadWAV(audio_path("character_teleport.wav").c_str());
	chest_open_sound = Mix_LoadWAV(audio_path("chest_open.wav").c_str());
	item_acquire_health_sound = Mix_LoadWAV(audio_path("item_acquire_health.wav").c_str());
	item_acquire_damage_sound = Mix_LoadWAV(audio_path("item_acquire_damage.wav").c_str());

	if (background_music == nullptr || character_teleport_sound == nullptr ||
		chest_open_sound == nullptr || item_acquire_health_sound == nullptr ||
		item_acquire_damage_sound == nullptr) {
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("music.wav").c_str(),
			audio_path("character_teleport.wav").c_str(),
			audio_path("chest_open.wav").c_str(),
			audio_path("item_acquire_health.wav").c_str(),
			audio_path("item_acquire_damage.wav").c_str());
		return nullptr;
	}

	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;
	// Playing background music indefinitely
	Mix_PlayMusic(background_music, -1);
	fprintf(stderr, "Loaded music\n");

	// Set all states to default
	restart_game();
}

// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update) {
	// Updating window title
	std::stringstream title_ss;
	title_ss << "Island of Lost Souls";
	glfwSetWindowTitle(window, title_ss.str().c_str());

	// Remove debug info from the last step
	while (registry.debugComponents.entities.size() > 0)
		registry.remove_all_components_of(registry.debugComponents.entities.back());

	// The screen and its state
	assert(registry.screenStates.components.size() <= 1);
	ScreenState& state = registry.screenStates.components[0];

	if (state.used_state == STATE_ID::IDLE) {
		// Changing textures
		auto& render_request_registry = registry.renderRequests;

		// Change the texture of the character according to its direction
		if (render_request_registry.has(player_character) &&
			previous_direction != current_direction) {
			TEXTURE_ASSET_ID used_texture;
			switch (current_direction) {
			default:
			case CHARACTER_DIRECTION::DOWN:
				used_texture = TEXTURE_ASSET_ID::CHARACTER_DOWN;
				break;
			case CHARACTER_DIRECTION::UP:
				used_texture = TEXTURE_ASSET_ID::CHARACTER_UP;
				break;
			case CHARACTER_DIRECTION::LEFT:
				used_texture = TEXTURE_ASSET_ID::CHARACTER_LEFT;
				break;
			case CHARACTER_DIRECTION::RIGHT:
				used_texture = TEXTURE_ASSET_ID::CHARACTER_RIGHT;
				break;
			}

			render_request_registry.get(player_character).used_texture = used_texture;
		}

		// Processing the animation state
		auto& timer_registry = registry.timer;
		for (Entity entity : registry.animations.entities) {
			if (!registry.timer.has(entity)) {
				Animation& animation = registry.animations.get(entity);
				animation.elapsed_ms += elapsed_ms_since_last_update;

				if (animation.elapsed_ms > ANIMATION_SPEED) {
					animation.current_frame = (animation.current_frame + 1) % animation.num_frames;
					animation.elapsed_ms = 0.f;
				}
			}
		}

		if (state.used_screen == SCREEN_ID::MAZE && timer_registry.components.size()>1) {
			Animation& animation1 = registry.animations.get(timer1);
			Animation& animation2 = registry.animations.get(timer2);

			for (Entity entity : registry.timer.entities) {
				if (!(animation1.current_frame == 0 && animation2.current_frame == 0)) {
					Animation& animation = registry.animations.get(entity);
					Timer& timer = registry.timer.get(entity);
					animation.elapsed_ms += elapsed_ms_since_last_update;
					if (animation.elapsed_ms > timer.updateTime) {
						if (animation.current_frame > 0) {
							animation.current_frame = (animation.current_frame - 1);
							animation.elapsed_ms = 0.f;
						}
						else if (animation.current_frame == 0) {
							animation.current_frame = 9;
							animation.elapsed_ms = 0.f;
						}
					}
				}
				else {
					state.used_screen = SCREEN_ID::GAMEOVER;
					generate_game_over();
				}
			}
		}

		if (state.used_screen == SCREEN_ID::GAMEOVER) {
			gameOverCount += elapsed_ms_since_last_update;
			if (gameOverCount >= gameOverTime) {
				gameOverCount = 0.f;
				restart_game();
			}
		}
	}
	else if (state.used_state == STATE_ID::LOOT) {
		// Processing the card state
		for (Entity entity : registry.cardAppearTimers.entities) {
			// Progress timer
			CardAppearTimer& counter = registry.cardAppearTimers.get(entity);
			counter.counter_ms -= elapsed_ms_since_last_update;

			if (counter.counter_ms < 0) {
				state.used_state = STATE_ID::IDLE;
				registry.cardAppearTimers.remove(entity);
				// Prevent card from being drawn once fade out timer expired
				registry.renderRequests.remove(entity);
				// As card has motion, move it outside of screen
				registry.motions.get(entity).position = { 0.f - CARD_BB_WIDTH, 0.f - CARD_BB_HEIGHT };
			}
		}
	}
	else if (state.used_state == STATE_ID::BATTLE) {
		Battle& battle = registry.battles.get(player_character);
		if (battle.is_my_turn && battle.current_mode == MODE_ID::DONE) {
			// If player's turn is over, switch over to the opponent
			battle.is_my_turn = false;
			battle.current_mode = MODE_ID::SELECT;
			registry.battles.get(current_boss).is_my_turn = true;

			// If opponent's health has dropped to 0...
			Health& health = registry.healthComponents.get(player_character);
			Health& health_other = registry.healthComponents.get(current_boss);
			if (health_other.current_health <= 0) {
				// Update max/starting health for next battle
				health.max_health = health.current_health;

				// Cards that were in the player's hand are returned to their deck
				return_cards_player_deck();
				registry.remove_all_components_of(current_boss);
				
				// Mark biome as completed
				Player& player = registry.players.get(player_character);
				switch (player.current_biome) {
				default:
				case BIOME_ID::FROST:
					registry.progressions.get(player_character).is_frost_biome_defeated = true;
					break;
				case BIOME_ID::BEACH:
					registry.progressions.get(player_character).is_beach_biome_defeated = true;
					break;
				case BIOME_ID::LAVA:
					registry.progressions.get(player_character).is_lava_biome_defeated = true;
					break;
				case BIOME_ID::JUNGLE:
					registry.progressions.get(player_character).is_jungle_biome_defeated = true;
					break;
				}

				// Return to the world hub
				battle.is_my_turn = true;
				player.current_biome = BIOME_ID::HUB;
				state.used_state = STATE_ID::IDLE;
				state.used_screen = SCREEN_ID::HUB;

				// Save game
				registry.screenTimers.emplace(player_character);
				save_game();
			}
		}
		else {
			// Remove cards in play that have dropped to a health of 0
			for (Entity card : registry.playerPlays.entities) {
				Health& health = registry.healthComponents.get(card);
				if (health.current_health == 0)
					registry.remove_all_components_of(card);
			}

			for (Entity card : registry.bossPlays.entities) {
				Health& health = registry.healthComponents.get(card);
				if (health.current_health == 0)
					registry.remove_all_components_of(card);
      }
    }
  }

	// Processing the screen
	float min_screen_counter_ms = 1000.f;
	for (Entity entity : registry.screenTimers.entities) {
		// Progress timer
		ScreenTimer& counter = registry.screenTimers.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;
		if (counter.counter_ms < min_screen_counter_ms)
			min_screen_counter_ms = counter.counter_ms;

		// Regenerate world once the screen timer expired
		if (counter.counter_ms < 0) {
			registry.screenTimers.remove(entity);
			state.darken_screen_factor = 0;

			switch (state.used_screen) {
			default:
			case SCREEN_ID::MENU:
				restart_game();
				break;
			case SCREEN_ID::INSTRUCTIONS:
				generate_instructions();
				break;
			case SCREEN_ID::HUB:
				generate_hub();
				break;
			case SCREEN_ID::BIOME:
				generate_random_map();
				break;
			case SCREEN_ID::MAZE:
				generate_random_maze();
				break;
			case SCREEN_ID::BATTLE:
				generate_board();
				break;

			}
		}
	}

	// Reduce window brightness if any of the present chickens is dying
	state.darken_screen_factor = 1 - min_screen_counter_ms / 1000;

	return true;
}

// Reset the world state to its initial state
void WorldSystem::restart_game() {
	// Debugging for memory/component leaks
	registry.list_all_components();
	printf("Restarting\n");

	// Reset the game speed
	current_speed = 155.f;

	// Remove all entities that we created
	while (registry.motions.entities.size() > 0)
		registry.remove_all_components_of(registry.motions.entities.back());

	// Debugging for memory/component leaks
	registry.list_all_components();

	// Create character
	player_character = createCharacter(renderer, { window_width_px / 2, window_height_px / 2 });

	// Create main menu
	generate_menu();

	// Reset screen parameters
	assert(registry.screenStates.components.size() <= 1);
	ScreenState& state = registry.screenStates.components[0];
	state.used_state = STATE_ID::STATE_COUNT;
	state.used_screen = SCREEN_ID::MENU;
}

// Helper function for stringifying `SCREEN_ID`
constexpr const char* stringify_screen(SCREEN_ID used_screen)
{
	switch (used_screen)
	{
	case SCREEN_ID::MENU:
	case SCREEN_ID::INSTRUCTIONS:
	case SCREEN_ID::HUB:
		return "2";
	case SCREEN_ID::BIOME:
	case SCREEN_ID::MAZE:
	case SCREEN_ID::BATTLE:
		return "3";
	default:
		throw std::invalid_argument("Unimplemented");
	}
}

// Helper function for converting to `SCREEN_ID`
SCREEN_ID screen_from_string(std::string s)
{
	switch (std::stoi(s))
	{
	case 2:
		return SCREEN_ID::HUB;
	case 3:
		return SCREEN_ID::BIOME;
	default:
		throw std::invalid_argument("Unimplemented");
	}
}

// Helper function for stringifying `BIOME_ID`
constexpr const char* stringify_biome(BIOME_ID used_biome)
{
	switch (used_biome)
	{
	default:
	case BIOME_ID::HUB:
		return "0";
	case BIOME_ID::FROST:
		return "1";
	case BIOME_ID::BEACH:
		return "2";
	case BIOME_ID::LAVA:
		return "3";
	case BIOME_ID::JUNGLE:
		return "4";
	}
}

// Helper function for converting to `BIOME_ID`
BIOME_ID biome_from_string(std::string s)
{
	switch (std::stoi(s))
	{
	case 0:
		return BIOME_ID::HUB;
	case 1:
		return BIOME_ID::FROST;
	case 2:
		return BIOME_ID::BEACH;
	case 3:
		return BIOME_ID::LAVA;
	case 4:
		return BIOME_ID::JUNGLE;
	default:
		throw std::invalid_argument("Unimplemented");
	}
}

// Helper function for saving/loading game state
std::ostream& operator<<(std::ostream& os, const WorldSystem::SaveData& sd)
{
	std::string cards = "";
	for (Entity card : registry.playerDecks.entities) {
		Health& health = registry.healthComponents.get(card);
		Damage& damage = registry.damageComponents.get(card);

		cards = cards
			+ std::to_string(health.current_health)
			+ " " + std::to_string(health.max_health)
			+ " " + std::to_string(damage.damage_output)
			+ ",";
	}

	os <<
		stringify_screen(sd.screen)
		<< "\n" <<
		stringify_biome(sd.biome)
		<< "\n" <<
		std::to_string(sd.is_jungle_biome_defeated)
		<< "\n" <<
		std::to_string(sd.is_lava_biome_defeated)
		<< "\n" <<
		std::to_string(sd.is_beach_biome_defeated)
		<< "\n" <<
		std::to_string(sd.is_frost_biome_defeated)
		<< "\n" <<
		cards;

	return os;
}

// Saves the game's state
void WorldSystem::save_game()
{
	std::ofstream file(save_path("save.txt"));
	
	assert(registry.screenStates.components.size() <= 1);
	ScreenState& state = registry.screenStates.components[0];
	Player& player = registry.players.get(player_character);
	Progression& player_progression = registry.progressions.get(player_character);

	SaveData save(
		state.used_screen,
		player.current_biome,
		player_progression.is_jungle_biome_defeated,
		player_progression.is_lava_biome_defeated,
		player_progression.is_beach_biome_defeated,
		player_progression.is_frost_biome_defeated,
		registry.playerDecks.entities);
	file << save;

	file.close();
}

// Loads the game's saved state
void WorldSystem::load_game()
{
	printf("Loading Save Data\n");
	std::ifstream file(save_path("save.txt"));
	
	assert(registry.screenStates.components.size() <= 1);
	ScreenState& state = registry.screenStates.components[0];
	Player& player = registry.players.get(player_character);
	Progression& player_progression = registry.progressions.get(player_character);

	std::string s;
	getline(file, s);
	state.used_state = STATE_ID::IDLE;
	state.used_screen = screen_from_string(s);
	printf("Saved current_screen: %s\n", stringify_screen(state.used_screen));
	getline(file, s);
	player.current_biome = biome_from_string(s);
	printf("Saved current_biome: %s\n", stringify_biome(player.current_biome));
	getline(file, s);
	player_progression.is_jungle_biome_defeated = std::stoi(s);
	getline(file, s);
	player_progression.is_lava_biome_defeated = std::stoi(s);
	getline(file, s);
	player_progression.is_beach_biome_defeated = std::stoi(s);
	getline(file, s);
	player_progression.is_frost_biome_defeated = std::stoi(s);
	printf("Saved biome progression: (\n jungle = %d, \n lava = %d, \n beach = %d, \n frost = %d\n)\n",
		player_progression.is_jungle_biome_defeated,
		player_progression.is_lava_biome_defeated,
		player_progression.is_beach_biome_defeated,
		player_progression.is_frost_biome_defeated);

	while (registry.playerDecks.entities.size() > 0)
		registry.remove_all_components_of(registry.playerDecks.entities.back());
	// Add cards
	if (std::getline(file, s)) {
		size_t pos = 0;
		std::string card_properties_string;
		std::string delimiter = ",";
		while ((pos = s.find(delimiter)) != std::string::npos) {
			card_properties_string = s.substr(0, pos);

			std::string arr[3];
			int i = 0;
			std::stringstream ssin(card_properties_string);
			while (ssin.good() && i < 3) {
				ssin >> arr[i];
				++i;
			}

			loadCard(renderer, std::stoi(arr[0]), std::stoi(arr[1]), std::stoi(arr[2]));
			s.erase(0, pos + delimiter.length());
		}

		printf("Saved cards: (\n");
		for (Entity card : registry.playerDecks.entities) {
			Health& health = registry.healthComponents.get(card);
			Damage& damage = registry.damageComponents.get(card);

			printf(" Card %i with %i health and %i damage\n",
				(int)card, (int)health.current_health, (int)damage.damage_output);
		}
		printf(")\n");
	}
	else
		printf("No saved cards\n");

	file.close();
}

// Generate main menu
void WorldSystem::generate_menu() {
	createMenu(renderer, { window_width_px / 2.f, window_height_px / 2.f });

	std::ifstream file(save_path("save.txt"));
	if (file.peek() == std::ifstream::traits_type::eof()) {
		createMenuStart(renderer, { window_width_px / 2.f, 250 });
		createMenuInstructions(renderer, { window_width_px / 2.f, 350 });
	}
	else {
		createMenuStart(renderer, { window_width_px / 2.f, 250 });
		createMenuContinue(renderer, { window_width_px / 2.f, 350 });
		createMenuInstructions(renderer, { window_width_px / 2.f, 450 });
	}
}

// Generate instructions
void WorldSystem::generate_instructions() {
	createInstructions(renderer, { window_width_px / 2.f, window_height_px / 2.f  });
	createInstructionsClose(renderer, { window_width_px - 50, 50 });
}
void WorldSystem::generate_story() {
	createStory(renderer, { window_width_px / 2.f, window_height_px / 2.f  });
	createClose(renderer, { window_width_px - 50, 50 });
}

void WorldSystem::generate_game_over() {
	createGameOver(renderer, { window_width_px / 2.f, window_height_px / 2.f });
}

// Clear the world and generate hub to access other biomes
void WorldSystem::generate_hub() {
	// Remove all entities that we created (applies when coming from another biome)
	auto& player_registry = registry.players;
	auto& motion_registry = registry.motions;
	for (int i = (int)motion_registry.components.size() - 1; i >= 0; --i) {
		if (!player_registry.has(motion_registry.entities[i]) &&
			!registry.playerDecks.has(motion_registry.entities[i]))
			registry.remove_all_components_of(motion_registry.entities[i]);
		else
			// Only prevent the character and cards from being drawn
			registry.renderRequests.remove(motion_registry.entities[i]);
	}

	// Create tiles
	Player& player = player_registry.get(player_character);
	for (int col = 0; col < NUM_TILES_ACROSS; col++) {
		for (int row = 0; row < NUM_TILES_DOWN; row++) {
			vec2 position = {
				col * TILE_BB_WIDTH + (TILE_BB_WIDTH / 2),
				row * TILE_BB_HEIGHT + (TILE_BB_HEIGHT / 2) };

			if (hub[row * NUM_TILES_ACROSS + col] == (int)TILES::WALL)
				createWall(renderer, position, player.current_biome);
			else
				createPath(renderer, position, player.current_biome);
		}
	}

	// Create non-interactable objects
	std::uniform_int_distribution<int>
		int_dist((int)TEXTURE_ASSET_ID::PALM_TREE_1, (int)TEXTURE_ASSET_ID::PALM_TREE_WITH_COCONUT_2);

	for (int col = 0; col < NUM_TILES_ACROSS; col++) {
		for (int row = 0; row < NUM_TILES_DOWN; row++) {
			// Disregard scale of tree as position should match scale of tile
			vec2 position = {
				col * TILE_BB_WIDTH + (TILE_BB_WIDTH / 2),
				row * TILE_BB_HEIGHT + (TILE_BB_HEIGHT / 2) };
			int index = row * NUM_TILES_ACROSS + col;

			// Place tree on the edges of path tiles
			bool can_place = false;
			if (col == 0 && row == 0)
				can_place = hub[index] == (int)TILES::PATH && hub[index + 1] == (int)TILES::WALL;
			else if (col == NUM_TILES_ACROSS - 1 && row == NUM_TILES_DOWN - 1)
				can_place = hub[index] == (int)TILES::PATH && hub[index - 1] == (int)TILES::WALL;
			else if (col == NUM_TILES_ACROSS / 2 || row == NUM_TILES_DOWN / 2)
				can_place = false;
			else
				can_place = hub[index] == (int)TILES::PATH && (hub[index - 1] == (int)TILES::WALL || hub[index + 1] == (int)TILES::WALL);

			if (can_place)
				createTree(renderer, position, (TEXTURE_ASSET_ID)int_dist(rng));
		}
	}

	// Create biome entrances
	Progression& player_progression = registry.progressions.get(player_character);
	if (!player_progression.is_frost_biome_defeated)
		createBiome(renderer, { TILE_BB_WIDTH / 2, window_height_px / 2 }, BIOME_ID::FROST);
	if (!player_progression.is_beach_biome_defeated)
		createBiome(renderer, { window_width_px / 2, TILE_BB_HEIGHT / 2 }, BIOME_ID::BEACH);
	if (!player_progression.is_lava_biome_defeated)
		createBiome(renderer, { window_width_px - (TILE_BB_WIDTH / 2), window_height_px / 2 }, BIOME_ID::LAVA);
	if (!player_progression.is_jungle_biome_defeated)
		createBiome(renderer, { window_width_px / 2, window_height_px - (TILE_BB_HEIGHT / 2) }, BIOME_ID::JUNGLE);

	// Create NPC
	createNpc(renderer, { window_width_px / 2, window_height_px / 3 });

	// Re-draw character last
	registry.motions.get(player_character).position = { window_width_px / 2, window_height_px / 2 };
	registry.renderRequests.insert(
		player_character,
		{ TEXTURE_ASSET_ID::CHARACTER_DOWN,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });
}

// Clear the world and procedurally generate map
void WorldSystem::generate_random_map() {
	// Remove all entities that we created
	auto& player_registry = registry.players;
	auto& motion_registry = registry.motions;
	for (int i = (int)motion_registry.components.size() - 1; i >= 0; --i) {
		if (!registry.players.has(motion_registry.entities[i]) &&
			!registry.playerDecks.has(motion_registry.entities[i]))
			registry.remove_all_components_of(motion_registry.entities[i]);
		else
			// Only prevent the character and cards from being drawn
			registry.renderRequests.remove(motion_registry.entities[i]);
	}

	// Create map (WorldSystem::walk_path adds paths)
	std::fill(map.begin(), map.end(), (int)TILES::WALL);
	walk_path();

	// Create tiles
	Player& player = player_registry.get(player_character);
	for (int col = 0; col < NUM_TILES_ACROSS; col++) {
		for (int row = 0; row < NUM_TILES_DOWN; row++) {
			vec2 position = {
				col * TILE_BB_WIDTH + (TILE_BB_WIDTH / 2),
				row * TILE_BB_HEIGHT + (TILE_BB_HEIGHT / 2) };

			if (map[row * NUM_TILES_ACROSS + col] == (int)TILES::WALL)
				createWall(renderer, position, player.current_biome);
			else
				createPath(renderer, position, player.current_biome);
		}
	}

	// Create chests
	for (uint i = 0; i < MAX_CHESTS; i++) {
		std::uniform_int_distribution<int> int_dist((int)TIER_ID::BRONZE, (int)TIER_ID::TIER_COUNT - 1);
		createChest(renderer, choose_position(map), (TIER_ID)int_dist(rng));
	}

	// Create biome entrance
	createBiome(renderer, choose_position(map), BIOME_ID::HUB);

	// Create enemies
	for (uint i = 0; i < MAX_SUBBOSSES; i++)
		createSubBoss(renderer, choose_position(map), player.current_biome);
	current_boss = createBoss(renderer, choose_position(map), player.current_biome);

	// Re-draw character
	registry.motions.get(player_character).position = choose_position(map);
	registry.renderRequests.insert(
		player_character,
		{ TEXTURE_ASSET_ID::CHARACTER_DOWN,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });
}

// Spawn random walker to procedurally generate map
// https://medium.com/pumpkinbox-blog/procedural-map-generation-with-godot-part-1-1b4e78191e90
void WorldSystem::walk_path() {
	std::array<std::array<int, 2>, 4> directions = { { {-1, 0}, {0, 1}, {1, 0}, {0, -1} } };
	std::uniform_int_distribution<int> int_dist(0, directions.size() - 1);
	vec2 walker = { 0, 0 };

	uint iteration = 0;
	while (iteration < MAX_ITERATIONS) {
		// Choose a random direction to walk
		std::array<int, 2> direction = directions[int_dist(rng)];
		vec2 random_direction = vec2(direction[0], direction[1]);

		// Verify direction is within bounds of window
		if (walker.x + random_direction.x >= 0 &&
			walker.x + random_direction.x < NUM_TILES_ACROSS &&
			walker.y + random_direction.y >= 0 &&
			walker.y + random_direction.y < NUM_TILES_DOWN) {
			// Move in that direction
			walker += random_direction;
			map[walker.y * NUM_TILES_ACROSS + walker.x] = (int)TILES::PATH;
			iteration += 1;
		}
	}
}

// Clear the world and procedurally generate maze
void WorldSystem::generate_random_maze() {
	// Remove all entities that we created
	auto& player_registry = registry.players;
	auto& motion_registry = registry.motions;
	for (int i = (int)registry.motions.components.size() - 1; i >= 0; --i) {
		if (!registry.players.has(motion_registry.entities[i]) &&
			!registry.playerDecks.has(motion_registry.entities[i]))
			registry.remove_all_components_of(motion_registry.entities[i]);
		else
			// Only prevent the character and cards from being drawn
			registry.renderRequests.remove(motion_registry.entities[i]);
	}

	MAZE_TIMER_CURRENT = 0;

	// Create maze (WorldSystem::divide_maze adds walls)
	std::fill(maze.begin(), maze.end(), (int)TILES::PATH);
	divide_maze(0, 0, NUM_TILES_ACROSS, NUM_TILES_DOWN, choose_orientation(NUM_TILES_ACROSS, NUM_TILES_DOWN));

	// Create tiles
	Player& player = player_registry.get(player_character);
	for (int col = 0; col < NUM_TILES_ACROSS; col++) {
		for (int row = 0; row < NUM_TILES_DOWN; row++) {
			vec2 position = {
				col * TILE_BB_WIDTH + (TILE_BB_WIDTH / 2),
				row * TILE_BB_HEIGHT + (TILE_BB_HEIGHT / 2) };

			if (maze[row * NUM_TILES_ACROSS + col] == (int)TILES::WALL)
				createWall(renderer, position, player.current_biome);
			else
				createPath(renderer, position, player.current_biome);
		}
	}

	// Create interactables
	for (uint i = 0; i < MAX_ITEMS; i++) {
		createHealth(renderer, choose_position(maze));
		createDamage(renderer, choose_position(maze));
	}

	// Create biome entrance, starting from the last row and column
	int index = (NUM_TILES_DOWN - 1) * NUM_TILES_ACROSS + (NUM_TILES_ACROSS - 1);
	while (maze[index] == (int)TILES::WALL)
		index--;

	// Disregard scale of biome as position should match scale of tile
	vec2 position = {
		(index % NUM_TILES_ACROSS) * TILE_BB_WIDTH + (TILE_BB_WIDTH / 2),
		(index / NUM_TILES_ACROSS) * TILE_BB_HEIGHT + (TILE_BB_HEIGHT / 2) };

	createBiome(renderer, position, player.current_biome);

	// Re-draw character, starting from the first row and column
	index = 0;
	while (maze[index] == (int)TILES::WALL)
		index++;

	// Disregard scale of character as position should match scale of tile
	position = {
		(index % NUM_TILES_ACROSS) * TILE_BB_WIDTH + (TILE_BB_WIDTH / 2),
		(index / NUM_TILES_ACROSS) * TILE_BB_HEIGHT + (TILE_BB_HEIGHT / 2) };

	registry.motions.get(player_character).position = position;
	registry.renderRequests.insert(
		player_character,
		{ TEXTURE_ASSET_ID::CHARACTER_DOWN,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	// Create timer
	timer1 = createTimer(renderer, { 860, 50 }, 9, 10000.f);
	timer2 = createTimer(renderer, { 910, 50 }, 0, 1000.f);
	registry.animations.get(timer1).elapsed_ms = 9000.f;
}

// Recursively carve out maze
// http://weblog.jamisbuck.org/2011/1/12/maze-generation-recursive-division-algorithm
void WorldSystem::divide_maze(int x, int y, int width, int height, int orientation) {
	// Check if maze has reached target resolution
	if (width < 2 || height < 2)
		return;

	bool is_horizontal = orientation == HORIZONTAL;

	// Take into account that walls are its own tiles
	int true_width = (width - 1) / 2;
	int true_height = (height - 1) / 2;
	std::uniform_int_distribution<int> width_dist(0, true_width - 1);
	std::uniform_int_distribution<int> height_dist(0, true_height - 1);

	// Bisect maze with a wall (determine where to draw wall from)
	int wall_x = x + (is_horizontal ? 0 : 2 * (width_dist(rng)) + 1);
	int wall_y = y + (is_horizontal ? 2 * (height_dist(rng)) + 1 : 0);

	// Add a single path through the wall
	int path_x = wall_x + (is_horizontal ? 2 * width_dist(rng) : 0);
	int path_y = wall_y + (is_horizontal ? 0 : 2 * height_dist(rng));

	// Determine direction/axis wall will be drawn
	int direction_x = is_horizontal ? 1 : 0;
	int direction_y = is_horizontal ? 0 : 1;

	// Determine length of wall (or number of tiles to draw)
	int wall_length = is_horizontal ? width : height;

	// Add walls, except for where a path should be
	for (int i = 0; i < wall_length; i++) {
		if (wall_x != path_x || wall_y != path_y)
			maze[wall_y * NUM_TILES_ACROSS + wall_x] = (int)TILES::WALL;

		wall_x += direction_x;
		wall_y += direction_y;
	}

	// Repeat above steps on left side of wall
	int new_x = x;
	int new_y = y;
	int new_width = is_horizontal ? width : wall_x - x;
	int new_height = is_horizontal ? wall_y - y : height;
	divide_maze(new_x, new_y, new_width, new_height, choose_orientation(new_width, new_height));

	// Repeat above steps on right side of wall
	new_x = is_horizontal ? x : wall_x + 1;
	new_y = is_horizontal ? wall_y + 1 : y;
	new_width = is_horizontal ? width : x + width - wall_x - 1;
	new_height = is_horizontal ? y + height - wall_y - 1 : height;
	divide_maze(new_x, new_y, new_width, new_height, choose_orientation(new_width, new_height));
}

// Helper function to determine which direction to bisect the maze
// http://weblog.jamisbuck.org/2011/1/12/maze-generation-recursive-division-algorithm
int WorldSystem::choose_orientation(int width, int height) {
	if (width < height)
		return HORIZONTAL;
	else if (height < width)
		return VERTICAL;
	else {
		std::uniform_int_distribution<int> int_dist(0, 1);
		return int_dist(rng) == 0 ? HORIZONTAL : VERTICAL;
	}
}

// Randomly select a position on the map that is traversable
vec2 WorldSystem::choose_position(std::vector<int>& tiles) {
	// Randomly picking row and column
	std::uniform_int_distribution<int> cols_dist(0, NUM_TILES_ACROSS - 1);
	std::uniform_int_distribution<int> rows_dist(0, NUM_TILES_DOWN - 1);

	// Ensure chosen position is traversable
	int col = 0;
	int row = 0;
	vec2 position = { 0, 0 };

	do {
		col = cols_dist(rng);
		row = rows_dist(rng);
		// Disregard scale of entity as position should match scale of tile
		position = {
			col * TILE_BB_WIDTH + (TILE_BB_WIDTH / 2),
			row * TILE_BB_HEIGHT + (TILE_BB_HEIGHT / 2) };
	} while (tiles[row * NUM_TILES_ACROSS + col] == (int)TILES::WALL);

	// Mark chosen position as a wall so that subsequent entities aren't spawned on top
	tiles[row * NUM_TILES_ACROSS + col] = (int)TILES::WALL;

	return position;
}

// Clear the world and generate board for card battle
void WorldSystem::generate_board() {
	// Remove all entities that we created
	auto& motion_registry = registry.motions;
	for (int i = (int)motion_registry.components.size() - 1; i >= 0; --i) {
		if (!registry.players.has(motion_registry.entities[i]) &&
			!registry.bosses.has(motion_registry.entities[i]) &&
			!registry.playerDecks.has(motion_registry.entities[i]))
			registry.remove_all_components_of(motion_registry.entities[i]);
		else
			// Only prevent the character, boss, and cards from being drawn
			registry.renderRequests.remove(motion_registry.entities[i]);
	}

	// Create health bars
	Entity bar = createBarUI({ window_width_px / 2, BOARD_OPPONENT_HEALTH_HEIGHT }, 
		{ window_width_px, BOARD_HEALTH_BB_HEIGHT });
	registry.boardOpponentBars.emplace(bar);
	bar = createBarUI({ window_width_px / 2, BOARD_PLAYER_HEALTH_HEIGHT }, 
		{ window_width_px, BOARD_HEALTH_BB_HEIGHT });
	registry.boardPlayerBars.emplace(bar);

	// Create placement for cards
	for (int i = 0; i < MAX_PLAY; i++) {
		createCardPlacement(renderer, 
			{ (i + 0.5) * window_width_px / MAX_PLAY, CARD_OPPONENT_PLAY_HEIGHT }, false);
		createCardPlacement(renderer, 
			{ (i + 0.5) * window_width_px / MAX_PLAY, CARD_PLAYER_PLAY_HEIGHT }, true);
	}

	// Render cards currently in deck
	int cards_in_hand = 0;
	auto& deck_registry = registry.playerDecks;
	while (deck_registry.components.size() > 0) {
		Motion& motion = motion_registry.get(get_card_player_deck());
		motion.position = { (cards_in_hand + 0.5) * window_width_px / MAX_PLAY, CARD_PLAYER_HAND_HEIGHT };

		// Allow a maximum of three cards in one's hand
		cards_in_hand++;
		if (cards_in_hand == MAX_HAND)
			break;
	}

	// Create card generator
	createCardGenerator(renderer, { (MAX_HAND + 0.5) * window_width_px / MAX_PLAY, CARD_PLAYER_HAND_HEIGHT });

	// Create mode indicator
	createModeUI(renderer, { window_width_px / 2, BOARD_MODE_HEIGHT });
}

// Compute collisions between entities
void WorldSystem::handle_collisions(float elapsed_ms) {
	// Elapsed time in seconds
	float step_seconds = elapsed_ms / 1000.f;

	// Loop over all collisions detected by the physics system
	auto& collisionsRegistry = registry.collisions; // TODO: @Tim, is the reference here needed?
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;

		// The screen and its state
		assert(registry.screenStates.components.size() <= 1);
		ScreenState& state = registry.screenStates.components[0];

		// Collisions that involve the character
		if (registry.players.has(entity)) {
			Player& player = registry.players.get(entity);

			// Checking Player - Biome collisions
			if (registry.biomes.has(entity_other)) {
				if (!registry.screenTimers.has(entity)) {
					// Teleport, reset timer, and make next screen a biome or hub
					registry.screenTimers.emplace(entity);
					Mix_PlayChannel(-1, character_teleport_sound, 0);

					player.current_biome = registry.biomes.get(entity_other).used_biome;
					if (player.current_biome == BIOME_ID::HUB)
						state.used_screen = SCREEN_ID::HUB;
					else
						state.used_screen = SCREEN_ID::BIOME;
				}
			}
			// Checking Player - Enemy collisions
			else if (registry.subBosses.has(entity_other) || registry.bosses.has(entity_other)) {
				// Get repulsed back
				Motion& enemy_motion = registry.motions.get(entity_other);
				enemy_motion.position.x += -enemy_motion.velocity.x * step_seconds;
				enemy_motion.position.y += -enemy_motion.velocity.y * step_seconds;
				enemy_motion.velocity = { 0.f, 0.f };

				if (!registry.screenTimers.has(entity)) {
					// Teleport, reset timer, and transition to next screen
					registry.screenTimers.emplace(entity);
					Mix_PlayChannel(-1, character_teleport_sound, 0);

					if (registry.subBosses.has(entity_other))
						state.used_screen = SCREEN_ID::MAZE;
					else {
						registry.traversables.emplace(entity_other); // forcing no collision
						state.used_state = STATE_ID::BATTLE;
						state.used_screen = SCREEN_ID::BATTLE;
						registry.battles.get(player_character).is_my_turn = true; // player starts first
					}
				}
			}
			// Checking Player - Lootable collisions
			else if (registry.lootables.has(entity_other)) {
				Lootable& lootable = registry.lootables.get(entity_other);
				if (!lootable.is_looted) {
					// Chest has been looted
					lootable.is_looted = true;
					Mix_PlayChannel(-1, chest_open_sound, 0);

					TEXTURE_ASSET_ID used_texture;
					switch (registry.tiers.get(entity_other).used_tier)
					{
					default:
					case TIER_ID::BRONZE:
						used_texture = TEXTURE_ASSET_ID::CHEST_BRONZE_OPENED;
						break;
					case TIER_ID::SILVER:
						used_texture = TEXTURE_ASSET_ID::CHEST_SILVER_OPENED;
						break;
					case TIER_ID::GOLD:
						used_texture = TEXTURE_ASSET_ID::CHEST_GOLD_OPENED;
						break;
					}

					registry.renderRequests.get(entity_other).used_texture = used_texture;

					// Generate card
					std::uniform_int_distribution<int> int_dist;
					switch (registry.tiers.get(entity_other).used_tier) {
					default:
					case TIER_ID::BRONZE:
						int_dist.param(std::uniform_int_distribution<int>::param_type(1, 2));
						break;
					case TIER_ID::SILVER:
						int_dist.param(std::uniform_int_distribution<int>::param_type(2, 3));
						break;
					case TIER_ID::GOLD:
						int_dist.param(std::uniform_int_distribution<int>::param_type(4, MAX_HEALTH_DAMAGE));
						break;
					}

					Entity card = createCard(renderer, { window_width_px / 2, window_height_px / 2 },
						player.current_biome, int_dist(rng), MAX_HEALTH_DAMAGE, int_dist(rng));

					registry.playerDecks.emplace(card);
					registry.cardAppearTimers.emplace(card);
					state.used_state = STATE_ID::LOOT;
				}
			}
			// Checking Player - NPC collisions
			else if (registry.npcs.has(entity_other)) {
				generate_story();
				state.used_screen= SCREEN_ID::STORY;
			}
			// Checking Player - Interactable collisions
			else if (registry.interactables.has(entity_other)) {
				// Update current health
				if (registry.healthComponents.has(entity_other)) {
					Mix_PlayChannel(-1, item_acquire_health_sound, 0);
					Health& player_health = registry.healthComponents.get(player_character);
					Health& item_health = registry.healthComponents.get(entity_other);

					player_health.current_health += item_health.current_health;
					player_health.max_health += item_health.current_health;
				}
				// Update current damage output
				else if (registry.damageComponents.has(entity_other)) {
					Mix_PlayChannel(-1, item_acquire_damage_sound, 0);
					Damage& player_damage = registry.damageComponents.get(player_character);
					Damage& item_damage = registry.damageComponents.get(entity_other);

					player_damage.damage_output += item_damage.damage_output;
				}

				registry.remove_all_components_of(entity_other);
			}

			// Get repulsed back
			Motion& player_motion = registry.motions.get(player_character);
			player_motion.position.x += -player_motion.velocity.x * step_seconds;
			player_motion.position.y += -player_motion.velocity.y * step_seconds;
			player_motion.velocity = { 0.f, 0.f };
		}
		// Collisions that involve subbosses
		else if (registry.subBosses.has(entity)) {
			// Checking SubBoss - Non-Traversable collisions (Enemy - Player collision handled above)
			if (!registry.players.has(entity_other) && !registry.traversables.has(entity_other)) {
				// Flip velocity
				Motion& enemy_motion = registry.motions.get(entity);
				enemy_motion.velocity.x = -enemy_motion.velocity.x;
				enemy_motion.velocity.y = -enemy_motion.velocity.y;
			}
		}
	}

	// Remove all collisions from this simulation step
	registry.collisions.clear();
}

// Should the game be over?
bool WorldSystem::is_over() const {
	return bool(glfwWindowShouldClose(window));
}

// On key callback
void WorldSystem::on_key(int key, int, int action, int mod) {
	(int)mod; // dummy to avoid compiler warning

	// The state and its screen
	assert(registry.screenStates.components.size() <= 1);
	ScreenState& state = registry.screenStates.components[0];

	// Handle character movement and direction with `WASD` or arrow keys
	if (state.used_state == STATE_ID::IDLE) {
		Motion& player_motion = registry.motions.get(player_character);
		player_motion.is_enroute = false;
		previous_direction = current_direction;

		if (key == GLFW_KEY_W || key == GLFW_KEY_UP) {
			if (action == GLFW_RELEASE)
				player_motion.velocity.y = 0.f;
			else
				player_motion.velocity.y = -current_speed;
			current_direction = CHARACTER_DIRECTION::UP;
		}

		if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
			if (action == GLFW_RELEASE)
				player_motion.velocity.y = 0.f;
			else
				player_motion.velocity.y = current_speed;
			current_direction = CHARACTER_DIRECTION::DOWN;
		}

		if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
			if (action == GLFW_RELEASE)
				player_motion.velocity.x = 0.f;
			else
				player_motion.velocity.x = -current_speed;
			current_direction = CHARACTER_DIRECTION::LEFT;
		}

		if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
			if (action == GLFW_RELEASE)
				player_motion.velocity.x = 0.f;
			else
				player_motion.velocity.x = current_speed;
			current_direction = CHARACTER_DIRECTION::RIGHT;
		}
	}

	// Print out player's deck of cards
	if (state.used_state == STATE_ID::IDLE) {
		if (action == GLFW_RELEASE && key == GLFW_KEY_I) {
			printf("Player's deck of cards contains:\n");

			for (Entity card : registry.playerDecks.entities) {
				Health& health = registry.healthComponents.get(card);
				Damage& damage = registry.damageComponents.get(card);

				printf("Card %i with %i health and %i damage\n",
					(int)card, (int)health.current_health, (int)damage.damage_output);
			}
		}
	}

	// Battle with cards currently at play and then end player's turn
	Battle& battle = registry.battles.get(player_character);
	if (state.used_state == STATE_ID::BATTLE && battle.is_my_turn) {
		if (action == GLFW_RELEASE && key == GLFW_KEY_SPACE) {
			// Card is removed from play if its health reaches 0 (handled in `WorldSystem::step`)
			play_cards(registry.playerPlays, registry.bossPlays, registry.boardOpponentBars, current_boss);
			registry.renderRequests.get(
				registry.boardModes.entities.back()).used_texture = TEXTURE_ASSET_ID::BOARD_WAIT;
			battle.current_mode = MODE_ID::DONE;
		}
	}

	// Saving game
	if (action == GLFW_PRESS && key == GLFW_KEY_Q) {
		if (state.used_screen == SCREEN_ID::HUB || state.used_screen == SCREEN_ID::BIOME)
			save_game();
		else
			printf("Cannot save during maze or battle\n");
	}
  
	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

		restart_game();
	}

	// Debugging
	if (key == GLFW_KEY_T) {
		if (action == GLFW_RELEASE)
			debugging.in_debug_mode = false;
		else
			debugging.in_debug_mode = true;
	}

	// Escape game
	if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// On mouse movement callback
void WorldSystem::on_mouse_move(vec2 mouse_position) {
	(vec2)mouse_position; // dummy to avoid compiler warning
}

// On mouse click callback
void WorldSystem::on_mouse_button(int button, int action, int mod) {
	(int)mod; // dummy to avoid compiler warning

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		// Get the mouse's current position
		double mouse_x, mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);
		vec2 mouse_position = vec2(mouse_x, mouse_y);
		
		// The screen and its state
		assert(registry.screenStates.components.size() <= 1);
		ScreenState& state = registry.screenStates.components[0];

		if (state.used_screen == SCREEN_ID::MENU) {
			// Determine which button was selected
			auto& motion_registry = registry.motions;

			// Assumption is that there is only one start button
			Entity start = registry.menuStarts.entities.back();
			Motion& motion_start = motion_registry.get(start);
			if (collides_with_mouse(mouse_position, motion_start)) {
				registry.screenTimers.emplace(start);
				state.used_state = STATE_ID::IDLE;
				state.used_screen = SCREEN_ID::HUB;
			}

			// May or may not be a continue button present (depends on `WorldSystem::generateMenu`)
			auto& continue_registry = registry.menuContinues;
			for (Entity entity : continue_registry.entities) {
				Motion& motion_continues = motion_registry.get(entity);
				if (collides_with_mouse(mouse_position, motion_continues)) {
					registry.screenTimers.emplace(entity);
					load_game();
				}
			}			

			// Assumption is that there is only one instructions button
			Entity instructions = registry.menuInstructions.entities.back();
			Motion& motion_instructions = motion_registry.get(instructions);
			if (collides_with_mouse(mouse_position, motion_instructions)) {
				registry.screenTimers.emplace(instructions);
				state.used_screen = SCREEN_ID::INSTRUCTIONS;
			}
		}
		else if (state.used_screen == SCREEN_ID::INSTRUCTIONS) {
			// Assumption is that there is only one close button
			Entity close = registry.instructionsClose.entities.back();
			Motion& motion = registry.motions.get(close);
			if (collides_with_mouse(mouse_position, motion)) {
				registry.screenTimers.emplace(close);
				state.used_screen = SCREEN_ID::MENU;
			}
		}
		else if (state.used_screen == SCREEN_ID::STORY) {
			// Assumption is that there is only one close button
			Entity close = registry.Close.entities.back();
			Motion& motion = registry.motions.get(close);
			if (collides_with_mouse(mouse_position, motion)) {
				registry.screenTimers.emplace(close);
				state.used_screen = SCREEN_ID::HUB;
			}
		}
		else if (state.used_screen == SCREEN_ID::BATTLE) {
			auto& hand_registry = registry.playerHands;
			auto& motion_registry = registry.motions;

			Battle& battle = registry.battles.get(player_character);
			if (battle.is_my_turn && battle.current_mode == MODE_ID::SELECT) {
				// Skip if cards at play have reached the maximum
				if (registry.playerPlays.components.size() < MAX_PLAY) {
					// Add a new card if generator is selected and holding less than three cards
					Motion& generator = motion_registry.get(registry.generators.entities.back());
					if (collides_with_mouse(mouse_position, generator) &&
						hand_registry.components.size() < MAX_HAND) {
						// Get a card from the player's deck
						if (registry.playerDecks.components.size() > 0) {
							Motion& motion = motion_registry.get(get_card_player_deck());
							motion.position = choose_placement_player_hand();
						}
						// Otherwise, generate a new card
						else {
							std::uniform_int_distribution<int> int_dist(1, MAX_HEALTH_DAMAGE);
							Player& player = registry.players.get(player_character);
							Entity card = createCard(renderer, { 0, 0 }, player.current_biome,
								int_dist(rng), MAX_HEALTH_DAMAGE, int_dist(rng));
							
							registry.playerHands.emplace(card);
							registry.motions.get(card).position = choose_placement_player_hand();
						}
					}

					// Determine which card was selected
					if (hand_registry.components.size() > 0) {
						for (int i = (int)hand_registry.components.size() - 1; i >= 0; i--) {
							Entity card = hand_registry.entities[i];
							Motion& motion = motion_registry.get(card);

							// Switch to card placement mode
							if (collides_with_mouse(mouse_position, motion)) {
								selected_card = card;
								registry.renderRequests.get(
									registry.boardModes.entities.back()).used_texture = TEXTURE_ASSET_ID::BOARD_PLACE;
								battle.current_mode = MODE_ID::PLACE;
								break;
							}
						}
					}
				}
				else {
					registry.renderRequests.get(
						registry.boardModes.entities.back()).used_texture = TEXTURE_ASSET_ID::BOARD_PLACE;
					battle.current_mode = MODE_ID::PLACE;
				}
			}
			else if (battle.is_my_turn && battle.current_mode == MODE_ID::PLACE) {
				// Skip if cards at play have reached the maximum
				if (registry.playerPlays.components.size() < MAX_PLAY) {
					// Determine where card is to be placed
					auto& board_player_registry = registry.boardPlayers;
					for (int i = (int)board_player_registry.components.size() - 1; i >= 0; i--) {
						Entity card_placement = board_player_registry.entities[i];
						Motion& motion = motion_registry.get(card_placement);

						// Remove the card from hand, put it in play, and switch to card play mode
						if (collides_with_mouse(mouse_position, motion) &&
							card_placement_unoccupied(motion.position, registry.playerPlays)) {
							hand_registry.remove(selected_card);
							registry.playerPlays.emplace(selected_card);
							motion_registry.get(selected_card).position = motion.position;

							registry.renderRequests.get(
								registry.boardModes.entities.back()).used_texture = TEXTURE_ASSET_ID::BOARD_PLAY;
							battle.current_mode = MODE_ID::PLAY;
							break;
						}
					}
				}
				else {
					registry.renderRequests.get(
						registry.boardModes.entities.back()).used_texture = TEXTURE_ASSET_ID::BOARD_PLAY;
					battle.current_mode = MODE_ID::PLAY;
				}
			}
		}
		else if (state.used_screen == SCREEN_ID::HUB || state.used_screen == SCREEN_ID::BIOME) {
			// Check that player clicked on a legal desitination
			auto& motion_registry = registry.motions;
			auto& traversable_registry = registry.traversables;
			for (uint i = 0; i < traversable_registry.components.size(); i++) {
				// Calculate a path for the player's character to follow
				Motion& motion_i = motion_registry.get(traversable_registry.entities[i]);
				if (collides_with_mouse(mouse_position, motion_i) && !obstacle_on_traversable(mouse_position)) {
					Motion& player_motion = motion_registry.get(player_character);
					player_motion.path = path_find(player_motion.position, mouse_position, player_motion);
					if (player_motion.path == NULL)
						std::cout << "No path found" << std::endl;
					else {
						player_motion.is_enroute = true;
						std::cout << "Path found" << std::endl;
					}
					break;
				}
				else
					std::cout << "Destination is not reachable" << std::endl;
			}
		}
	}
}

// Determine whether entity was clicked on by the mouse
bool WorldSystem::collides_with_mouse(const vec2 mouse_position, const Motion& other)
{
	const vec2 position_start = { 
		other.position.x - (abs(other.scale.x) / 2.f), 
		other.position.y - (abs(other.scale.y) / 2.f) };
	const vec2 position_end = { 
		other.position.x + (abs(other.scale.x) / 2.f), 
		other.position.y + (abs(other.scale.y) / 2.f) };

	if (mouse_position.x >= position_start.x && 
		mouse_position.x <= position_end.x && 
		mouse_position.y >= position_start.y && 
		mouse_position.y <= position_end.y)
		return true;
	return false;
}

// Determine whether another entity is rendered on top of a traversable
bool WorldSystem::obstacle_on_traversable(vec2 mouse_position) {
	bool occupied = false;
	auto& motion_registry = registry.motions;
	for (uint i = 0; i < motion_registry.components.size(); i++) {
		// Skip entities that are traversable
		Entity entity_i = motion_registry.entities[i];
		if (registry.traversables.has(entity_i))
			continue;

		Motion& motion_i = motion_registry.components[i];
		if (collides_with_mouse(mouse_position, motion_i)) {
			occupied = true;
			break;
		}
	}

	return occupied;
}

// Use Manhatten heuristic as player can only move in 4 directions
double WorldSystem::calc_heuristic(vec2 a, vec2 b) {
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// Determines path player's character should follow; returns NULL if no path found
Path* WorldSystem::path_find(vec2 start, vec2 dest, Motion& motion_p) {
	Path* path = new Path();
	path->next = start;

	struct Node {
		vec2 pos;
		int cost; // distance to get to this node, following path laid out via parents
		int heuristic; // Euclidean distance from this node to the destination
		Node* parent;

		bool operator == (const Node& a) {
			return (pos.x == a.pos.x && pos.y == a.pos.y);
		}
	};

	std::vector<Node*> open_list;
	std::vector<Node*> closed_list;

	Node* start_node = new Node({ start, 0, 0, NULL });
	open_list.push_back(start_node);

	// `open_list` will have decsending order where the lowest `f` is at the back of the list 
	while (!open_list.empty()) {
		// Access current node that has the smallest f (an estimate of the
		// cost of a path from the start to a goal via q) on the list
		Node* current = open_list.back();

		// Pop current node off of the open list
		open_list.pop_back();

		// Check if current node is the destination
		if (calc_heuristic(current->pos, dest) <= GRANULARITY * 2) {
			path->path_stack.push(current->pos);
			while (current->parent != NULL) {
				current = current->parent;
				path->path_stack.push(current->pos);
			}
			return path;
		}
		printf("(x,y) = (%f, %f)\n", current->pos.x, current->pos.y);
		
		// Set neighbours
		std::vector<Node*> neighbours = {
			new Node({{ current->pos.x - GRANULARITY,current->pos.y }, GRANULARITY, 0, current}),
			new Node({{ current->pos.x + GRANULARITY,current->pos.y }, GRANULARITY, 0, current}),
			new Node({{ current->pos.x, current->pos.y - GRANULARITY }, GRANULARITY, 0, current}),
			new Node({{ current->pos.x, current->pos.y + GRANULARITY }, GRANULARITY, 0, current})
		};

		for (Node* nextp : neighbours) {
			Node& next = *nextp;

			// Check if neighbour is an obstacle; if true, put it in on the closed list and skip
			int next_is_obstacle = false;
			ComponentContainer<Motion> &motion_container = registry.motions;
			for (uint i = 0; i < motion_container.components.size(); i++)
			{
				Entity entity_i = motion_container.entities[i];
				// Skip entities that are traversable and the player character
				if (registry.traversables.has(entity_i) || registry.players.has(entity_i))
					continue;

				// Skip mobile entities
				Motion& motion_i = motion_container.components[i];
				if (motion_i.velocity != vec2(0,0))
					continue;

				if (PhysicsSystem::collides(motion_i, 
					Motion{ next.pos, motion_p.angle, { 0, 0 }, motion_p.scale})) {
					std::cout << "Collision detected" << std::endl;
					next_is_obstacle = true;
					break;
				}
			}
			if (next_is_obstacle)
			{
				next.heuristic = 0; // ensures that node will be percived as already searched
				closed_list.push_back(nextp);
				continue;
			}

			next.cost = current->cost + next.cost;
			next.heuristic = calc_heuristic(next.pos, dest);

			auto check_list_for_same_and_less_f = [](Node next, std::vector<Node*> list) {
				for (Node* n : list) {
					if (next == *n && (n->cost + n->heuristic) <= (next.cost + next.heuristic))
						return true;
				}
				return false;
			};

			if (check_list_for_same_and_less_f(next, open_list) ||
				check_list_for_same_and_less_f(next, closed_list))
				continue;

			open_list.push_back(nextp);

			// Computationally cheaper to sort an already sorted list
			auto sort_list = [](std::vector<Node*>* list) {
				std::stable_sort(list->begin(), list->end(), 
					[](Node* n1, Node* n2) { return n1->cost + n1->heuristic > n2->cost + n2->heuristic; });
			};
			sort_list(&open_list);
		}
		closed_list.push_back(current);
	}
	
	// No path found
	return NULL;
}