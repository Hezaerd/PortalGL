#version 330 core

in vec3 ourColor;

out vec4 Collr;

void main()
{
	Collr = vec4(ourColor, 1.0f);
}

