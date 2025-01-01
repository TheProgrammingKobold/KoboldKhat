set(GLFW_DIR "${CMAKE_SOURCE_DIR}/client/depend/GLFW")
set(GLAD_DIR "${CMAKE_SOURCE_DIR}/client/depend/GLAD")
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/client/depend/IMGUI")

# To add GLAD
add_library(glad STATIC
	${GLAD_DIR}/src/glad.c
)
target_include_directories(glad PUBLIC
	${GLAD_DIR}
)

# To add GLFW
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
	IMPORTED_LOCATION "${GLFW_DIR}/libglfw3.a"
	INTERFACE_INCLUDE_DIRECTORIES "${GLFW_DIR}"
)

# adding IMGUI
set(IMGUI_SRC
	${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
	${IMGUI_DIR}/imgui_impl_opengl3.cpp
	${IMGUI_DIR}/imgui_impl_glfw.cpp

)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
	${IMGUI_DIR}
)
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)
	
find_package(OpenGL REQUIRED)

add_executable(client
	main.cpp
)

target_link_libraries(client PRIVATE
	glad
	glfw
	imgui
	OpenGL::GL
	networking
)

target_link_libraries(client PRIVATE networking)
if (WIN32)
	target_link_libraries(client PRIVATE ws2_32 wsock32)
endif()

