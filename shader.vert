#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;
// layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
// out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 0.0f, 1.0f);
    ourColor = color;
//     ourColor = vec3(1.0f, 1.0f, 1.0f);
//     TexCoord = texCoord;
}