// internal
#include "ai_system.hpp"
#include "battle_system.hpp"

AISystem::AISystem()
{
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());
}

void AISystem::init(RenderSystem* renderer_arg)
{
	this->renderer = renderer_arg;
}

void AISystem::step(float elapsed_ms)
{
	auto& battle_registry = registry.battles;
	for (uint i = 0; i < battle_registry.components.size(); i++)
	{
		auto& timer = registry.bossModeTimers;
		Battle& battle = battle_registry.components[i];
		Entity entity = battle_registry.entities[i];

		Entity entity_other = registry.players.entities.back();
		Player& player = registry.players.get(entity_other);

		if (registry.bosses.has(entity) && battle.is_my_turn)
		{
			if (!timer.has(entity) && battle.current_mode == MODE_ID::SELECT)
			{
				// Skip if cards at play have reached the maximum
				if (registry.bossPlays.components.size() < MAX_PLAY)
				{
					std::uniform_int_distribution<int> int_dist(1, MAX_HEALTH_DAMAGE);
					selected_card = createCard(renderer, { 0, 0 }, player.current_biome,
						int_dist(rng), MAX_HEALTH_DAMAGE, int_dist(rng));

					registry.renderRequests.remove(selected_card);
					registry.bossHands.emplace(selected_card);
				}

				// Start a timer to simulate the opponent "thinking"
				registry.bossModeTimers.emplace(entity);
				battle.current_mode = MODE_ID::PLACE;
			}
			else if (!timer.has(entity) && battle.current_mode == MODE_ID::PLACE)
			{
				// Skip if cards at play have reached the maximum
				if (registry.bossPlays.components.size() < MAX_PLAY)
				{
					auto& motion_registry = registry.motions;
					auto& board_boss_registry = registry.boardBosses;
					std::uniform_int_distribution<int> placement_dist(0, board_boss_registry.entities.size() - 1);

					bool unplaced = true;
					while (unplaced)
					{
						Entity card_placement = board_boss_registry.entities[placement_dist(rng)];
						Motion& motion = motion_registry.get(card_placement);

						// Remove the card from hand, put it in play, and switch to card selection mode
						if (card_placement_unoccupied(motion.position, registry.bossPlays)) {
							motion_registry.get(selected_card).position = motion.position;
							registry.bossHands.remove(selected_card);
							registry.bossPlays.emplace(selected_card);

							// Set texture to be used
							TEXTURE_ASSET_ID used_texture;
							switch (player.current_biome)
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
								selected_card,
								{ used_texture,
								  EFFECT_ASSET_ID::CARD,
								  GEOMETRY_BUFFER_ID::SPRITE });

							unplaced = false;
						}
					}
				}

				// Start a timer to simulate the opponent "thinking"
				registry.bossModeTimers.emplace(entity);
				battle.current_mode = MODE_ID::PLAY;
			}
			else if (!timer.has(entity) && battle.current_mode == MODE_ID::PLAY)
			{
				// Card is removed from play if its health reaches 0 (handled in `WorldSystem::step`)
				play_cards(registry.bossPlays, registry.playerPlays, registry.boardPlayerBars, entity_other);
				battle.current_mode = MODE_ID::DONE;
			}
			else if (!timer.has(entity) && battle.current_mode == MODE_ID::DONE)
			{
				// If opponent's turn is over, switch over to the player
				battle.is_my_turn = false;
				battle.current_mode = MODE_ID::SELECT;
				battle_registry.get(entity_other).is_my_turn = true;
				registry.renderRequests.get(
					registry.boardModes.entities.back()).used_texture = TEXTURE_ASSET_ID::BOARD_SELECT;				

				// If player's health has dropped to 0, restart the game (handled in `WorldSystem::step`)
				assert(registry.screenStates.components.size() <= 1);
				ScreenState& state = registry.screenStates.components[0];
				Health& health = registry.healthComponents.get(entity_other);
				if (health.current_health <= 0) {
					registry.screenTimers.emplace(entity);
					state.used_state = STATE_ID::IDLE;
					state.used_screen = SCREEN_ID::MENU;
				}
			}
		}
	}

	// Processing the card battle mode state
	for (Entity entity : registry.bossModeTimers.entities) {
		// Progress timer
		BossModeTimer& counter = registry.bossModeTimers.get(entity);
		counter.counter_ms -= elapsed_ms;

		if (counter.counter_ms < 0)
			registry.bossModeTimers.remove(entity);
	}
}