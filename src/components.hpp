#pragma once
#include "common.hpp"
#include <stack>
#include <vector>
#include <unordered_map>
#include "../ext/stb_image/stb_image.h"

// Enumerator to represent the current biome player is in
enum class BIOME_ID
{
	HUB = 0,
	FROST = HUB + 1,
	BEACH = FROST + 1,
	LAVA = BEACH + 1,
	JUNGLE = LAVA + 1,
	BIOME_COUNT = JUNGLE + 1
};
const int biome_count = (int)BIOME_ID::BIOME_COUNT;

// Player component
struct Player
{
	BIOME_ID current_biome = BIOME_ID::BIOME_COUNT;
};

// NPC component
struct Npc
{

};

// Sub-Boss and Boss have a hard shell
struct Enemy
{

};

// World belongs to a certain biome
struct Biome
{
	BIOME_ID used_biome = BIOME_ID::BIOME_COUNT;
};

// Card may be added to hand
struct Hand
{

};

// Card may be added to deck
struct Deck
{

};

// Card may be played during battle
struct Play
{

};

// Card generator component
struct Generator
{

};

// Board UI component
struct BoardUI
{

};

// Menu UI component
struct MenuUI
{

};

// Instructions UI component
struct InstructionsUI
{

};

struct StoryUI
{

};

// Health component assigned to player and cards
struct Health
{
	float current_health = 50;
	float max_health = 50;
};

// Damage component assigned to player and cards
struct Damage
{
	float damage_output = 50;
};

// Damage component assigned to player and cards
struct Timer
{
	float updateTime = 0;
	
};

// Items the player can aquire
struct Interactable
{

};

// Component where no collision occurs
struct Traversable
{

};

// Chests can be looted
struct Lootable
{
	bool is_looted = 0;
};

// Enumerator to represent the tiers chests can be assigned
enum class TIER_ID {
	BRONZE = 0,
	SILVER = BRONZE + 1,
	GOLD = SILVER + 1,
	TIER_COUNT = GOLD + 1
};
const int tier_count = (int)TIER_ID::TIER_COUNT;

// Chests have different tiers
struct Tier
{
	TIER_ID used_tier = TIER_ID::TIER_COUNT;
};

// Structure to store path information
struct Path {
	vec2 next = { -1, -1 }; // current step on way to destination
	std::stack<vec2> path_stack; // list of postions to travel to on way to destination
};

// All data relevant to the shape and motion of entities
struct Motion 
{
	vec2 position = { 0, 0 };
	float angle = 0;
	vec2 velocity = { 0, 0 };
	vec2 scale = { 10, 10 };
	bool is_enroute = false;
	struct Path* path;
};

// Enumerator to represent current mode of card battle
enum class MODE_ID
{
	SELECT = 0,
	PLACE = SELECT + 1,
	PLAY = PLACE + 1,
	DONE = PLAY + 1,
	MODE_COUNT = DONE + 1
};
const int mode_count = (int)MODE_ID::MODE_COUNT;

// Structure to store battle information
struct Battle
{
	bool is_my_turn = 0;
	MODE_ID current_mode = MODE_ID::SELECT;
};

// Stucture to store collision information
struct Collision
{
	// Note, the first object is stored in the ECS container.entities
	Entity other; // the second object involved in the collision
	Collision(Entity& other) { this->other = other; };
};

// Structure to store spritesheet information
struct Animation 
{
	int current_frame = 0;
	int num_frames = 1;
	float elapsed_ms = 0.f;
};

struct Progression
{
	bool is_jungle_biome_defeated = 0;
	bool is_lava_biome_defeated = 0;
	bool is_beach_biome_defeated = 0;
	bool is_frost_biome_defeated = 0;
};

// Data structure for toggling debug mode
struct Debug 
{
	bool in_debug_mode = 0;
	bool in_freeze_mode = 0;
};
extern Debug debugging;

// A struct to refer to debugging graphics in the ECS
struct DebugComponent
{
	// Note, an empty struct has size 1
};

// Enumerator to represent current screen state
enum class STATE_ID
{
	IDLE = 0,
	LOOT = IDLE + 1,
	BATTLE = LOOT + 1,
	STATE_COUNT = BATTLE + 1
};
const int state_count = (int)STATE_ID::STATE_COUNT;

// Enumerator to represent current screen
enum class SCREEN_ID {
	MENU = 0,
	INSTRUCTIONS = MENU +1,
	HUB = INSTRUCTIONS + 1,
	STORY = HUB + 1,
	BIOME = STORY + 1,
	MAZE = BIOME + 1,
	BATTLE = MAZE + 1,
	GAMEOVER = BATTLE + 1,
	SCREEN_COUNT = GAMEOVER + 1
};
const int screen_count = (int)SCREEN_ID::SCREEN_COUNT;

// Sets the state of the screen and game
struct ScreenState
{
	float darken_screen_factor = -1;
	STATE_ID used_state = STATE_ID::STATE_COUNT;
	SCREEN_ID used_screen = SCREEN_ID::SCREEN_COUNT;
};

// A timer that will be associated to changing screens
struct ScreenTimer
{
	float counter_ms = 1000;
};

// A timer that will be associated to appearing card
struct CardAppearTimer
{
	float counter_ms = 2000;
};

// A timer that will be associated to length of card battle mode
struct BossModeTimer
{
	float counter_ms = 2000;
};

// Single Vertex Buffer element for non-textured meshes (coloured.vs.glsl & chicken.vs.glsl)
struct ColoredVertex
{
	vec3 position;
	vec3 color;
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex
{
	vec3 position;
	vec2 texcoord;
};

// Mesh datastructure for storing vertex and index buffers
struct Mesh
{
	static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
	vec2 original_size = { 1,1 };
	std::vector<ColoredVertex> vertices;
	std::vector<uint16_t> vertex_indices;
};

/**
 * The following enumerators represent global identifiers refering to graphic
 * assets. For example TEXTURE_ASSET_ID are the identifiers of each texture
 * currently supported by the system.
 *
 * So, instead of referring to a game asset directly, the game logic just
 * uses these enumerators and the RenderRequest struct to inform the renderer
 * how to structure the next draw command.
 *
 * There are 2 reasons for this:
 *
 * First, game assets such as textures and meshes are large and should not be
 * copied around as this wastes memory and runtime. Thus separating the data
 * from its representation makes the system faster.
 *
 * Second, it is good practice to decouple the game logic from the render logic.
 * Imagine, for example, changing from OpenGL to Vulkan, if the game logic
 * depends on OpenGL semantics it will be much harder to do the switch than if
 * the renderer encapsulates all asset data and the game logic is agnostic to it.
 *
 * The final value in each enumeration is both a way to keep track of how many
 * enums there are, and as a default value to represent uninitialized fields.
 */

enum class TEXTURE_ASSET_ID 
{
	MENU = 0,
	MENU_START = MENU + 1,
	MENU_CONTINUE = MENU_START + 1,
	MENU_INSTRUCTIONS = MENU_CONTINUE + 1,
	INSTRUCTIONS = MENU_INSTRUCTIONS +1,
	INSTRUCTIONS_CLOSE = INSTRUCTIONS +1,
	NPC = INSTRUCTIONS_CLOSE + 1,
	STORY = NPC + 1,
	CLOSE = STORY +1,
	CHARACTER_UP = CLOSE + 1,
	CHARACTER_DOWN = CHARACTER_UP + 1,
	CHARACTER_LEFT = CHARACTER_DOWN + 1,
	CHARACTER_RIGHT = CHARACTER_LEFT + 1,
	BIOME_HUB = CHARACTER_RIGHT + 1,
	BIOME_HUB_PATH = BIOME_HUB + 1,
	BIOME_HUB_WALL = BIOME_HUB_PATH + 1,
	BIOME_FROST = BIOME_HUB_WALL + 1,
	BIOME_FROST_PATH = BIOME_FROST + 1,
	BIOME_FROST_WALL = BIOME_FROST_PATH + 1,
	BIOME_BEACH = BIOME_FROST_WALL + 1,
	BIOME_BEACH_PATH = BIOME_BEACH + 1,
	BIOME_BEACH_WALL = BIOME_BEACH_PATH + 1,
	BIOME_LAVA = BIOME_BEACH_WALL + 1,
	BIOME_LAVA_PATH = BIOME_LAVA + 1,
	BIOME_LAVA_WALL = BIOME_LAVA_PATH + 1,
	BIOME_JUNGLE = BIOME_LAVA_WALL + 1,
	BIOME_JUNGLE_PATH = BIOME_JUNGLE + 1,
	BIOME_JUNGLE_WALL = BIOME_JUNGLE_PATH + 1,
	CHEST_BRONZE_OPENED = BIOME_JUNGLE_WALL + 1,
	CHEST_BRONZE_CLOSED = CHEST_BRONZE_OPENED + 1,
	CHEST_SILVER_OPENED = CHEST_BRONZE_CLOSED + 1,
	CHEST_SILVER_CLOSED = CHEST_SILVER_OPENED + 1,
	CHEST_GOLD_OPENED = CHEST_SILVER_CLOSED + 1,
	CHEST_GOLD_CLOSED = CHEST_GOLD_OPENED + 1,
	ENEMY_GHOST_LEFT = CHEST_GOLD_CLOSED + 1,
	ENEMY_GHOST_RIGHT = ENEMY_GHOST_LEFT + 1,
	PALM_TREE_1 = ENEMY_GHOST_RIGHT + 1,
	PALM_TREE_2 = PALM_TREE_1 + 1,
	PALM_TREE_3 = PALM_TREE_2 + 1,
	PALM_TREE_4 = PALM_TREE_3 + 1,
	PALM_TREE_WITH_COCONUT_1 = PALM_TREE_4 + 1,
	PALM_TREE_WITH_COCONUT_2 = PALM_TREE_WITH_COCONUT_1 + 1,
	BOARD_PLAYER = PALM_TREE_WITH_COCONUT_2 + 1,
	BOARD_OPPONENT = BOARD_PLAYER + 1,
	BOARD_GENERATOR = BOARD_OPPONENT + 1,
	BOARD_SELECT = BOARD_GENERATOR + 1,
	BOARD_PLACE = BOARD_SELECT + 1,
	BOARD_PLAY = BOARD_PLACE + 1,
	BOARD_WAIT = BOARD_PLAY + 1,
	CARD_FRAME = BOARD_WAIT + 1,
	CARD_HEALTH = CARD_FRAME + 1,
	CARD_DAMAGE = CARD_HEALTH + 1,
	ITEM_HEALTH = CARD_DAMAGE + 1,
	ITEM_DAMAGE = ITEM_HEALTH + 1,
	SUB_BEACH = ITEM_DAMAGE + 1,
	SUB_FROST = SUB_BEACH + 1,
	SUB_JUNGLE = SUB_FROST +1,
	SUB_LAVA = SUB_JUNGLE + 1,
	BOSS_BEACH = SUB_LAVA + 1,
	BOSS_FROST = BOSS_BEACH + 1,
	BOSS_JUNGLE = BOSS_FROST + 1,
	BOSS_LAVA = BOSS_JUNGLE + 1,
	TIMER = BOSS_LAVA + 1,
	GAMEOVER = TIMER + 1,
	TEXTURE_COUNT = GAMEOVER + 1,
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID 
{
	COLOURED = 0,
	EGG = COLOURED + 1,
	CARD = EGG + 1,
	ANIMATED = CARD + 1,
	TEXTURED = ANIMATED + 1,
	WIND = TEXTURED + 1,
	EFFECT_COUNT = WIND + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID 
{
	SPRITE = 0,
	EGG = SPRITE + 1,
	DEBUG_LINE = EGG + 1,
	SCREEN_TRIANGLE = DEBUG_LINE + 1,
	GEOMETRY_COUNT = SCREEN_TRIANGLE + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct RenderRequest 
{
	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
};