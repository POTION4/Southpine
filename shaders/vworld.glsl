#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

out vec3 pos;
out vec3 normal;
out vec3 vColor;


uniform mat4 model;
layout (std140) uniform Matrices {
    mat4 view;
    mat4 perspec;
};



void main() {
    gl_Position = perspec * view * model * vec4(aPos, 1.0);
    pos = vec3(model * vec4(aPos, 1.0));
    normal = vec3(model * vec4(aNormal, 1.0));
    vColor = aColor;
}

