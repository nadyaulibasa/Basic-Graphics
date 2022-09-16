#version 330 core

uniform mat4 u_transform;

layout (location = 0) in vec4 pos;
layout (location = 1) in vec2 tc_in;

out vec2 tc;

void main() {
    gl_Position = u_transform * pos;
    tc = tc_in;
}