#version 430 core

layout (location = 0) in vec2 vertex_pos;

out vec2 t_uvs;

const vec2 uvs[4] = {vec2(1, 1), vec2(1, 0), vec2(0, 0), vec2(0, 1)};

void main() {
	 gl_Position = vec4(vertex_pos, 0.0, 1.0);
	 t_uvs = uvs[gl_VertexID];
}



