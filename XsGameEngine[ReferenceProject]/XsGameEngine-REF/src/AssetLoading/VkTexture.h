#pragma once

#include "../Utillities.h"

#include <stb_image/stb_image.h>



class VkTexture
{
public:
	VkTexture(VkPhysicalDevice* physicalDevice, VkDevice* device, VkQueue* graphicsQueue, VkCommandPool* graphicsCommandPool, VkSampler* sampler, VkDescriptorPool* descriptorPool, VkDescriptorSetLayout* samplerSetLayout);

	int createTexture(std::string fileName);
	bool createTextureImage(std::string fileName);
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	stbi_uc* loadTextureFile(std::string fileName, int* width, int* height, VkDeviceSize* imageSize);
	int createTextureDescriptor(VkImageView textureImage);

	bool getState() { return DESTROYED; }
	void cleanUpTexture();


	VkDescriptorSet getSDS() { return SamplerDescriptorSet; }

private:
	VkPhysicalDevice* PhysicalDevice{};
	VkDevice* Device{};
	VkQueue* GraphicsQueue{};
	VkCommandPool* GraphicsCommandPool{};
	VkSampler* TextureSampler{};
	VkDescriptorPool* SamplerDescriptorPool{};
	VkDescriptorSetLayout* SamplerSetLayout{};

	uint32_t mipLevel{};
	VkImage textureImage{};
	VkDeviceMemory textureImageMemory{};
	VkImageView textureImageView{};
	VkDescriptorSet SamplerDescriptorSet{};

	bool DESTROYED = false;
};

