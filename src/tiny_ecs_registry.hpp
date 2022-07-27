#pragma once
#include <vector>

#include "tiny_ecs.hpp"
#include "components.hpp"

class ECSRegistry
{
	// Callbacks to remove a particular or all entities in the system
	std::vector<ContainerInterface*> registry_list;

public:
	// Manually created list of all components this game has
	ComponentContainer<Player> players;
	ComponentContainer<Npc> npcs;
	ComponentContainer<Enemy> subBosses;
	ComponentContainer<Enemy> bosses;
	ComponentContainer<Biome> biomes;
	ComponentContainer<Hand> playerHands;
	ComponentContainer<Hand> bossHands;
	ComponentContainer<Deck> playerDecks;
	ComponentContainer<Deck> bossDecks;
	ComponentContainer<Play> playerPlays;
	ComponentContainer<Play> bossPlays;
	ComponentContainer<Generator> generators;
	ComponentContainer<BoardUI> boardPlayers;
	ComponentContainer<BoardUI> boardBosses;
	ComponentContainer<BoardUI> boardModes;
	ComponentContainer<BoardUI> boardPlayerBars;
	ComponentContainer<BoardUI> boardOpponentBars;
	ComponentContainer<MenuUI> menuStarts;
	ComponentContainer<MenuUI> menuContinues;
	ComponentContainer<MenuUI> menuInstructions;
	ComponentContainer<InstructionsUI> instructionsClose;
	ComponentContainer<StoryUI> Close;
	ComponentContainer<Health> healthComponents;
	ComponentContainer<Damage> damageComponents;
	ComponentContainer<Timer> timer;
	ComponentContainer<Interactable> interactables;
	ComponentContainer<Traversable> traversables;
	ComponentContainer<Lootable> lootables;
	ComponentContainer<Tier> tiers;
	ComponentContainer<Motion> motions;
	ComponentContainer<Battle> battles;
	ComponentContainer<Collision> collisions;
	ComponentContainer<Animation> animations;
	ComponentContainer<Progression> progressions;
	ComponentContainer<DebugComponent> debugComponents;
	ComponentContainer<ScreenState> screenStates;
	ComponentContainer<ScreenTimer> screenTimers;
	ComponentContainer<CardAppearTimer> cardAppearTimers;
	ComponentContainer<BossModeTimer> bossModeTimers;
	ComponentContainer<Mesh*> meshPtrs;
	ComponentContainer<RenderRequest> renderRequests;
	ComponentContainer<vec3> colors;

	// constructor that adds all containers for looping over them
	// IMPORTANT: Don't forget to add any newly added containers!
	ECSRegistry()
	{	
		registry_list.push_back(&players);
		registry_list.push_back(&npcs);
		registry_list.push_back(&subBosses);
		registry_list.push_back(&bosses);
		registry_list.push_back(&biomes);
		registry_list.push_back(&playerHands);
		registry_list.push_back(&bossHands);
		registry_list.push_back(&playerDecks);
		registry_list.push_back(&bossDecks);
		registry_list.push_back(&playerPlays);
		registry_list.push_back(&bossPlays);
		registry_list.push_back(&generators);
		registry_list.push_back(&boardPlayers);
		registry_list.push_back(&boardBosses);
		registry_list.push_back(&boardModes);
		registry_list.push_back(&boardPlayerBars);
		registry_list.push_back(&boardOpponentBars);
		registry_list.push_back(&menuStarts);
		registry_list.push_back(&menuContinues);
		registry_list.push_back(&menuInstructions);
		registry_list.push_back(&instructionsClose);
		registry_list.push_back(&Close);
		registry_list.push_back(&healthComponents);
		registry_list.push_back(&damageComponents);
		registry_list.push_back(&timer);
		registry_list.push_back(&interactables);
		registry_list.push_back(&traversables);
		registry_list.push_back(&lootables);
		registry_list.push_back(&tiers);
		registry_list.push_back(&motions);
		registry_list.push_back(&battles);
		registry_list.push_back(&collisions);
		registry_list.push_back(&progressions);
		registry_list.push_back(&animations);
		registry_list.push_back(&screenStates);
		registry_list.push_back(&debugComponents);
		registry_list.push_back(&screenTimers);
		registry_list.push_back(&cardAppearTimers);
		registry_list.push_back(&bossModeTimers);
		registry_list.push_back(&meshPtrs);
		registry_list.push_back(&renderRequests);
		registry_list.push_back(&colors);
	}

	void clear_all_components() {
		for (ContainerInterface* reg : registry_list)
			reg->clear();
	}

	void list_all_components() {
		printf("Debug info on all registry entries:\n");
		for (ContainerInterface* reg : registry_list)
			if (reg->size() > 0)
				printf("%4d components of type %s\n", (int)reg->size(), typeid(*reg).name());
	}

	void list_all_components_of(Entity e) {
		printf("Debug info on components of entity %u:\n", (unsigned int)e);
		for (ContainerInterface* reg : registry_list)
			if (reg->has(e))
				printf("type %s\n", typeid(*reg).name());
	}

	void remove_all_components_of(Entity e) {
		for (ContainerInterface* reg : registry_list)
			reg->remove(e);
	}
};

extern ECSRegistry registry;