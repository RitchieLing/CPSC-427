// internal
#include "physics_system.hpp"
#include "world_init.hpp"
#include "world_system.hpp"

const float PhysicsSystem::PATH_SPEED_MODIFIER = 30.f;

// Returns the local bounding coordinates scaled by the current size of the entity
vec2 get_bounding_box(const Motion& motion)
{
	// abs is to avoid negative scale due to the facing direction.
	return { abs(motion.scale.x), abs(motion.scale.y) };
}

// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You can
// surely implement a more accurate detection
bool PhysicsSystem::collides(const Motion& motion1, const Motion& motion2)
{
	vec2 dp = motion1.position - motion2.position;
	float dist_squared = dot(dp,dp);
	const vec2 other_bonding_box = get_bounding_box(motion1) / 2.f;
	const float other_r_squared = dot(other_bonding_box, other_bonding_box);
	const vec2 my_bonding_box = get_bounding_box(motion2) / 2.f;
	const float my_r_squared = dot(my_bonding_box, my_bonding_box);
	const float r_squared = max(other_r_squared, my_r_squared);
	if (dist_squared < r_squared)
		return true;
	return false;
}

// Current enemy movement direction
void PhysicsSystem::step(float elapsed_ms)
{
	// Elapsed time in seconds
	float step_seconds = elapsed_ms / 1000.f;

	// Moving entities
	auto& motion_registry = registry.motions;

	// Move enemies along a vertical path and based on how much time has passed
	assert(registry.screenStates.components.size() <= 1);
	ScreenState& state = registry.screenStates.components[0];

	if (state.used_state == STATE_ID::IDLE)
	{
		auto& deadly_container = registry.subBosses;
		for (uint i = 0; i < deadly_container.components.size(); i++)
		{
			Motion& enemy_motion = motion_registry.get(deadly_container.entities[i]);
			enemy_motion.position.y += enemy_motion.velocity.y * step_seconds;

			// Handle collision between enemies and the game window
			const vec2 enemy_bounding_box = get_bounding_box(enemy_motion);
			float enemy_radius = sqrt(dot(enemy_bounding_box / 2.f, enemy_bounding_box / 2.f));

			if (enemy_motion.position.y - enemy_radius < 0.f)
			{
				enemy_motion.position.y = 0.f + enemy_radius;
				enemy_motion.velocity.y = -enemy_motion.velocity.y;
			}
			if (enemy_motion.position.y + enemy_radius > window_height_px)
			{
				enemy_motion.position.y = window_height_px - enemy_radius;
				enemy_motion.velocity.y = -enemy_motion.velocity.y;
			}
		}
	}

	// Move character according to its velocity and based on how much time has passed
	Motion& player_motion = motion_registry.get(registry.players.entities.back());
	player_motion.position += player_motion.velocity * step_seconds;
	
	if (player_motion.is_enroute)
	{
		vec2 direction_to_next = player_motion.path->next - player_motion.position;
		double distance_to_next = WorldSystem::calc_heuristic(player_motion.path->next,player_motion.position);
		
		if (distance_to_next <= WorldSystem::GRANULARITY)
		{
			vec2 node = player_motion.path->next;
			if (player_motion.path->path_stack.empty())
			{
				player_motion.is_enroute = false;
				direction_to_next = { 0,0 };
			}
			else 
			{
				player_motion.path->next = player_motion.path->path_stack.top();
				player_motion.path->path_stack.pop();
				direction_to_next = player_motion.path->next - player_motion.position;
			}		
		}
		player_motion.velocity = PATH_SPEED_MODIFIER * direction_to_next;
	} 

	// Handle collision between the player and the game window
	const vec2 player_bounding_box = get_bounding_box(player_motion);
	float player_radius = sqrt(dot(player_bounding_box / 2.f, player_bounding_box / 2.f));

	if (player_motion.position.x - player_radius < 0.f)
	{
		player_motion.position.x = 0.f + player_radius;
		player_motion.velocity.x = 0.f;
	}
	if (player_motion.position.x + player_radius > window_width_px)
	{
		player_motion.position.x = window_width_px - player_radius;
		player_motion.velocity.x = 0.f;
	}
	if (player_motion.position.y - player_radius < 0.f)
	{
		player_motion.position.y = 0.f + player_radius;
		player_motion.velocity.y = 0.f;
	}
	if (player_motion.position.y + player_radius > window_height_px)
	{
		player_motion.position.y = window_height_px - player_radius;
		player_motion.velocity.y = 0.f;
	}

	// Check for collisions between all moving entities
    ComponentContainer<Motion> &motion_container = registry.motions;
	for (uint i = 0; i < motion_container.components.size(); i++)
	{
		Entity entity_i = motion_container.entities[i];
		// Skip entities that are traversable
		if (registry.traversables.has(entity_i))
			continue;

		Motion& motion_i = motion_container.components[i];
		// note starting j at i+1 to compare all (i,j) pairs only once (and to not compare with itself)
		for (uint j = i+1; j < motion_container.components.size(); j++)
		{
			Entity entity_j = motion_container.entities[j];
			// Skip entities that are traversable
			if (registry.traversables.has(entity_j))
				continue;

			Motion& motion_j = motion_container.components[j];
			if (collides(motion_i, motion_j))
			{
				// Create a collisions event
				// We are abusing the ECS system a bit in that we potentially insert muliple collisions for the same entity
				registry.collisions.emplace_with_duplicates(entity_i, entity_j);
				registry.collisions.emplace_with_duplicates(entity_j, entity_i);
			}
		}
	}

	// debugging of bounding boxes
	if (debugging.in_debug_mode)
	{
		uint size_before_adding_new = (uint)motion_container.components.size();
		for (uint i = 0; i < size_before_adding_new; i++)
		{
			Motion& motion_i = motion_container.components[i];
			Entity entity_i = motion_container.entities[i];

			// don't draw debugging visuals around debug lines
			if (registry.debugComponents.has(entity_i))
				continue;

			// visualize the radius with two axis-aligned lines
			const vec2 bonding_box = get_bounding_box(motion_i);
			float radius = sqrt(dot(bonding_box/2.f, bonding_box/2.f));
			vec2 line_scale1 = { motion_i.scale.x / 10, 2*radius };
			vec2 line_scale2 = { 2*radius, motion_i.scale.x / 10};
			vec2 position = motion_i.position;
			Entity line1 = createLine(motion_i.position, line_scale1);
			Entity line2 = createLine(motion_i.position, line_scale2);
		}
	}
}