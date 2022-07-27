#pragma once

#include <array>
#include <utility>

#include "common.hpp"
#include "components.hpp"
#include "tiny_ecs.hpp"

// System responsible for setting up OpenGL and for rendering all the
// visual entities in the game
class RenderSystem {
	/**
	 * The following arrays store the assets the game will use. They are loaded
	 * at initialization and are assumed to not be modified by the render loop.
	 *
	 * Whenever possible, add to these lists instead of creating dynamic state
	 * it is easier to debug and faster to execute for the computer.
	 */
	std::array<GLuint, texture_count> texture_gl_handles;
	std::array<ivec2, texture_count> texture_dimensions;

	// Make sure these paths remain in sync with the associated enumerators.
	// Associated id with .obj path
	const std::vector < std::pair<GEOMETRY_BUFFER_ID, std::string>> mesh_paths =
	{
		// specify meshes of other assets here
	};

	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, texture_count> texture_paths = {
			textures_path("main_menu.png"),
			textures_path("main_menu_start.png"),
			textures_path("main_menu_continue.png"),
			textures_path("main_menu_instructions.png"),
			textures_path("instructions_list.png"),
			textures_path("instructions_close.png"),
			textures_path("npc.png"),
			textures_path("story.png"),
			textures_path("instructions_close.png"),
			textures_path("character_up.png"),
			textures_path("character_down.png"),
			textures_path("character_left.png"),
			textures_path("character_right.png"),
			textures_path("biome_hub.png"),
			textures_path("biome_hub_path.png"),
			textures_path("biome_hub_wall.png"),
			textures_path("biome_frost.png"),
			textures_path("biome_frost_path.png"),
			textures_path("biome_frost_wall.png"),
			textures_path("biome_beach.png"),
			textures_path("biome_beach_path.png"),
			textures_path("biome_beach_wall.png"),
			textures_path("biome_lava.png"),
			textures_path("biome_lava_path.png"),
			textures_path("biome_lava_wall.png"),
			textures_path("biome_jungle.png"),
			textures_path("biome_jungle_path.png"),
			textures_path("biome_jungle_wall.png"),
			textures_path("chest_bronze_opened.png"),
			textures_path("chest_bronze_closed.png"),
			textures_path("chest_silver_opened.png"),
			textures_path("chest_silver_closed.png"),
			textures_path("chest_gold_opened.png"),
			textures_path("chest_gold_closed.png"),
			textures_path("enemy_ghost_left.png"),
			textures_path("enemy_ghost_right.png"),
			textures_path("palm_tree_1.png"),
			textures_path("palm_tree_2.png"),
			textures_path("palm_tree_3.png"),
			textures_path("palm_tree_4.png"),
			textures_path("palm_tree_with_coconut_1.png"),
			textures_path("palm_tree_with_coconut_2.png"),
			textures_path("board_card_player.png"),
			textures_path("board_card_opponent.png"),
			textures_path("board_card_generator.png"),
			textures_path("board_mode_select.png"),
			textures_path("board_mode_place.png"),
			textures_path("board_mode_play.png"),
			textures_path("board_mode_wait.png"),
			textures_path("card_frame.png"),
			textures_path("card_health.png"),
			textures_path("card_damage.png"),
			textures_path("item_health.png"),
			textures_path("item_damage.png"),
			textures_path("sub_beach.png"),
			textures_path("sub_frost.png"),
			textures_path("sub_jungle.png"),
			textures_path("sub_lava.png"),
			textures_path("boss_beach.png"),
			textures_path("boss_frost.png"),
			textures_path("boss_jungle.png"),
			textures_path("boss_lava.png"),
			textures_path("timer.png"),
			textures_path("game_over.png")

	};

	std::array<GLuint, effect_count> effects;
	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, effect_count> effect_paths = {
		shader_path("coloured"),
		shader_path("egg"),
		shader_path("card"),
		shader_path("animated"),
		shader_path("textured"),
		shader_path("wind") };

	std::array<GLuint, geometry_count> vertex_buffers;
	std::array<GLuint, geometry_count> index_buffers;
	std::array<Mesh, geometry_count> meshes;

public:
	// Initialize the window
	bool init(GLFWwindow* window);

	template <class T>
	void bindVBOandIBO(GEOMETRY_BUFFER_ID gid, std::vector<T> vertices, std::vector<uint16_t> indices);

	void initializeGlTextures();

	void initializeGlEffects();

	void initializeGlMeshes();
	Mesh& getMesh(GEOMETRY_BUFFER_ID id) { return meshes[(int)id]; };

	void initializeGlGeometryBuffers();
	// Initialize the screen texture used as intermediate render target
	// The draw loop first renders to this texture, then it is used for the wind
	// shader
	bool initScreenTexture();

	// Destroy resources associated to one or all entities created by the system
	~RenderSystem();

	// Draw all entities
	void draw();

	mat3 createProjectionMatrix();

private:
	// Internal drawing functions for each entity type
	void drawTexturedMesh(Entity entity, const mat3& projection);
	void drawToScreen();

	// Window handle
	GLFWwindow* window;

	// Screen texture handles
	GLuint frame_buffer;
	GLuint off_screen_render_buffer_color;
	GLuint off_screen_render_buffer_depth;

	Entity screen_state_entity;
};

bool loadEffectFromFile(
	const std::string& vs_path, const std::string& fs_path, GLuint& out_program);