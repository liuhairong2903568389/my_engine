#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_pos;
out vec2 texture_Pos;

void main()
{
	gl_Position = vec4(position, 1.0);
	texture_Pos = texture_pos;
};

#shader fragment
#version 330 core
in vec2 texture_Pos;
out vec4 color;


uniform float T_color;
uniform sampler2D mytexture1;
uniform sampler2D mytexture2;
void main()
{
	color = mix(texture(mytexture1,texture_Pos),texture(mytexture2,texture_Pos),1.0);
};