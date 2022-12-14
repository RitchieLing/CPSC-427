#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "components.hpp"
#include "tiny_ecs_registry.hpp"

// A simple physics system that moves rigid bodies and checks for collision
class PhysicsSystem
{
public:
	void step(float elapsed_ms);
	static bool collides(const Motion& motion1, const Motion& motion2);
private:
	//modifies the speed at which the player follows the path.
	static const float PATH_SPEED_MODIFIER; 
};