#pragma once
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <string>
#include <vector>

#include <GLEW/glew.h>

#include "CommonValues.h"

#include "DirectionalLight.h"
//#include "PointLight.h"
//#include "SpotLight.h"




class Shaders
{
public:
	Shaders();
	~Shaders();

	void AddShader(GLuint& program, std::vector<char> ShaderCode/*const char* ShaderCode*/, GLenum shaderType);
	void CompileShaders();
	std::vector<char> readFile(const std::string& filePath);		//MODIFIED

	//swaped GLuint with uint32_t 
	uint32_t getProgram();
	uint32_t getUniformModel();
	uint32_t getUniformProjection();
	uint32_t getUniformView();

	uint32_t getAmbientIntensityLocation();
	uint32_t getAmbientColourLocation();

	uint32_t getDiffuseIntensityLocation();
	uint32_t getDirectionLocation();

	uint32_t getSpecularIntensityLocation();
	uint32_t getShininessLocation();
	uint32_t getEyePositionLocation();

	//void SetDirectionalLight(DirectionalLight * dLight);
	//void SetPointLight(PointLight* pLight, unsigned int lightCount);
	//void SetSpotLight(SpotLight* pLight, unsigned int lightCount);

	void SetViewMatrix(glm::mat4 view) { viewMatrix = view; }
	glm::mat4* GetViewMatrix() { return &viewMatrix; }

	void clearShader();
private:
	int pointLightCount;
	int spotLightCount;

	GLuint shaderProgram{}, /*uniformModel, uniformProjection, uniformView,*/ uniformEyePosition,
		uniformSpecularIntensity, uniformShininess;

	//GLuint uniformModel;
	//GLuint uniformProjection;
	//GLuint uniformView;
	struct {
		glm::mat4 uniformModel;
		glm::mat4 uniformProjection;
		glm::mat4 uniformView;
	}ubo;
	GLuint UBObuffer;

	glm::mat4 viewMatrix;

	// Directional Light
	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	// PointLights
	GLuint uniformPointLightCount;
	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	// SpotLights
	GLuint uniformSpotLightCount;
	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;

	} uniformSpotLight[MAX_SPOT_LIGHTS];

	//const char* vertexShader;

	//const char* fragmentShader;
	
	std::string vertexShader;

	std::string fragmentShader;

};




