#pragma once
//#include <stdio.h>
#include <fstream>
//#include <string>
#include <vector>

//#include <GLEW/glew.h>
#include "../../../vendor/Glad/glad.h"

//#include "CommonValues.h"

#include <GLM/glm.hpp>

//#include "DirectionalLight.h"
//#include "PointLight.h"
//#include "SpotLight.h"



class Shaders
{
public:
	Shaders();
	~Shaders();

	void AddShader(GLuint& program, std::vector<char> ShaderCode/*const char* ShaderCode*/, GLenum shaderType);
	void CompileShaders();
	void SetProjection(glm::mat4 Projection);
	std::vector<char> readFile(const std::string& filePath);		//MODIFIED

	//swaped GLuint with uint32_t 
	const uint32_t getProgram();
	const uint32_t getUniformModel();
	const uint32_t getUniformProjection();
	const uint32_t getUniformView();

	const uint32_t getMvpLocation();

	//uint32_t getAmbientIntensityLocation();
	//uint32_t getAmbientColourLocation();

	//uint32_t getDiffuseIntensityLocation();
	//uint32_t getDirectionLocation();

	uint32_t getSpecularIntensityLocation();
	uint32_t getShininessLocation();
	uint32_t getEyePositionLocation();

	//void SetDirectionalLight(DirectionalLight * dLight);
	//void SetPointLight(PointLight* pLight, unsigned int lightCount);
	//void SetSpotLight(SpotLight* pLight, unsigned int lightCount);

	void SetViewMatrix(glm::mat4 view) { ubo.uniformViewMatrix = view; }
	glm::mat4 const GetViewMatrix() const { return ubo.uniformViewMatrix; }
	glm::mat4* ViewMatrix() { return &ubo.uniformViewMatrix; }
	

	void clearShader();
private:
	//int pointLightCount;
	//int spotLightCount;

	GLuint shaderProgram, uniformEyePosition{},
		uniformSpecularIntensity{}, uniformShininess{};

	//GLuint uniformModel;
	//GLuint uniformProjection;
	//GLuint uniformView;

	
	//GLuint ModelMat;
	struct {
		glm::mat4 uniformModelMatrix{ glm::mat4(1.0) };
		glm::mat4 uniformProjectionMatrix{ glm::mat4(1.0) };
		glm::mat4 uniformViewMatrix{ glm::mat4(1.0) };
	}ubo;
	GLuint UBObuffer;

	glm::mat4 viewMatrix;

	// Directional Light
	//struct {
	//	GLuint uniformColour;
	//	GLuint uniformAmbientIntensity;
	//	GLuint uniformDiffuseIntensity;

	//	GLuint uniformDirection;
	//} uniformDirectionalLight;

	//// PointLights
	//GLuint uniformPointLightCount;
	//struct {
	//	GLuint uniformColour;
	//	GLuint uniformAmbientIntensity;
	//	GLuint uniformDiffuseIntensity;

	//	GLuint uniformPosition;
	//	GLuint uniformConstant;
	//	GLuint uniformLinear;
	//	GLuint uniformExponent;
	//} uniformPointLight[MAX_POINT_LIGHTS];

	//// SpotLights
	//GLuint uniformSpotLightCount;
	//struct {
	//	GLuint uniformColour;
	//	GLuint uniformAmbientIntensity;
	//	GLuint uniformDiffuseIntensity;

	//	GLuint uniformPosition;
	//	GLuint uniformConstant;
	//	GLuint uniformLinear;
	//	GLuint uniformExponent;

	//	GLuint uniformDirection;
	//	GLuint uniformEdge;

	//} uniformSpotLight[MAX_SPOT_LIGHTS];

	//const char* vertexShader;

	//const char* fragmentShader;
	
	//std::string vertexShader;

	//std::string fragmentShader;

};




