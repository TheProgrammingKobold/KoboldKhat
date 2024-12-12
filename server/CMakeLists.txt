add_executable(server
	main.cpp
	server.cpp
)

target_link_libraries(server PRIVATE networking)

if (WIN32)
	
	target_link_libraries(server PRIVATE ws2_32 wsock32)
endif()
