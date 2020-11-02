#version 330 core

layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

uniform vec3 uLightPosition;

out vec3 vFragPosition;
out vec3 vNormal;
out vec2 vTextCoords;
out vec3 vLightPosition;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
	vFragPosition = vec3(uView * uModel * vec4(aPos, 1.0));
	vLightPosition = vec3(uView * vec4(uLightPosition, 1.0));
	vNormal = mat3(transpose(inverse(uView * uModel))) * aNormal;
	vTextCoords = aTextCoords;
}