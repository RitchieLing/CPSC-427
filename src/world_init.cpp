#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

Entity createMenu(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values
	motion.scale = vec2({ window_width_px, window_width_px });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MENU,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createMenuStart(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values
	motion.scale = vec2({ MENU_BUTTON_BB_WIDTH, MENU_BUTTON_BB_HEIGHT });

	// Create an (empty) MenuStart component to be able to refer to all start buttons
	registry.menuStarts.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MENU_START,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createMenuContinue(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values
	motion.scale = vec2({ MENU_BUTTON_BB_WIDTH, MENU_BUTTON_BB_HEIGHT });

	// Create an (empty) MenuContinue component to be able to refer to all start buttons
	registry.menuContinues.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MENU_CONTINUE,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createMenuInstructions(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values
	motion.scale = vec2({ MENU_BUTTON_BB_WIDTH, MENU_BUTTON_BB_HEIGHT });

	// Create an (empty) MenuInstructions component to be able to refer to all instruction buttons
	registry.menuInstructions.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MENU_INSTRUCTIONS,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createInstructionsClose(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values
	motion.scale = vec2({ INSTRUCTIONS_CLOSE_BB_WIDTH, INSTRUCTIONS_CLOSE_BB_HEIGHT });

	// Create an (empty) InstructionsClose component to be able to refer to all close buttons
	registry.instructionsClose.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::INSTRUCTIONS_CLOSE,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createClose(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values
	motion.scale = vec2({ INSTRUCTIONS_CLOSE_BB_WIDTH, INSTRUCTIONS_CLOSE_BB_HEIGHT });

	// Create an (empty) InstructionsClose component to be able to refer to all close buttons
	registry.Close.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::CLOSE,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createGameOver(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values
	motion.scale = vec2({ window_width_px, window_width_px });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::GAMEOVER,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createInstructions(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values
	motion.scale = vec2({ window_width_px, window_width_px });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::INSTRUCTIONS,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createCharacter(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values
	motion.scale = vec2({ CHARACTER_BB_WIDTH, CHARACTER_BB_HEIGHT });

	// Create an (empty) Character component to be able to refer to all characters
	registry.players.emplace(entity);
	registry.battles.emplace(entity);
	registry.progressions.emplace(entity);
	registry.healthComponents.emplace(entity);
	registry.damageComponents.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::CHARACTER_DOWN,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createNpc(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ NPC_BB_WIDTH, NPC_BB_HEIGHT });

	// Create an (empty) NPC component to be able to refer to all NPCs
	registry.npcs.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::NPC,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createStory(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values
	motion.scale = vec2({ window_width_px, window_width_px });

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::STORY,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}


Entity createSubBoss(RenderSystem* renderer, vec2 position, BIOME_ID used_biome)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 100.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ SUBBOSS_BB_WIDTH, SUBBOSS_BB_HEIGHT });
	TEXTURE_ASSET_ID used_texture;

	// Create an (empty) SubBoss component to be able to refer to all sub-bosses
	registry.subBosses.emplace(entity);
  
	Animation& animation = registry.animations.emplace(entity);
	animation.num_frames = ENEMY_NUM_FRAMES;

	switch (used_biome)
	{
	default:
	case BIOME_ID::FROST:
		used_texture = TEXTURE_ASSET_ID::SUB_FROST;
		break;
	case BIOME_ID::BEACH:
		used_texture = TEXTURE_ASSET_ID::SUB_BEACH;
		break;
	case BIOME_ID::LAVA:
		used_texture = TEXTURE_ASSET_ID::SUB_LAVA;
		break;
	case BIOME_ID::JUNGLE:
		used_texture = TEXTURE_ASSET_ID::SUB_JUNGLE;
		break;
	}

	registry.renderRequests.insert(
		entity,
		{ used_texture,
		  EFFECT_ASSET_ID::ANIMATED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createBoss(RenderSystem* renderer, vec2 position, BIOME_ID used_biome)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 100.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ BOSS_BB_WIDTH, BOSS_BB_HEIGHT });
	TEXTURE_ASSET_ID used_texture;

	// Create an (empty) Boss component to be able to refer to all Bosses
	registry.bosses.emplace(entity);
	registry.battles.emplace(entity);
	registry.healthComponents.emplace(entity);
	registry.damageComponents.emplace(entity);
  
	Animation& animation = registry.animations.emplace(entity);
	animation.num_frames = ENEMY_NUM_FRAMES;

	// Set texture to be used
	switch (used_biome)
	{
	default:
	case BIOME_ID::FROST:
		used_texture = TEXTURE_ASSET_ID::BOSS_FROST;
		break;
	case BIOME_ID::BEACH:
		used_texture = TEXTURE_ASSET_ID::BOSS_BEACH;
		break;
	case BIOME_ID::LAVA:
		used_texture = TEXTURE_ASSET_ID::BOSS_LAVA;
		break;
	case BIOME_ID::JUNGLE:
		used_texture = TEXTURE_ASSET_ID::BOSS_JUNGLE;
		break;
	}

	registry.renderRequests.insert(
		entity,
		{ used_texture,
		  EFFECT_ASSET_ID::ANIMATED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createChest(RenderSystem* renderer, vec2 position, TIER_ID used_tier)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ CHEST_BB_WIDTH, CHEST_BB_HEIGHT });

	// Create an (empty) Chest component to be able to refer to all chests
	registry.lootables.emplace(entity);
	auto& tier = registry.tiers.emplace(entity);
	tier.used_tier = used_tier;

	// Set texture to be used
	TEXTURE_ASSET_ID used_texture;
	switch (used_tier)
	{
	default:
	case TIER_ID::BRONZE:
		used_texture = TEXTURE_ASSET_ID::CHEST_BRONZE_CLOSED;
		break;
	case TIER_ID::SILVER:
		used_texture = TEXTURE_ASSET_ID::CHEST_SILVER_CLOSED;
		break;
	case TIER_ID::GOLD:
		used_texture = TEXTURE_ASSET_ID::CHEST_GOLD_CLOSED;
		break;
	}

	registry.renderRequests.insert(
		entity,
		{ used_texture,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createCardPlacement(RenderSystem* renderer, vec2 position, bool is_for_player)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ CARD_BB_WIDTH, CARD_BB_HEIGHT });

	// Create an (empty) CardPlacement component to be able to refer to all card placements
	if (is_for_player)
		registry.boardPlayers.emplace(entity);
	else
		registry.boardBosses.emplace(entity);

	// Set texture to be used
	TEXTURE_ASSET_ID used_texture;
	if (is_for_player)
		used_texture = TEXTURE_ASSET_ID::BOARD_PLAYER;
	else
		used_texture = TEXTURE_ASSET_ID::BOARD_OPPONENT;

	registry.renderRequests.insert(
		entity,
		{ used_texture,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createCardGenerator(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ CARD_BB_WIDTH, CARD_BB_HEIGHT });

	// Create an (empty) CardGenerator component to be able to refer to all card generators
	registry.generators.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BOARD_GENERATOR,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createCard(RenderSystem* renderer, vec2 position, BIOME_ID used_biome,
	int current_health, int max_health, int damage_output)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ CARD_BB_WIDTH, CARD_BB_HEIGHT });

	// Create an (empty) Card component to be able to refer to all cards
	auto& health = registry.healthComponents.emplace(entity);
	health.current_health = current_health;
	health.max_health = max_health;

	auto& damage = registry.damageComponents.emplace(entity);
	damage.damage_output = damage_output;

	// Set texture to be used
	TEXTURE_ASSET_ID used_texture;
	switch (used_biome)
	{
	default:
	case BIOME_ID::FROST:
		used_texture = TEXTURE_ASSET_ID::BIOME_FROST;
		break;
	case BIOME_ID::BEACH:
		used_texture = TEXTURE_ASSET_ID::BIOME_BEACH;
		break;
	case BIOME_ID::LAVA:
		used_texture = TEXTURE_ASSET_ID::BIOME_LAVA;
		break;
	case BIOME_ID::JUNGLE:
		used_texture = TEXTURE_ASSET_ID::BIOME_JUNGLE;
		break;
	}

	registry.renderRequests.insert(
		entity,
		{ used_texture,
		  EFFECT_ASSET_ID::CARD,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity loadCard(RenderSystem* renderer, int current_health, int max_health, int damage_output)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = { 0.f - CARD_BB_WIDTH, 0.f - CARD_BB_HEIGHT };

	// Setting initial values
	motion.scale = vec2({ CARD_BB_WIDTH, CARD_BB_HEIGHT });

	// Create an (empty) Card component to be able to refer to all cards
	registry.playerDecks.emplace(entity);

	auto& health = registry.healthComponents.emplace(entity);
	health.current_health = current_health;
	health.max_health = max_health;

	auto& damage = registry.damageComponents.emplace(entity);
	damage.damage_output = damage_output;

	return entity;
}

Entity createModeUI(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ BOARD_MODE_BB_WIDTH, BOARD_MODE_BB_HEIGHT });

	// Create an (empty) ModeUI component to be able to refer to modes
	registry.boardModes.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BOARD_SELECT,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createBarUI(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
		  EFFECT_ASSET_ID::EGG,
		  GEOMETRY_BUFFER_ID::DEBUG_LINE });

	// Create motion
	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	return entity;
}

Entity createPath(RenderSystem* renderer, vec2 position, BIOME_ID used_biome)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ TILE_BB_WIDTH, TILE_BB_HEIGHT });

	// Create an (empty) Path component to be able to refer to all tiles representing paths
	registry.traversables.emplace(entity);

	// Set texture to be used
	TEXTURE_ASSET_ID used_texture;
	switch (used_biome)
	{
	default:
	case BIOME_ID::HUB:
		used_texture = TEXTURE_ASSET_ID::BIOME_HUB_PATH;
		break;
	case BIOME_ID::FROST:
		used_texture = TEXTURE_ASSET_ID::BIOME_FROST_PATH;
		break;
	case BIOME_ID::BEACH:
		used_texture = TEXTURE_ASSET_ID::BIOME_BEACH_PATH;
		break;
	case BIOME_ID::LAVA:
		used_texture = TEXTURE_ASSET_ID::BIOME_LAVA_PATH;
		break;
	case BIOME_ID::JUNGLE:
		used_texture = TEXTURE_ASSET_ID::BIOME_JUNGLE_PATH;
		break;
	}

	registry.renderRequests.insert(
		entity,
		{ used_texture,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createWall(RenderSystem* renderer, vec2 position, BIOME_ID used_biome)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ TILE_BB_WIDTH, TILE_BB_HEIGHT });

	// Set texture to be used
	TEXTURE_ASSET_ID used_texture;
	switch (used_biome)
	{
	default:
	case BIOME_ID::HUB:
		used_texture = TEXTURE_ASSET_ID::BIOME_HUB_WALL;
		break;
	case BIOME_ID::FROST:
		used_texture = TEXTURE_ASSET_ID::BIOME_FROST_WALL;
		break;
	case BIOME_ID::BEACH:
		used_texture = TEXTURE_ASSET_ID::BIOME_BEACH_WALL;
		break;
	case BIOME_ID::LAVA:
		used_texture = TEXTURE_ASSET_ID::BIOME_LAVA_WALL;
		break;
	case BIOME_ID::JUNGLE:
		used_texture = TEXTURE_ASSET_ID::BIOME_JUNGLE_WALL;
		break;
	}

	registry.renderRequests.insert(
		entity,
		{ used_texture,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createTree(RenderSystem* renderer, vec2 position, TEXTURE_ASSET_ID used_texture)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ TREE_BB_WIDTH, TREE_BB_HEIGHT });

	// Initialize the texture
	registry.renderRequests.insert(
		entity,
		{ used_texture,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createTimer(RenderSystem* renderer, vec2 position, int currentFrame, float updateTime)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ 50, 50 });

	// Create an (empty) Timer component to be able to refer to all timers
	registry.traversables.emplace(entity);

	Animation& animation = registry.animations.emplace(entity);
	animation.num_frames = 10;
	animation.current_frame = currentFrame;

	Timer& timer = registry.timer.emplace(entity);
	timer.updateTime = updateTime;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TIMER,
		  EFFECT_ASSET_ID::ANIMATED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createHealth(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ ITEM_BB_WIDTH, ITEM_BB_HEIGHT });

	// Create an (empty) Health component to be able to refer to items that increase health
	registry.interactables.emplace(entity);

	Health& health = registry.healthComponents.emplace(entity);
	health.current_health = 5;

	Animation& animation = registry.animations.emplace(entity);
	animation.num_frames = ITEM_HEALTH_NUM_FRAMES;

	// Initialize the texture
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ITEM_HEALTH,
		  EFFECT_ASSET_ID::ANIMATED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createDamage(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ ITEM_BB_WIDTH, ITEM_BB_HEIGHT });

	// Create an (empty) Damage component to be able to refer to items that increase damage
	registry.interactables.emplace(entity);

	Damage& damage = registry.damageComponents.emplace(entity);
	damage.damage_output = 5;

	Animation& animation = registry.animations.emplace(entity);
	animation.num_frames = ITEM_DAMAGE_NUM_FRAMES;

	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ITEM_DAMAGE,
		  EFFECT_ASSET_ID::ANIMATED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createBiome(RenderSystem* renderer, vec2 position, BIOME_ID used_biome)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// Setting initial values
	motion.scale = vec2({ TILE_BB_WIDTH, TILE_BB_HEIGHT });

	// Create an (empty) Biome component to be able to refer to all biomes
	auto& biome = registry.biomes.emplace(entity);
	biome.used_biome = used_biome;

	// Set texture to be used
	TEXTURE_ASSET_ID used_texture;
	switch (used_biome)
	{
	default:
	case BIOME_ID::HUB:
		used_texture = TEXTURE_ASSET_ID::BIOME_HUB;
		break;
	case BIOME_ID::FROST:
		used_texture = TEXTURE_ASSET_ID::BIOME_FROST;
		break;
	case BIOME_ID::BEACH:
		used_texture = TEXTURE_ASSET_ID::BIOME_BEACH;
		break;
	case BIOME_ID::LAVA:
		used_texture = TEXTURE_ASSET_ID::BIOME_LAVA;
		break;
	case BIOME_ID::JUNGLE:
		used_texture = TEXTURE_ASSET_ID::BIOME_JUNGLE;
		break;
	}

	registry.renderRequests.insert(
		entity,
		{ used_texture,
		  EFFECT_ASSET_ID::TEXTURED,
		  GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createLine(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
		  EFFECT_ASSET_ID::EGG,
		  GEOMETRY_BUFFER_ID::DEBUG_LINE });

	// Create motion
	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	registry.debugComponents.emplace(entity);
	return entity;
}