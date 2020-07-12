#version 330 core

layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec3 normal;

uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vFragPosition;
out vec3 vNormal;

void main()
{
	gl_Position = uProjection * uView * vec4(aPos, 1.0);
	vFragPosition = aPos;
	vNormal = normal;
}