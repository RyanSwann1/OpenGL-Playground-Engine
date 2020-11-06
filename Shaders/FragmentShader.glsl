#version 330 core

#define POINT_LIGHT_COUNT 3

struct Light
{
	vec3 position;
	vec3 color;
	float radius;
	float compression;
};

out vec4 color;

in vec3 vDirectionalLightDirection;
in vec3 vFragPosition;
in vec3 vNormal;
in vec2 vTextCoords;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform Light uPointLights[POINT_LIGHT_COUNT];

const float ambientStrength = 0.2;
const float specularStrength = 0.5;
const vec3 directionalLightColor = vec3(1.0, 1.0, 1.0);
const float directionalLightItensity = 0.3;

vec3 calculateDirectionalLight(vec3 n)
{
	vec3 nDirectionalLight = normalize(vDirectionalLightDirection);

	vec3 diffuse = texture(texture_diffuse, vTextCoords).rgb * 
		max(dot(nDirectionalLight, n), 0.0) * directionalLightColor * directionalLightItensity;

	vec3 specular = texture(texture_specular, vTextCoords).rgb * directionalLightColor * directionalLightItensity * specularStrength * 
	pow(max(dot(normalize(-vFragPosition), normalize(-reflect(nDirectionalLight, n))), 0.0), 64);
	
	return diffuse + specular;
}

vec3 calculatePointLight(vec3 n, Light light)
{
	vec3 lightDirection = normalize(light.position - vFragPosition);

	float distance = length(light.position - vFragPosition);
	float attenuation = pow(smoothstep(light.radius, 0, length(light.position - vFragPosition)), light.compression);

	vec3 diffuse = texture(texture_diffuse, vTextCoords).rgb * max(dot(lightDirection, n), 0.0) * light.color;	
	
	vec3 specular = texture(texture_specular, vTextCoords).rgb * light.color * specularStrength * 
	pow(max(dot(normalize(-vFragPosition), normalize(reflect(-lightDirection, n))), 0.0), 64);
	
	return diffuse * attenuation + specular * attenuation;
}

void main()
{
	vec3 n = normalize(vNormal);	

	float diffuseAlpha = texture(texture_diffuse, vTextCoords).a;
	vec3 ambient = texture(texture_diffuse, vTextCoords).rgb * ambientStrength;
	
	vec3 result = vec3(0.0);
	for(int i = 0; i < POINT_LIGHT_COUNT; ++i)
	{
		result += calculatePointLight(n, uPointLights[i]);
	}

	color = vec4(vec3(result + calculateDirectionalLight(n) + ambient), diffuseAlpha);	
	if(color.a < 0.1)
	{
		discard;
	}
};