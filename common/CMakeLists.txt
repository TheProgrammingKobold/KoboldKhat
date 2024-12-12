file(GLOB NETWORKING_HEADERS
	*.h
)

file(GLOB NETWORKING_SOURCES
	*.cpp
)

add_library(networking STATIC
	${NETWORKING_SOURCES}
	${NETWORKING_HEADERS}
)

target_include_directories(networking PUBLIC ws2_32)

target_include_directories(networking PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/depend
)
