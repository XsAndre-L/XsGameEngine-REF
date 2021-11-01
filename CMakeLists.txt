cmake_minimum_required(VERSION 3.20.0)

project(XsGameEngine)


add_subdirectory(Submodules)
add_subdirectory(XsGameEngine)

#Compiler Configurations
add_compile_definitions(VULKAN)

add_compile_options(-std=c++17)
set(CMAKE_CXX_STANDARD 17)


if(EXISTS "$ENV{VULKAN_SDK}")
	message("-- VULKAN_SDK FOUND : $ENV{VULKAN_SDK}" )
else()
	message("-- ERROR : VULKAN_SDK environment variable not found!" )
endif()


add_executable(${PROJECT_NAME}
${ALL_CODE}

)

target_include_directories(${PROJECT_NAME} 
PUBLIC "$ENV{VULKAN_SDK}/include"
PUBLIC "Submodules/glfw/include"
PUBLIC "Submodules/glm"
PUBLIC "Submodules/assimp/include"
PUBLIC "XsGameEngine/vendor"
)


target_link_directories(${PROJECT_NAME}
	PRIVATE "Submodules/glfw/src"
	PRIVATE "$ENV{VULKAN_SDK}/Lib"
	PRIVATE "${PROJECT_BINARY_DIR}/Submodules/assimp/contrib/zlib"
	PRIVATE "${PROJECT_BINARY_DIR}/Submodules/assimp/lib"
)

target_link_libraries(${PROJECT_NAME}
	"glfw"
	"opengl32"
	"vulkan-1"
	"zlibstaticd"
	"assimp-vc142-mtd"
)

file(COPY ${PROJECT_SOURCE_DIR}/XsGameEngine/Models DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/XsGameEngine/Textures DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/XsGameEngine/Shaders DESTINATION ${PROJECT_BINARY_DIR})

#add_library(test STATIC IMPORTED [])
#target_link_libraries(app_external_deps PRIVATE)