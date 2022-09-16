#version 330 core

uniform sampler2D u_tex;

in vec2 tc;

out vec4 fs_col;

void main() {
    fs_col = texture(u_tex, tc);
}