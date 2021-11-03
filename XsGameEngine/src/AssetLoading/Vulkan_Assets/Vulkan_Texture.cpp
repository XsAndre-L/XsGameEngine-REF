#include "Vulkan_Texture.h"

Vulkan_Texture::Vulkan_Texture(VkPhysicalDevice* physicalDevice,VkDevice* device, VkQueue* graphicsQueue, VkCommandPool* graphicsCommandPool, VkSampler* sampler, VkDescriptorPool* descriptorPool, VkDescriptorSetLayout* samplerSetLayout)
{
	PhysicalDevice = physicalDevice;
	Device = device;
	GraphicsQueue = graphicsQueue;
	GraphicsCommandPool = graphicsCommandPool;
	TextureSampler = sampler;
	SamplerDescriptorPool = descriptorPool;
	SamplerSetLayout = samplerSetLayout;
}

int Vulkan_Texture::createTexture(std::string fileName)
{

	//Create Texture Image and get its location in array
	bool TexSuccess = createTextureImage(fileName);

	if (TexSuccess == false) { return 1; }

	//Create image view and add to list 
	textureImageView = createImageView(*Device, textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, mipLevel);
	//textureImageViews.push_back(imageView);

	//TODO: Create Descriptor Set Here
	int descriptorLoc = createTextureDescriptor(textureImageView);

	return 0;
}

bool Vulkan_Texture::createTextureImage(std::string fileName)
{
	//Load Image File
	int width, height;
	VkDeviceSize imageSize;
	stbi_uc* imageData = loadTextureFile(fileName, &width, &height, &imageSize);

	if (imageData == nullptr) {
		return false;
	}

	//Calculate MipLevels								///busy
	mipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
	printf("mip levels: %d", mipLevel);

	//Create staging buffer to hold loaded data, ready to copy to device
	VkBuffer imageStagingBuffer;
	VkDeviceMemory imageStagingBufferMemory;
	createBuffer(*PhysicalDevice, *Device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		&imageStagingBuffer, &imageStagingBufferMemory);

	//Copy image data to staging buffer
	void* data;
	vkMapMemory(*Device, imageStagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, imageData, static_cast<size_t>(imageSize));
	vkUnmapMemory(*Device, imageStagingBufferMemory);

	//Free original image data
	stbi_image_free(imageData);

	//Create image to hold final texture
	//VkImage texImage;
	//VkDeviceMemory texImageMemory;
	textureImage = createImage(*PhysicalDevice, *Device, width, height, mipLevel, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &textureImageMemory);

	//Transfer Texture to a Transfer DST Optimal Layout
	transitionImageLayout(*Device, *GraphicsQueue, *GraphicsCommandPool, textureImage,
		VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevel);

	//Copy Data To Image
	copyImageBuffer(*Device, *GraphicsQueue, *GraphicsCommandPool, imageStagingBuffer, textureImage, width, height);

	//Transfer Texture to a shader read only Layout -- when creating mipMaps
	//transitionImageLayout(mainDevice.logicalDevice, graphicsQueue, graphicsCommandPool, texImage,							///bussy
	//	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels[mipLevels.size() - 1]);
	generateMipmaps(textureImage, VK_FORMAT_R8G8B8A8_SRGB, width, height, mipLevel);

	// Add texture data to vector for reference

	//textureImages.push_back(texImage);
	//textureImageMemory.push_back(texImageMemory);

	//Destroy staging buffers
	vkDestroyBuffer(*Device, imageStagingBuffer, nullptr);
	vkFreeMemory(*Device, imageStagingBufferMemory, nullptr);



	return true;
}

// Generate MipMaps for given texture
void Vulkan_Texture::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {		///busy

	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(*PhysicalDevice, imageFormat, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		throw std::runtime_error("texture image format does not support linear blitting!");
	}

	VkCommandBuffer commandBuffer = beginCommandBuffer(*Device, *GraphicsCommandPool);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;


	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++) {

		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		VkImageBlit blit{};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer,
			image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier);

	endAndSubmitCommandBuffer(*Device, *GraphicsCommandPool, *GraphicsQueue, commandBuffer);
}

stbi_uc* Vulkan_Texture::loadTextureFile(std::string fileName, int* width, int* height, VkDeviceSize* imageSize)
{
	int channels;

	std::string fileLoc = "Textures/" + fileName;
	stbi_uc* image = stbi_load(fileLoc.c_str(), width, height, &channels, STBI_rgb_alpha);

	if (!image)
	{
		//printf("Failed to load Texture ( %s )", fileName.c_str());
		std::cout << "Failed to load Texture : " << fileName << std::endl;
		image = nullptr;
	}

	//static_cast<uint64_t> because VkDeviceSize [imageSize] is 64bit large
	*imageSize = static_cast<uint64_t>(*width) * *height * 4;

	return image;
}


int Vulkan_Texture::createTextureDescriptor(VkImageView textureImage)
{
	VkDescriptorSet descriptorSet{};

	// Descriptor Set Allocation Info
	VkDescriptorSetAllocateInfo setAllocInfo{};
	setAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocInfo.descriptorPool = *SamplerDescriptorPool;
	setAllocInfo.descriptorSetCount = 1;
	setAllocInfo.pSetLayouts = SamplerSetLayout;

	// Allocate Descriptor Sets
	VkResult result = vkAllocateDescriptorSets(*Device, &setAllocInfo, &SamplerDescriptorSet);
	if (result != VK_SUCCESS) {

		throw std::runtime_error("could not allocate info! -");// + std::to_string(result));
	}

	VkDescriptorImageInfo imageInfo{};
	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageInfo.imageView = textureImage;
	imageInfo.sampler = *TextureSampler;

	VkWriteDescriptorSet descriptorWrite{};
	descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrite.dstSet = SamplerDescriptorSet;
	descriptorWrite.dstBinding = 0;
	descriptorWrite.dstArrayElement = 0;
	descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	descriptorWrite.descriptorCount = 1;
	descriptorWrite.pImageInfo = &imageInfo;

	vkUpdateDescriptorSets(*Device, 1, &descriptorWrite, 0, nullptr);

	//samplerDescriptorSets.push_back(descriptorSet);

	return 1;//samplerDescriptorSets.size() - 1;
}

void Vulkan_Texture::cleanUpTexture()
{
	if (getState()) { return; }

	vkDestroyImageView(*Device, textureImageView, nullptr);
	vkDestroyImage(*Device, textureImage, nullptr);
	vkFreeMemory(*Device, textureImageMemory, nullptr);

	DESTROYED = true;
}
