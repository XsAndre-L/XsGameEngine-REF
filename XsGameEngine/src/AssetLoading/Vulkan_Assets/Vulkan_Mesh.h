#pragma once
#include"../../Renderer/Vulkan_Renderer/VulkanUtillities.h"


class Vulkan_Mesh
{
public:
	Vulkan_Mesh() = default;
	Vulkan_Mesh(	
		VkPhysicalDevice newPhysicalDevice, 
		VkDevice newDevice,
		VkQueue transferQueue, 
		VkCommandPool transferCommandPool,
		std::vector<Vertex>* vertices, 
		std::vector<uint32_t> * indices, 
		int newTexId);

	~Vulkan_Mesh() = default;

	size_t getTexId() { return texId; };
	void setTexId(size_t ID) { texId = ID; }

	//virtual Vulkan_Mesh getOBJ() override { return *this; }
	uint32_t getVertexCount() { return vertexCount; };
	uint32_t getIndexCount() { return indexCount; };
	VkBuffer getVertexBuffer() { return vertexBuffer; };
	VkBuffer getIndexBuffer() { return indexBuffer; };

	void destroyBuffers();
	
private:

	size_t texId{};

	// Vertex Info
	uint32_t vertexCount;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	// Index Info
	uint32_t indexCount;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	// Device Storage
	VkPhysicalDevice physicalDevice;
	VkDevice device;

	// Functions
	void createVertexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices);
	void createIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t>* indices);
	
};

