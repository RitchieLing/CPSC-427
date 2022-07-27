#pragma once

// internal
#include "common.hpp"

// stlib
#include <vector>
#include <random>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

#include "render_system.hpp"
#include "world_init.hpp"

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class WorldSystem
{
public:
	WorldSystem();

	// Creates a window
	GLFWwindow* create_window();

	// starts the game
	void init(RenderSystem* renderer);

	// Releases all associated resources
	~WorldSystem();

	// Steps the game ahead by ms milliseconds
	bool step(float elapsed_ms);

	// Check for collisions
	void handle_collisions(float elapsed_ms);

	// Should the game be over?
	bool is_over()const;

	// Structure for supporting saving/loading of game state
	class SaveData {
		SCREEN_ID screen;
		BIOME_ID biome;
		bool is_jungle_biome_defeated,
			is_lava_biome_defeated,
			is_beach_biome_defeated,
			is_frost_biome_defeated;
		std::vector<Entity> cards;
	public:
		SaveData(
			SCREEN_ID screen_arg,
			BIOME_ID biome_arg,
			bool is_jungle_biome_defeated_arg,
			bool is_lava_biome_defeated_arg,
			bool is_beach_biome_defeated_arg,
			bool is_frost_biome_defeated_arg,
			std::vector<Entity> cards) {
			this->screen = screen_arg;
			this->biome = biome_arg;
			this->is_jungle_biome_defeated = is_jungle_biome_defeated_arg;
			this->is_lava_biome_defeated = is_lava_biome_defeated_arg;
			this->is_beach_biome_defeated = is_beach_biome_defeated_arg;
			this->is_frost_biome_defeated = is_frost_biome_defeated_arg;
			this->cards = cards;
		}
		friend std::ostream& operator<<(std::ostream& os, const SaveData& sd);
	};

	// Path calculator
	static const int GRANULARITY; // size of nodes the map is divided into
	static double calc_heuristic(vec2 a, vec2 b);
	struct Path* path_find(vec2 start, vec2 dest, Motion& motion_p);
private:
	// Input callback functions
	void on_key(int key, int, int action, int mod);
	void on_mouse_move(vec2 pos);
	void on_mouse_button(int button, int action, int mod);

	// Determine if entity was clicked on
	bool collides_with_mouse(const vec2 mouse_position, const Motion& other);
	bool obstacle_on_traversable(vec2 mouse_position);

	// Save/load game
	void save_game();
	void load_game();

	// Restart level
	void restart_game();

	// Menu generator
	void generate_menu();
	void generate_instructions();
	void generate_story();

	// Game over
	void generate_game_over();

	// World generator
	void generate_hub();

	// Random map generator
	void walk_path();
	void generate_random_map();

	// Random maze generator
	int choose_orientation(int width, int height);
	void divide_maze(int x, int y, int width, int height, int orientation);
	void generate_random_maze();

	// Choose position in world to place entity
	vec2 choose_position(std::vector<int>& tiles);

	// Board generator for card battle
	void generate_board();

	// OpenGL window handle
	GLFWwindow* window;

	// Game state
	RenderSystem* renderer;
	float current_speed;
	CHARACTER_DIRECTION previous_direction;
	CHARACTER_DIRECTION current_direction;
	Entity player_character;

	// World references
	std::vector<int> hub;
	std::vector<int> map;
	std::vector<int> maze;

	// Card battle references
	Entity selected_card;
	Entity current_boss;

	// Maze references
	Entity timer1;
	Entity timer2;

	// Music references
	Mix_Music* background_music;
	Mix_Chunk* character_teleport_sound;
	Mix_Chunk* chest_open_sound;
	Mix_Chunk* item_acquire_health_sound;
	Mix_Chunk* item_acquire_damage_sound;

	// C++ random number generator
	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniform_dist; // number between 0..1
};