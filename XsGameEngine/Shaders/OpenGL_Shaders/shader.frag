#version 460

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 TexCoords;
//layout(location = 2) in vec3 Normal;
//layout(location = 3) in vec3 FragPos;

layout(location = 0) out vec4 outColor; //Final output Color   


layout(binding = 0) uniform sampler2D textureSampler;

//layout(set = 0, binding = 2) uniform ExtraSettings
//{
//	vec3 eyePosition;
//}extras;
//layout(binding = 2) uniform eyePosition;
/*
struct Light
{
	vec3 colour;
	float ambientIntensity;
	float diffuseIntensity;
};


struct DirectionalLightInfo
{
	Light base;
	vec3 lightDirection;
};

const int MAX_DIRECTIONAL_LIGHTS = 3;

// --- ALL LIGHTS
layout(set = 0, binding = 1) uniform AllLights
{
	DirectionalLightInfo directionalLights[MAX_DIRECTIONAL_LIGHTS];
}allLights;


// --CALCULATE DIR
vec4 CalcLightByDirection(vec3 colour, float ambientIntensity ,float diffuseIntensity , vec3 lightDirection)
{
	vec4 ambientColour = vec4(colour.x* ambientIntensity, colour.y * ambientIntensity, colour.z* ambientIntensity , 1.0f) ;

	float diffuseFactor = max(dot(normalize(Normal), normalize(lightDirection)), 0.0f);
	diffuseFactor *= diffuseIntensity;
	vec4 diffuseColour = vec4(colour * diffuseFactor , 1.0f);
	

	return (ambientColour + diffuseColour);// + diffuseColour );
}

vec4 CalcDirectionalLight()
{
	vec4 finalDirectionalColour = vec4(0.0f, 0.0f,0.0f,1.0f);
	for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++){
		if(allLights.directionalLights[i].base.diffuseIntensity > 0.0f) {
			finalDirectionalColour += CalcLightByDirection(allLights.directionalLights[i].base.colour,allLights.directionalLights[i].base.ambientIntensity, allLights.directionalLights[i].base.diffuseIntensity, allLights.directionalLights[i].lightDirection );
		}
	}
	return finalDirectionalColour;
}

*/

void main(){
	
	//vec4 finalColour = CalcDirectionalLight();

	//vec4 ambientColour = vec4(allLights.directionalLights[0].colour , 1.0f) * allLights.directionalLights[0].ambientIntensity;
	//vec4 ambientColour2 = vec4(allLights.nLights[1].colour, 1.0f) * allLights.nLights[1].ambientIntensity;

	//ambientColour += ambientColour2;
	
	//outColor = texture(textureSampler , TexCoords) * finalColour ;
	outColor = texture(textureSampler , TexCoords);
	//outColor = vec4(fragColor, 1.0);
}