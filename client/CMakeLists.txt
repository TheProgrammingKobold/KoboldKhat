add_executable(client
	main.cpp
	client.cpp
)

target_link_libraries(client PRIVATE networking)
if (WIN32)
	target_link_libraries(client PRIVATE ws2_32 wsock32)
endif()
