cmake_minimum_required(VERSION 3.20.0)

project(XsGameEngine)


add_subdirectory(Submodules)
add_subdirectory(XsGameEngine)



message("message here is it ------------ $ENV{VULKAN_SDK} -- " )



add_compile_definitions(VULKAN)

add_compile_options(-std=c++17)
set(CMAKE_CXX_STANDARD 17)


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
	PRIVATE "Submodules/assimp/build/lib/${CMAKE_BUILD_TYPE}"
	PRIVATE "Submodules/assimp/build/contrib/zlib/${CMAKE_BUILD_TYPE}"
)

target_link_libraries(${PROJECT_NAME}
	"glfw"
	"opengl32"
	"vulkan-1"
	"zlibstaticd"
	"assimp-vc142-mtd"
)


#add_library(test STATIC IMPORTED [])
#target_link_libraries(app_external_deps PRIVATE)