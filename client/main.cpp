#include "networking.h"

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <winnt.h>

enum AppState
{
	CONNECT,
	CHAT,
};

class ClientApp
{
public:

	ClientApp()
	{
		runGUI();
	}

private:

	void runGUI()
	{
		if (!glfwInit())
		{
			std::cout << "ERROR glfw failed to init!\n";
			return;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(800, 600, "Imgui demo", nullptr, nullptr);
		if (!m_window)
		{
			std::cout << "ERROR window failed to create\n";
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_window);
		glfwSwapInterval(1); // Enables vsync
		
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "ERROR glad failed to init\n";
			glfwTerminate();
			return;
		}


		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 330");	

		AppState appState = CONNECT;

		char ip[128] = "127.0.0.1";
		int port = 12345;
		char username[128] = "";
		char messageBuffer[256] = "";

		while (!glfwWindowShouldClose(m_window))
		{
			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			switch (appState)
			{
				case AppState::CONNECT:
				{
					ImGui::Begin("Connect to Server");
					ImGui::InputText("IP Address", ip, sizeof(ip));
					ImGui::InputInt("Port", &port);
					ImGui::InputText("Username", username, sizeof(username));

					if (ImGui::Button("Connect"))
					{
						m_client = std::make_unique<Client>(m_context, ip, static_cast<uint16_t>(port), username);
						m_client->connect();
						appState = AppState::CHAT;
					}

					ImGui::End();
					
				} break;

				case AppState::CHAT:
				{
					ImGui::Begin("Chat");
					{
						ImGui::BeginChild("Messages", ImVec2(0, -30), true);

						while (!m_client->isMessageQueueEmpty())
						{
							std::string messBuffer = m_client->popMessageFromQueue();

							m_messages.push_back(messBuffer);
						}

						for (const auto& message : m_messages)
						{
							ImGui::TextUnformatted(message.c_str());
						}

						ImGui::EndChild();

						ImGui::InputText("Message", messageBuffer, sizeof(messageBuffer));
						ImGui::SameLine();
						if (ImGui::Button("Send"))
						{
							m_client->sendMessage(messageBuffer);

							std::string tempMsg = "'";
							tempMsg += username;
							tempMsg += "': ";
							tempMsg += messageBuffer;

							m_messages.push_back(messageBuffer);

							messageBuffer[0] = '\0';
						}
					}
					ImGui::End();

				} break;
			}

			ImGui::Render();
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(m_window);
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

private:

	asio::io_context m_context;

	GLFWwindow* m_window;

	std::unique_ptr<Client> m_client;

	std::vector<std::string> m_messages;

};

int main()
{
	ClientApp app;


	/*
	std::cout << "Please type the user name you'd like to use before connecting to the server!!\n";

	std::string username;
	std::getline(std::cin, username);

	try 
	{
		asio::io_context context;

		Client client(context, "127.0.0.1", 12345, username);

		client.connect();


		std::string message;
		
		while (client.isConnected())
		{
			std::getline(std::cin, message);
			if (message == "/quit") break;
			client.sendMessage(message);
		}
		
	}
	catch (const std::exception& e) 
	{
		std::cerr << "ERROR: " << e.what() << '\n';
	}
	*/

	return 0;
}
