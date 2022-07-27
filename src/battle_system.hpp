#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "tiny_ecs_registry.hpp"

const int MAX_HAND = 3;
const int MAX_PLAY = 4;
const int MAX_HEALTH_DAMAGE = 5;

const int CARD_OPPONENT_PLAY_HEIGHT = 170;
const int CARD_PLAYER_PLAY_HEIGHT = 475;
const int CARD_PLAYER_HAND_HEIGHT = 756;
const int BOARD_OPPONENT_HEALTH_HEIGHT = 20;
const int BOARD_PLAYER_HEALTH_HEIGHT = 322;
const int BOARD_HEALTH_BB_HEIGHT = 25;
const int BOARD_MODE_HEIGHT = 920;

Entity& get_card_player_deck();
void return_cards_player_deck();
bool card_placement_unoccupied(vec2 position, ComponentContainer<Play>& play_registry);
vec2 choose_placement_player_hand();
void play_cards(ComponentContainer<Play>& player_play_registry, 
	ComponentContainer<Play>& opponent_play_registry, 
	ComponentContainer<BoardUI>& opponent_bar, Entity& opponent);