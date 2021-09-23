#include "VkMesh.h"



#pragma region Public Functions & Construction and Deconstruction


VkMesh::VkMesh(VkPhysicalDevice newPhysicalDevice, VkDevice newDevice, VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices, std::vector<uint32_t>* indices, int newTexId)
{
	
	vertexCount = static_cast<uint32_t>(vertices->size());
	indexCount = static_cast<uint32_t>(indices->size());
	physicalDevice = newPhysicalDevice;
	device = newDevice;

	//vkWaitForFences(newDevice, 1, &fences->at(*currentFrame), VK_TRUE, std::numeric_limits<uint32_t>::max());
	createVertexBuffer(transferQueue, transferCommandPool, vertices);
	createIndexBuffer(transferQueue, transferCommandPool, indices);

	//modelMatrix.model = glm::mat4(1.0f);
}

void VkMesh::destroyBuffers()
{
	if (vertexBufferMemory != nullptr) {
		vkDestroyBuffer(device, vertexBuffer, nullptr);
		vkFreeMemory(device, vertexBufferMemory, nullptr);
		vkDestroyBuffer(device, indexBuffer, nullptr);
		vkFreeMemory(device, indexBufferMemory, nullptr);
	}
}

#pragma endregion


#pragma region Create Buffer Functions
// Buffer To store all vertices in memory
void VkMesh::createVertexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<Vertex>* vertices)
{
	//Get size of buffer needed for vertices 
	VkDeviceSize bufferSize = sizeof(Vertex) * vertices->size();

	#pragma region Create Staging/SRC Buffer
	//Temporary buffer to "stage" vertex data before transferring to GPU
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	//Create staging Buffer and Allocate Memory to it
	createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);
	#pragma endregion

	#pragma region Map Memory To Staging Buffer [vertices DATA -to- staging Buffer]
	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices->data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);
	#pragma endregion


	#pragma region Create GPU Only Buffer [&vertexBuffer]
	//Create buffer with TRANSFER_DST_BIT to mark as recipient of transfer data(also VERTEX_BUFFER)
	//Buffer memory is to be DEVICE_LOCAL_BIT meaning memory is on the GPU and only accessible by it and not the CPU
	createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &vertexBuffer, &vertexBufferMemory);
	#pragma endregion

	#pragma region Copy data from "Staging Buffer"(CPU and GPU visible) -to- "Vertex Buffer"(GPU visible)
	// Copy staging buffer to vertex buffer on GPU
	copyBuffer(device, transferQueue, transferCommandPool, stagingBuffer, vertexBuffer, bufferSize);
	#pragma endregion

	//CleanUp
	#pragma region Destroy Staging Buffer and Free Memory
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
	#pragma endregion
}
// Buffer To store all indices in memory
void VkMesh::createIndexBuffer(VkQueue transferQueue, VkCommandPool transferCommandPool, std::vector<uint32_t>* indices)
{
	//Get size of buffer needed for indices
	VkDeviceSize bufferSize = sizeof(uint32_t) * indices->size();

	#pragma region Create Staging/SRC Buffer
	//Temporary buffer to "stage" vertex data before transferring to GPU
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	//Create staging Buffer and Allocate Memory to it
	createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
				 &stagingBuffer, &stagingBufferMemory);

	#pragma endregion

	#pragma region Map Memory To Staging Buffer [vertices DATA -to- staging Buffer]
	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices->data(), (size_t)bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);
	#pragma endregion


	#pragma region Create GPU Only Buffer [&indexBuffer]
	//Create buffer with TRANSFER_DST_BIT to mark as recipient of transfer data(also INDEX_BUFFER)
	//Buffer memory is to be DEVICE_LOCAL_BIT meaning memory is on the GPU and only accessible by it and not the CPU
	createBuffer(physicalDevice, device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &indexBuffer, &indexBufferMemory);
	#pragma endregion

	//#pragma region Copy data from "Staging Buffer"(CPU and GPU visible) -to- "Index Buffer"(GPU visible)
	// Copy staging buffer to vertex buffer on GPU
	copyBuffer(device, transferQueue, transferCommandPool, stagingBuffer, indexBuffer, bufferSize);
	//#pragma endregion

	#pragma region Destroy Staging Buffer and Free Memory
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
	#pragma endregion
}

#pragma endregion


