#version 430 core

out vec4 frag_color;


in vec2 t_uvs;

uniform sampler2D img;

void main() {
	frag_color = texture(img, t_uvs);
}