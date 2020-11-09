#version 330 core

#define POINT_LIGHT_COUNT 3

struct Light
{
	vec4 position;
	vec4 color;
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
uniform	Light uPointLights[POINT_LIGHT_COUNT];
uniform vec3 uDirectionalLightColor;
uniform float uDirectionalLightIntensity;

const float ambientStrength = 0.2;
const float specularStrength = 0.5;

vec3 calculateDirectionalLight(vec3 n)
{
	vec3 nDirectionalLight = normalize(vDirectionalLightDirection);

	vec3 diffuse = texture(texture_diffuse, vTextCoords).rgb * 
		max(dot(nDirectionalLight, n), 0.0) * uDirectionalLightColor * uDirectionalLightIntensity;

	vec3 specular = texture(texture_specular, vTextCoords).rgb * uDirectionalLightColor * uDirectionalLightIntensity * specularStrength * 
		pow(max(dot(normalize(-vFragPosition), normalize(-reflect(nDirectionalLight, n))), 0.0), 64);
	
	return diffuse + specular;
}

vec3 calculatePointLight(vec3 n, Light light)
{
	vec3 lightDirection = normalize(light.position.xyz - vFragPosition);

	vec3 diffuse = texture(texture_diffuse, vTextCoords).rgb * max(dot(lightDirection, n), 0.0) * light.color.xyz;	
	
	vec3 specular = texture(texture_specular, vTextCoords).rgb * light.color.xyz * specularStrength * 
		pow(max(dot(normalize(-vFragPosition), normalize(reflect(-lightDirection, n))), 0.0), 64);

	float attenuation = pow(smoothstep(light.radius, 0, length(light.position.xyz - vFragPosition)), light.compression);
	
	return diffuse * attenuation + specular * attenuation;
}

void main()
{
	vec3 n = normalize(vNormal);	

	float diffuseAlpha = texture(texture_diffuse, vTextCoords).a;
	vec3 ambient = texture(texture_diffuse, vTextCoords).rgb * ambientStrength;
	
	vec3 pointLightingResult = vec3(0.0);
	for(int i = 0; i < POINT_LIGHT_COUNT; ++i)
	{
		pointLightingResult += calculatePointLight(n, uPointLights[i]);
	}

	color = vec4(vec3(pointLightingResult + calculateDirectionalLight(n) + ambient), diffuseAlpha);	
	if(color.a < 0.1)
	{
		discard;
	}
};