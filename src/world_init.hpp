#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"

// These are hard coded to the dimensions of the entity texture
const float MENU_BUTTON_BB_WIDTH = 3.17f * 64.f;
const float MENU_BUTTON_BB_HEIGHT = 0.84f * 64.f;
const float INSTRUCTIONS_CLOSE_BB_WIDTH = 0.75f * 64.f;
const float INSTRUCTIONS_CLOSE_BB_HEIGHT = 0.75f * 64.f;
const float CHARACTER_BB_WIDTH = 1.f * 64.f;
const float CHARACTER_BB_HEIGHT = 1.f * 64.f;
const float NPC_BB_WIDTH = 0.9f * 64.f;
const float NPC_BB_HEIGHT = 0.9f * 64.f;
const float SUBBOSS_BB_WIDTH = 0.78f * 64.f;
const float SUBBOSS_BB_HEIGHT = 0.78f * 64.f;
const float BOSS_BB_WIDTH = 1.25f * 64.f;
const float BOSS_BB_HEIGHT = 1.17f * 64.f;
const float CHEST_BB_WIDTH = 0.5f * 64.f;
const float CHEST_BB_HEIGHT = 0.5f * 64.f;
const float CARD_BB_WIDTH = 3.125f * 64.f;
const float CARD_BB_HEIGHT = 4.f * 64.f;
const float CARD_EMPTY_BB_WIDTH = 3.05f * 64.f;
const float CARD_EMPTY_BB_HEIGHT = 3.92f * 64.f;
const float BOARD_MODE_BB_WIDTH = 6.65f * 64.f;
const float BOARD_MODE_BB_HEIGHT = 0.81f * 64.f;
const float TREE_BB_WIDTH = 1.f * 64.f;
const float TREE_BB_HEIGHT = 1.f * 64.f;
const float ITEM_BB_WIDTH = 0.5f * 64.f;
const float ITEM_BB_HEIGHT = 0.45f * 64.f;
const float TILE_BB_WIDTH = 1.f * 64.f;
const float TILE_BB_HEIGHT = 1.f * 64.f;

// These are hard coded to the number of frames in the entity texture
const int ITEM_HEALTH_NUM_FRAMES = 8;
const int ITEM_DAMAGE_NUM_FRAMES = 10;
const int ENEMY_NUM_FRAMES = 3;

enum class TILES {
	PATH = 0,
	WALL = PATH + 1
};

enum class CHARACTER_DIRECTION {
	UP = 0,
	DOWN = UP + 1,
	LEFT = DOWN + 1,
	RIGHT = LEFT + 1
};

// the menu
Entity createMenu(RenderSystem* renderer, vec2 position);
Entity createMenuStart(RenderSystem* renderer, vec2 position);
Entity createMenuContinue(RenderSystem* renderer, vec2 position);
Entity createMenuInstructions(RenderSystem* renderer, vec2 position);
Entity createInstructions(RenderSystem* renderer, vec2 position);
Entity createInstructionsClose(RenderSystem* renderer, vec2 position);
Entity createStory(RenderSystem* renderer, vec2 position);
Entity createClose(RenderSystem* renderer, vec2 position);
// game over
Entity createGameOver(RenderSystem* renderer, vec2 position);
// the player
Entity createCharacter(RenderSystem* renderer, vec2 position);
// the npc
Entity createNpc(RenderSystem* renderer, vec2 position);
// the enemies
Entity createSubBoss(RenderSystem* renderer, vec2 position, BIOME_ID used_biome);
Entity createBoss(RenderSystem* renderer, vec2 position, BIOME_ID used_biome);
// the chests
Entity createChest(RenderSystem* renderer, vec2 position, TIER_ID used_tier);
// the cards
Entity createCardPlacement(RenderSystem* renderer, vec2 position, bool is_for_player);
Entity createCardGenerator(RenderSystem* renderer, vec2 position);
Entity createCard(RenderSystem* renderer, vec2 position, BIOME_ID used_biome,
	int current_health, int max_health, int damage_output);
Entity loadCard(RenderSystem* renderer, int current_health, int max_health, int damage_output);
// the board
Entity createModeUI(RenderSystem* renderer, vec2 position);
Entity createBarUI(vec2 position, vec2 size);
// the tiles
Entity createPath(RenderSystem* renderer, vec2 position, BIOME_ID used_biome);
Entity createWall(RenderSystem* renderer, vec2 position, BIOME_ID used_biome);
// the non-interactable objects
Entity createTree(RenderSystem* renderer, vec2 position, TEXTURE_ASSET_ID used_texture);
Entity createTimer(RenderSystem* renderer, vec2 position, int currentFrame, float updateTime);
// the interactable objects
Entity createHealth(RenderSystem* renderer, vec2 position);
Entity createDamage(RenderSystem* renderer, vec2 position);
// the biomes
Entity createBiome(RenderSystem* renderer, vec2 position, BIOME_ID used_biome);
// a red line for debugging purposes
Entity createLine(vec2 position, vec2 size);