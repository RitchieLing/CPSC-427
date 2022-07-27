#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform int current_frame;
uniform int num_frames;
uniform vec3 fcolor;

// Output color
layout(location = 0) out vec4 color;

void main()
{
	vec2 new_texcoord = texcoord;
	// Determine size of each frame relative to sprite sheet
	new_texcoord.x /= num_frames;
	// Slide texture coordinate to the next frame or sprite
	new_texcoord.x += current_frame * 1.0 / num_frames;
	
	color = vec4(fcolor, 1.0) * texture(sampler0, new_texcoord);
}
