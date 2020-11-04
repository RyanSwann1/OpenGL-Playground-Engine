#version 330 core

layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vFragPosition;
out vec3 vNormal;
out vec2 vTextCoords;
out vec3 vLightPosition;
out vec3 vDirectionalLightDirection;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
	vFragPosition = vec3(uView * uModel * vec4(aPos, 1.0));
	vNormal = mat3(transpose(inverse(uView * uModel))) * aNormal;
	vDirectionalLightDirection = mat3(transpose(inverse(uView))) * vec3(-0.14, 0.54, 0.0);
	vTextCoords = aTextCoords;
}