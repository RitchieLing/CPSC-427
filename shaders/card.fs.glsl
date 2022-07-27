#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform sampler2D sampler3;
uniform int current_health;
uniform int max_health_damage;
uniform int current_damage;
uniform vec3 fcolor;

// Output color
layout(location = 0) out vec4 color;

void main()
{
	// Use hard coded scale and position, accounting for card frame texture
	vec4 biome = texture(sampler0, texcoord * vec2(3, 3.78) - vec2(1.0, 0.25));
	vec4 card = texture(sampler1, texcoord);

	// Apply over operator using premultiplied alpha
	// https://en.wikipedia.org/wiki/Alpha_compositing
	card.rgb *= card.a;
	biome.rgb *= biome.a;

	vec3 rgb_adder = biome.rgb;
	float alpha_adder = biome.a;
	vec3 rgb_multiplier = card.rgb * (1 - biome.a);
	float alpha_multiplier = card.a * (1 - biome.a);

	// Render texture per health
	vec4 health;
	for (int i = 0; i < current_health; i++)
	{
		// Use hard coded scale and position, accounting for card frame texture
		vec2 scale = vec2(max_health_damage + 1.0, max_health_damage + 2.5);
		vec2 position = vec2(i + 0.5, max_health_damage);
		
		health = texture(sampler2, texcoord * scale - position);
		
		health.rgb *= health.a;
		rgb_adder += health.rgb;
		alpha_adder += health.a;
		rgb_multiplier *= (1 - health.a);
		alpha_multiplier *= (1 - health.a);
	}

	// Render texture per damage
	vec4 damage;
	for (int i = 0; i < current_damage; i++)
	{
		// Use hard coded scale and position, accounting for card frame texture
		vec2 scale = vec2(max_health_damage + 1.0, max_health_damage + 2.5);
		vec2 position = vec2(i + 0.5, max_health_damage + 1);
		
		damage = texture(sampler3, texcoord * scale - position);
		
		damage.rgb *= damage.a;
		rgb_adder += damage.rgb;
		alpha_adder += damage.a;
		rgb_multiplier *= (1 - damage.a);
		alpha_multiplier *= (1 - damage.a);
	}

	// Final color is addition of all textures
	// Card frame color is only shown when alpha channel of other textures are 0
	color.rgb = rgb_adder + rgb_multiplier;
	color.a = alpha_adder + alpha_multiplier;
}
