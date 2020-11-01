#version 330 core

layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vfragPosition;
out vec3 vNormal;
out vec2 vTextCoords;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
	vfragPosition = vec3(uModel * vec4(aPos, 1.0));
	vNormal = aNormal;
	vTextCoords = aTextCoords;
}