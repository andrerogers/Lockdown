#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 camMatrix;
uniform vec3 translate;

void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * model * vec4(aPos + translate, 1.0f);
}