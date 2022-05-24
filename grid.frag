#version 330 core

in vec4 ourColor;
out vec4 color;


void main()
{
    if(ourColor.a < 0.1)
        discard;

    color = ourColor;
}