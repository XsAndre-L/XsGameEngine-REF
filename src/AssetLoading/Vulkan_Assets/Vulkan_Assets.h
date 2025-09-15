#pragma once
// This Class in meant to keep track of all assets that are created
//  Keeps track of all "Models" , "Textures" , "Lights"
#include <mutex>
#include <thread>

#include "../../CommonValues.h"
#include "../Lights/DirectionalLight.h"
#include "../asset-constants.h"
#include "Vulkan_MeshModel.h"
#include "Vulkan_Texture.h"

class Vulkan_Assets {
  public:
    Assimp::Importer *importer;
    const aiScene *scene;
    bool Load_Model_Files = false; // TODO
    fs::path path;

    Vulkan_Assets() = default;
    Vulkan_Assets(VkPhysicalDevice &physicalDevice,
                  VkDevice &logicalDevice,
                  VkQueue &transferQueue,
                  VkCommandPool &graphicsCommandPool,
                  VkSampler &textureSampler,
                  VkDescriptorPool &samplerDescriptorPool,
                  VkDescriptorSetLayout &samplerSetLayout);

    void asyncAssets(const fs::path &ModelPath);
    void createAsset(const fs::path &ModelPath);

    // Models
    int *SetSelected() { return &selectedModel; }

    int createVulkan_MeshModel(fs::path modelPath);
    void destroyVulkan_MeshModel(int MeshId);

    // Scene Objects
    std::vector<std::string> MeshModelNames;
    std::vector<Vulkan_MeshModel> MeshModelList;

    // Textures
    uint16_t addTexture(std::string fileName);

    std::vector<std::string> textureNames;
    std::vector<Vulkan_Texture> textureList;

    // Lights
    void addLight();
    DirectionalLight newLight;

    std::vector<DirectionalLight> LightList;
    // AllLights AllL;

    struct AllLights {
        DirectionalLight::DirectionalLightInfo directionalLight[MAX_DIRECTIONAL_LIGHTS];

    } allLights;

    // Other
    struct AllAssets {
        std::vector<std::string> *ModelNames{};
        std::vector<Vulkan_MeshModel> *modelPointers{};
        std::vector<Vulkan_Texture> *texturePointers{};
        AllLights *lightPointers{};
    } allAssets;

    AllAssets *getAssetInfo();

    void cleanUpAssets();

  private:
    int selectedModel = 0;

    VkPhysicalDevice *PhysicalDevice{nullptr};
    VkDevice *Device{nullptr};
    VkQueue *TransferQueue{nullptr};
    VkCommandPool *GraphicsCommandPool{nullptr};

    VkSampler *TextureSampler{nullptr};
    VkDescriptorPool *SamplerDescriptorPool{nullptr};
    VkDescriptorSetLayout *SamplerSetLayout{nullptr};

    std::thread *AssetLoadThread{nullptr};
};
