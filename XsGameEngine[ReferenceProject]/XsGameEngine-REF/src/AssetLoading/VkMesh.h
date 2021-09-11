#pragma once
#include"../Utillities.h"


class VkMesh
{
public:
	VkMesh();
	VkMesh(	VkPhysicalDevice newPhysicalDevice, VkDevice newDevice,
			VkQueue transferQueue, VkCommandPool transferCommandPool,
			std::vector<Vertex>* vertices, std::vector<uint32_t> * indices, 
			int newTexId);


	size_t getTexId() { return texId; };
	void setTexId(size_t ID) { texId = ID; }

	int getVertexCount() { return vertexCount; };
	int getIndexCount() { return indexCount; };
	VkBuffer getVertexBuffer() { return vertexBuffer; };
	VkBuffer getIndexBuffer() { return indexBuffer; };

	void destroyBuffers();
	~VkMesh();
private:

	size_t texId;

	// Vertex Info
	int vertexCount;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	// Index Info
	int indexCount;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	// Device Storage
	VkPhysicalDevice physicalDevice;
	VkDevice device;

	// Functions
	void createVertexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices);
	void createIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t>* indices);
	
};

