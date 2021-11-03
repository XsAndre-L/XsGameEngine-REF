

set(MAIN_SCOPE
	
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/CommonValues.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/PreprocessorDefs.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Time.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Time.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/ProgramLayers/ProgramLayers.h"
)

set(WINDOW_SCOPE
"${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Public/GLFW_Window.h"
"${CMAKE_CURRENT_SOURCE_DIR}/src/Window/Private/GLFW_Window.cpp"
)

set(RENDERER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer")
set(RENDERER_SCOPE
"${RENDERER_PATH}/GUI_Renderer.cpp"
"${RENDERER_PATH}/GUI_Renderer.h"


"${RENDERER_PATH}/Vulkan_Renderer/Vulkan_Renderer.h"
"${RENDERER_PATH}/Vulkan_Renderer/Vulkan_Renderer.cpp"
"${RENDERER_PATH}/Vulkan_Renderer/VulkanUtillities.h"
"${RENDERER_PATH}/Vulkan_Renderer/VkValidation.h"

"${RENDERER_PATH}/OpenGL_Renderer/OpenGL_Renderer.h"
"${RENDERER_PATH}/OpenGL_Renderer/OpenGL_Renderer.cpp"
"${RENDERER_PATH}/OpenGL_Renderer/Shaders.h"
"${RENDERER_PATH}/OpenGL_Renderer/Shaders.cpp"
"${RENDERER_PATH}/OpenGL_Renderer/Material.h"
"${RENDERER_PATH}/OpenGL_Renderer/Material.cpp"
)

set(ASSET_LOADING_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/AssetLoading")
set(ASSET_LOADING_SCOPE
"${ASSET_LOADING_PATH}/Entity.h"
"${ASSET_LOADING_PATH}/Entity.cpp"

"${ASSET_LOADING_PATH}/Vulkan_Assets/Vulkan_Assets.h"
"${ASSET_LOADING_PATH}/Vulkan_Assets/Vulkan_Assets.cpp"
"${ASSET_LOADING_PATH}/Vulkan_Assets/Vulkan_Mesh.h"
"${ASSET_LOADING_PATH}/Vulkan_Assets/Vulkan_Mesh.cpp"
"${ASSET_LOADING_PATH}/Vulkan_Assets/Vulkan_MeshModel.h"
"${ASSET_LOADING_PATH}/Vulkan_Assets/Vulkan_MeshModel.cpp"
"${ASSET_LOADING_PATH}/Vulkan_Assets/Vulkan_Texture.h"
"${ASSET_LOADING_PATH}/Vulkan_Assets/Vulkan_Texture.cpp"

"${ASSET_LOADING_PATH}/OpenGL_Assets/OpenGL_Assets.h"
"${ASSET_LOADING_PATH}/OpenGL_Assets/OpenGL_Assets.cpp"
"${ASSET_LOADING_PATH}/OpenGL_Assets/OpenGL_Mesh.h"
"${ASSET_LOADING_PATH}/OpenGL_Assets/OpenGL_Mesh.cpp"
"${ASSET_LOADING_PATH}/OpenGL_Assets/OpenGL_MeshModel.h"
"${ASSET_LOADING_PATH}/OpenGL_Assets/OpenGL_MeshModel.cpp"
"${ASSET_LOADING_PATH}/OpenGL_Assets/OpenGL_Texture.h"
"${ASSET_LOADING_PATH}/OpenGL_Assets/OpenGL_Texture.cpp"

"${ASSET_LOADING_PATH}/Lights/Light.h"
"${ASSET_LOADING_PATH}/Lights/Light.cpp"
"${ASSET_LOADING_PATH}/Lights/DirectionalLight.h"
"${ASSET_LOADING_PATH}/Lights/DirectionalLight.cpp"

)

set(VENDOR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vendor")
set(VENDOR_SCOPE
"${VENDOR_PATH}/Glad/glad.c"
"${VENDOR_PATH}/Glad/glad.h"
"${VENDOR_PATH}/Glad/khrplatform.h"

"${VENDOR_PATH}/ImGUI/imconfig.h"
"${VENDOR_PATH}/ImGUI/imgui.cpp"
"${VENDOR_PATH}/ImGUI/imgui.h"
"${VENDOR_PATH}/ImGUI/imgui_demo.cpp"
"${VENDOR_PATH}/ImGUI/imgui_draw.cpp"
"${VENDOR_PATH}/ImGUI/imgui_impl_glfw.h"
"${VENDOR_PATH}/ImGUI/imgui_impl_glfw.cpp"
"${VENDOR_PATH}/ImGUI/imgui_internal.h"
"${VENDOR_PATH}/ImGUI/imgui_tables.cpp"
"${VENDOR_PATH}/ImGUI/imgui_widgets.cpp"
"${VENDOR_PATH}/ImGUI/imstb_rectpack.h"
"${VENDOR_PATH}/ImGUI/imstb_textedit.h"
"${VENDOR_PATH}/ImGUI/imstb_truetype.h"

"${VENDOR_PATH}/ImGUI/Vulkan/imgui_impl_vulkan.cpp"
"${VENDOR_PATH}/ImGUI/Vulkan/imgui_impl_vulkan.h"
"${VENDOR_PATH}/ImGUI/OpenGL/imgui_impl_opengl3.h"
"${VENDOR_PATH}/ImGUI/OpenGL/imgui_impl_opengl3.cpp"
"${VENDOR_PATH}/ImGUI/OpenGL/imgui_impl_opengl3_loader.h"
)




set(ALL_CODE 
${VENDOR_SCOPE}
${WINDOW_SCOPE}
${RENDERER_SCOPE}
${ASSET_LOADING_SCOPE}
${MAIN_SCOPE}

PARENT_SCOPE
)