#pragma once

// stlib
#include <vector>
#include <random>

#include "common.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"

class AISystem
{
public:
	AISystem();

	void init(RenderSystem* renderer);

	void step(float elapsed_ms);
private:
	// Game state
	RenderSystem* renderer;

	// Card battle references
	Entity selected_card;

	// C++ random number generator
	std::default_random_engine rng;
};