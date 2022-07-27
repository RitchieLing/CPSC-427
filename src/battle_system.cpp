// internal
#include "common.hpp"

#include "battle_system.hpp"

// Retrieve and re-render a card from player's deck
Entity& get_card_player_deck() {
	Entity& card = registry.playerDecks.entities.back();
	Player& player = registry.players.get(registry.players.entities.back());

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
		card,
		{ used_texture,
		  EFFECT_ASSET_ID::CARD,
		  GEOMETRY_BUFFER_ID::SPRITE });

	// Add card to current hand and remove it from the deck
	registry.playerDecks.remove(card);
	registry.playerHands.emplace(card);

	return card;
}

// Return cards that were in player's hand to the player's deck
void return_cards_player_deck() {
	auto& hand_registry = registry.playerHands;
	for (Entity entity : hand_registry.entities)
		registry.playerDecks.emplace(entity);

	while (hand_registry.entities.size() > 0)
		hand_registry.remove(hand_registry.entities.back());
}

// Is there a card placed in the selected card placement?
bool card_placement_unoccupied(vec2 position, ComponentContainer<Play>& play_registry) {
	bool unoccupied = true;

	auto& motion_registry = registry.motions;
	for (int i = (int)play_registry.components.size() - 1; i >= 0; i--) {
		Motion& motion = motion_registry.get(play_registry.entities[i]);

		// Cannot place card here if there is already one present
		if (motion.position.x == position.x) {
			unoccupied = false;
			break;
		}
	}

	return unoccupied;
}

// Determine an unoccupied position in player's hand
vec2 choose_placement_player_hand() {
	bool unoccupied = true;
	vec2 placement = { 0, 0 };

	auto& hand_registry = registry.playerHands;
	auto& motion_registry = registry.motions;
	for (int i = 0; i < MAX_HAND; i++) {
		for (int j = (int)hand_registry.components.size() - 1; j >= 0; j--) {
			Motion& motion = motion_registry.get(hand_registry.entities[j]);
			placement = { (i + 0.5) * window_width_px / MAX_PLAY, CARD_PLAYER_HAND_HEIGHT };

			// Cannot place new card here if there is already one present
			if (motion.position.x == placement.x) {
				unoccupied = false;
				break; // break out of inner loop
			}
		}

		if (unoccupied)
			break; // break out of outer loop
		unoccupied = true;
	}

	return placement;
}

// Attack with cards currently at play
void play_cards(ComponentContainer<Play>& player_play_registry, 
	ComponentContainer<Play>& opponent_play_registry, 
	ComponentContainer<BoardUI>& opponent_bar, Entity& opponent) {
	auto& motion_registry = registry.motions;
	for (int i = (int)player_play_registry.components.size() - 1; i >= 0; i--) {
		Entity card = player_play_registry.entities[i];
		Motion& motion = motion_registry.get(card);
		Damage& damage = registry.damageComponents.get(card);

		// Reduce enemy's health by the damage output of each card at play
		Health& health_other = registry.healthComponents.get(opponent);
		health_other.current_health -= damage.damage_output;

		// Decrease and re-position the opponent's health bar
		Entity bar = opponent_bar.entities.back();
		Motion& motion_bar = motion_registry.get(bar);
		motion_bar.scale.x -= damage.damage_output * (window_width_px / health_other.max_health);
		motion_bar.position.x = motion_bar.scale.x / 2.f;

		// For every enemy card at play...
		for (int j = (int)opponent_play_registry.components.size() - 1; j >= 0; j--) {
			Entity card_other = opponent_play_registry.entities[j];
			Motion& motion_other = registry.motions.get(card_other);

			// ...reduce enemy card's health if it is in front of player's card
			if (motion.position.x == motion_other.position.x) {
				Health& health_card = registry.healthComponents.get(card_other);
				health_card.current_health = max(health_card.current_health - damage.damage_output, 0.f);
				break; // break as only one card can be in the same column
			}
		}
	}
}