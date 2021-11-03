#include "Shaders.h"
#include <iostream>

Shaders::Shaders()
{
	shaderProgram = 0;
	UBObuffer = 0;
	//ubo.uniformModel = 0;
	//ubo.uniformProjection = 0;
	uniformEyePosition = 0;

	//pointLightCount = 0;
	//spotLightCount = 0;

	//CompileShaders();
}




#pragma region Shaders
void Shaders::AddShader(GLuint& program, std::vector<char> ShaderCode/*const char* ShaderCode*/, GLenum shaderType) {
	uint32_t currShader = glCreateShader(shaderType);
	

	//const GLchar* Code[1];
	//Code[0] = ShaderCode.c_str();

	//GLint codeLength[1];
	//codeLength[0] = strlen(ShaderCode.c_str());

	glShaderBinary(1, &currShader, GL_SHADER_BINARY_FORMAT_SPIR_V, ShaderCode.data(), ShaderCode.size());
	glSpecializeShader(currShader, "main",0,nullptr,nullptr);	//This compiles the shaders

	//glShaderSource(currShader, 1, Code, codeLength);
	//glCompileShader(currShader);

	int32_t result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(currShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(currShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(program, currShader);

}

void Shaders::CompileShaders() {
	shaderProgram = glCreateProgram();

	if (!shaderProgram)
	{
		printf("ERROR creating shader program!\n");
		return;
	}
	//src/Renderer/OpenGL_Renderer/
	std::vector<char> vShader = readFile("Shaders/OpenGL_Shaders/vert.spv");
	std::vector<char> fShader = readFile("Shaders/OpenGL_Shaders/frag.spv");

	//vertexShader.append(vShader.data());	//Using Vectors instead of strings now
	//fragmentShader.append(fShader.data());

	AddShader(shaderProgram, vShader, GL_VERTEX_SHADER);
	AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
		printf("Error Validating program: '%s'\n", eLog);
		return;
	}

	//Works
	//ModelMat = glGetUniformLocation(shaderProgram, "model");
	//uniformView = glGetUniformLocation(shaderProgram, "view");
	//uniformProjection = glGetUniformLocation(shaderProgram, "projection");
	
	//Make a OpenGL Uniform Buffer Object
	glGenBuffers(1, &UBObuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, UBObuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ubo), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//Binding The Block to our UBObuffer
	GLuint Block = glGetUniformBlockIndex(shaderProgram, "uboVP");
	glUniformBlockBinding(shaderProgram, Block, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBObuffer);	//Bind The Binding point (binding = 0) to UBObuffer Object

	std::cout << "ubo sizzeee - " << sizeof(ubo) << std::endl;

	//MVP.uniformModel = glGetUniformLocation(shaderProgram, "model");
	//MVP.uniformView = glGetUniformLocation(shaderProgram, "view");
	//MVP.uniformProjection = glGetUniformLocation(shaderProgram, "projection");//uboViewProjection   glGetAttribLocation
	
	

	//glUniformBlockBinding(shaderProgram, 0, 0);
	//glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, Block);
	//glGetProgramResourceiv();

	//uniformEyePosition = glGetUniformLocation(shaderProgram, "eyePosition");

	//uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderProgram, "directionalLight.base.colour");
	//uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderProgram, "directionalLight.base.ambientIntensity");
	//uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderProgram, "directionalLight.direction");
	//uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderProgram, "directionalLight.base.diffuseIntensity");

	//uniformSpecularIntensity = glGetUniformLocation(shaderProgram, "material.specularIntensity");
	//uniformShininess = glGetUniformLocation(shaderProgram, "material.shininess");

	/*
	uniformPointLightCount = glGetUniformLocation(shaderProgram, "pointLightCount");
	
	
	for (int i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderProgram, locBuff);

	}*/

	/*uniformSpotLightCount = glGetUniformLocation(shaderProgram, "spotLightCount");

	for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", i);
		uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderProgram, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderProgram, locBuff);

	}*/

	//glBindFragDataLocation(shaderProgram, 0, "outColor");
}

void Shaders::SetProjection(glm::mat4 projection) {

	#pragma region Set Projection
	glUseProgram(shaderProgram);
	glBindBuffer(GL_UNIFORM_BUFFER, getMvpLocation());

	glBufferSubData(GL_UNIFORM_BUFFER, 128, 64, &projection);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	/////glUniformMatrix4fv(shaders.getUniformProjection(), 1, GL_FALSE, glm::value_ptr(projection));
	glUseProgram(0);
	#pragma endregion
}

std::vector<char> Shaders::readFile(const std::string& filePath)//const char* filePath)
{

	std::ifstream file(filePath, std::ios::in | std::ios::binary |std::ios::ate);
	
	//Check if file stream successfully opened
	if (!file.is_open())
	{
		throw std::runtime_error("Could not open file!");
	}

	//Get current read position and use to resize file buffer
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> fileBuffer(fileSize);

	//Move read position (seek to) the start of the file
	file.seekg(0);

	//Read the file data into the buffer
	file.read(fileBuffer.data(), fileSize);

	file.close();

	return fileBuffer;

	/*std::ifstream File;
	std::string shader;
	std::string line;

	File.open(filePath);
	
	if (File.is_open())
	{
		while (!File.eof())
		{
			std::getline(File, line);
			shader.append(line + "\n");
		}
	}
	
	File.close();

	std::string code = shader.c_str();

	
	if (code.c_str() != NULL)
	{
		return code;
	}*/
	
}

const uint32_t Shaders::getProgram()
{
	return shaderProgram;
}

const uint32_t Shaders::getUniformModel()
{
	//ubo.model
	return UBObuffer;
}
const uint32_t Shaders::getUniformProjection()
{
	//ubo.projection
	return UBObuffer;
}
const uint32_t Shaders::getUniformView()
{
	//ubo.view;
	return UBObuffer;
}

const uint32_t Shaders::getMvpLocation()
{
	return UBObuffer;
}

//uint32_t Shaders::getAmbientIntensityLocation()
//{
//	return uniformDirectionalLight.uniformAmbientIntensity;
//}
//uint32_t Shaders::getAmbientColourLocation()
//{
//	return uniformDirectionalLight.uniformColour;
//}
//uint32_t Shaders::getDiffuseIntensityLocation()
//{
//	return uniformDirectionalLight.uniformDiffuseIntensity;
//}
//uint32_t Shaders::getDirectionLocation()
//{
//	return uniformDirectionalLight.uniformDirection;
//}
uint32_t Shaders::getSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}
uint32_t Shaders::getShininessLocation()
{
	return uniformShininess;
}
uint32_t Shaders::getEyePositionLocation()
{
	return uniformEyePosition;
}
/*
void Shaders::SetDirectionalLight(DirectionalLight* dLight)
{
	dLight->UseLight(
		uniformDirectionalLight.uniformAmbientIntensity, 
		uniformDirectionalLight.uniformColour, 
		uniformDirectionalLight.uniformDiffuseIntensity, 
		uniformDirectionalLight.uniformDirection);
}

void Shaders::SetPointLight(PointLight* pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(
			uniformPointLight[i].uniformAmbientIntensity, 
			uniformPointLight[i].uniformColour, 
			uniformPointLight[i].uniformDiffuseIntensity, 
			uniformPointLight[i].uniformPosition, 
			uniformPointLight[i].uniformConstant, 
			uniformPointLight[i].uniformLinear, 
			uniformPointLight[i].uniformExponent
		);
	}
}

void Shaders::SetSpotLight(SpotLight* pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(
			uniformSpotLight[i].uniformAmbientIntensity,
			uniformSpotLight[i].uniformColour,
			uniformSpotLight[i].uniformDiffuseIntensity,
			uniformSpotLight[i].uniformPosition,
			uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant,
			uniformSpotLight[i].uniformLinear,
			uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge
		);
	}
}
*/
void Shaders::clearShader()
{
	
	if (shaderProgram != 0)
	{
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}
	glDeleteBuffers(1, &UBObuffer);
	UBObuffer = 0;
	
	//ubo.uniformModel = 0;
	//ubo.uniformProjection = 0;
}
#pragma endregion

Shaders::~Shaders()
{
	clearShader();
}