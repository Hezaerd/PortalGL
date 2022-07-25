cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME portalGL)


project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_FILE "${PROJECT_SOURCE_DIR}/game/src/*.cpp")
file(GLOB_RECURSE INCLUDE_FILE "${PROJECT_SOURCE_DIR}/game/include/*.h")
file(GLOB_RECURSE GL_SRC_FILE "${PROJECT_SOURCE_DIR}/libGL/OPENGL PROJECT/*.cpp")
file(GLOB_RECURSE GL_INCLUDE_FILE "${PROJECT_SOURCE_DIR}/libGL/OPENGL PROJECT/*.h")
add_executable(${PROJECT_NAME}
	"${SRC_FILE}"
	"${INCLUDE_FILE}"
	"${GL_SRC_FILE}"
	"${GL_INCLUDE_FILE}"
	)
	
source_group("Header Files\\OpenGL" FILES ${GL_INCLUDE_FILE})
source_group("Source Files\\OpenGL" FILES ${GL_SRC_FILE})
source_group("Header Files\\portalGL" FILES ${INCLUDE_FILE})
source_group("Source Files\\portalGL" FILES ${SRC_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC "libGL/OPENGL PROJECT/source/include")
target_include_directories(${PROJECT_NAME} PUBLIC "libmaths")
target_include_directories(${PROJECT_NAME} PUBLIC "game/include")

set_property(TARGET ${PROJECT_NAME} PROPERTY
VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
set(LIB_DIR "external")
#	Add GLAD
target_include_directories(${PROJECT_NAME} PUBLIC "libGL/GLAD/include")
target_sources(${PROJECT_NAME} PUBLIC "libGL/GLAD/src/glad.cpp")


#	Add GLFW
set(GLFW3_ROOT "libGL/GLFW")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMake")
find_package(GLFW3 REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW3_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${GLFW3_LIBRARY})


#	Add STB
target_include_directories(${PROJECT_NAME} PUBLIC "${LIB_DIR}/STB")

# Add Maths LIbrary
set(MATHS_DIR "libmaths")
file(GLOB_RECURSE MATH_SOURCES "${MATHS_DIR}/*.cpp" "${MATHS_DIR}/*.h")
add_library(LibMaths STATIC EXCLUDE_FROM_ALL "${MATH_SOURCES}")
target_include_directories(LibMaths PRIVATE ${MATHS_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${MATHS_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC LibMaths)


#Add bullet
set(BULLET_BUILD_LIB_DIR "${LIB_DIR}/BULLET/lib")
set(BULLET_ROOT "${LIB_DIR}/BULLET")

find_package(Bullet REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${BULLET_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC
	debug ${BULLET_COLLISION_LIBRARY_DEBUG}
	debug ${BULLET_DYNAMICS_LIBRARY_DEBUG}
	debug ${BULLET_LINEAR_MATH_DEBUG}
	optimized ${BULLET_DYNAMICS_LIBRARY}
	optimized ${BULLET_LINEAR_MATH}
	optimized ${BULLET_COLLISION_LIBRARY}
	)


# Add ImGUI
# Set where the ImGui files are stored
set(IMGUI_PATH  "${LIB_DIR}/imgui-master")
file(GLOB_RECURSE IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_PATH}/*.h)
add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ${IMGUI_PATH})
target_include_directories(ImGui PUBLIC ${GLFW3_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ImGui)

#Add irrKang
set(IRRKLANG_ROOT "${LIB_DIR}/irrKlang")
find_package(Irrklang REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${IRRKLANG_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${IRRKLANG_LIBRARY})


#Add assimp
set(ASSIMP_ROOT "${LIB_DIR}/assimp")
find_package(Assimp REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC
		debug ${ASSIMP_LIBRARY_DEBUG}
		optimized ${ASSIMP_LIBRARY}
		)
		
#Add RapidJson
set(RAPIDJSON_ROOT "${LIB_DIR}/rapidjson/include")
find_package(RapidJSON REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${RapidJSON_INCLUDE_DIR})



