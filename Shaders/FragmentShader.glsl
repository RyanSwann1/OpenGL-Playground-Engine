#version 330 core

out vec4 color;

in vec2 vTextCoords;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

uniform bool diffuseTexture;
uniform bool specularTexture;

void main()
{
	if(diffuseTexture)
	{
		color = texture(texture_diffuse, vTextCoords);
	}

	if(specularTexture)
	{
		color = texture(texture_specular, vTextCoords);
	}
};