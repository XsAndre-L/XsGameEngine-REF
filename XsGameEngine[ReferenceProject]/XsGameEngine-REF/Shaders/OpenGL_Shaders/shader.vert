#version 460 			// Use GLSL 4.5

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
//layout(location = 3) in vec3 col;

//layout (location = 3) uniform mat4 model;		//These locations work
//layout (location = 4) uniform mat4 view;
//layout (location = 5) uniform mat4 projection;

layout(std140, binding = 0) uniform UboViewProjection 
{
	mat4 model;
	mat4 view;
	mat4 projection;
}uboVP; 
//layout(location = 6) uniform UboViewProjection uboVP;

/*layout(std140 ,binding = 0) uniform UboViewProjection {
	mat4 model;
	mat4 view;
	mat4 projection;
} uboViewProjection;*/



//layout(push_constant) uniform PushModel {
//	mat4 model;
//} pushModel;

layout(location = 0) out vec3 fragColor;
//layout(location = 1) out vec2 TexCoords;
//layout(location = 2) out vec3 Normal;
//layout(location = 3) out vec3 FragPos;

void main(){
	
	gl_Position =  uboVP.projection * uboVP.view * uboVP.model * vec4(pos,1.0);
	fragColor = vec3(0.0,0.0,1.0); //col;	 
	//TexCoords = tex;
	//Normal = mat3(transpose(inverse(uboViewProjection.model))) * norm;
	//FragPos = (uboViewProjection.model * vec4(pos, 1.0)).xyz;
}