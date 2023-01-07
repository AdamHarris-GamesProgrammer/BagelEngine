#type vertex

#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextureCoordinate;

uniform mat4 u_ViewProjection;
			
out vec2 v_TexCoord;
out vec4 v_Color;
	
void main() {
	v_TexCoord = a_TextureCoordinate;
	v_Color = a_Color;
	gl_Position = u_ViewProjection  * vec4(a_Position, 1.0);
}

#type fragment

#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;

layout(location = 0) out vec4 color;


uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float u_TextureScaling;

void main() {
	color = v_Color;
}