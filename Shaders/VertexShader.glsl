#version 330 core

layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoords;

layout (std140) uniform uMatrices
{
	mat4 uProjection;
	mat4 uView;
};

uniform vec3 uDirectionalLight;
uniform vec3 uViewPosition;
uniform mat3 uModelMatrixNormal;
uniform mat4 uProjViewModel;
uniform mat4 uViewModel;

out vec3 vFragPosition;
out vec3 vNormal;
out vec2 vTextCoords;
out vec3 vLightPosition;
out vec3 vDirectionalLight;
out vec3 vViewDirection;

void main()
{
	gl_Position = uProjViewModel * vec4(aPos, 1.0);
	vFragPosition = vec3(uViewModel * vec4(aPos, 1.0));
	vViewDirection = uViewPosition;
	vNormal = uModelMatrixNormal * aNormal;
	vDirectionalLight = uDirectionalLight;
	vTextCoords = aTextCoords;
}