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

			if (m_client)
			{
				while (!m_client->isMessageQueueEmpty())
				{
					std::string messBuffer = m_client->popMessageFromQueue();

					m_messages.push_back(messBuffer);
				}
			}


			switch (appState)
			{
				case AppState::CONNECT:
				{
					// This is to make it fill the whole window 
					ImGui::SetNextWindowPos(ImVec2(0,0));
					ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
					
					ImGui::Begin("Connect to Server", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
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
					ImGui::SetNextWindowPos(ImVec2(0,0));
					ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
					ImGui::Begin("Main Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
					
						
					{
						ImGui::BeginChild("Messages", ImVec2(0, ImGui::GetContentRegionAvail().y - 30), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

						bool atBottom = ImGui::GetScrollY() >= ImGui::GetScrollMaxY();
						for (const auto& message : m_messages)
						{
							ImGui::TextWrapped("%s", message.c_str());
						}

						if (atBottom) ImGui::SetScrollHereY(1.0f);	

						ImGui::EndChild();
					}
					
					{
						// Needed to set the focus to the text input box so you don't have to click
						if (ImGui::IsWindowAppearing())
						{
							ImGui::SetItemDefaultFocus();
							ImGui::SetKeyboardFocusHere();
						}

						if (ImGui::InputText("Message", messageBuffer, sizeof(messageBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
						{
							// Checks if the message is empty or not before sending message
							if (std::strlen(messageBuffer) > 0 && std::any_of(messageBuffer, messageBuffer + std::strlen(messageBuffer), [](char c) { return  !std::isspace(c); }))
							{
								m_client->sendMessage(messageBuffer);

								std::string tempMsg = "'";
								tempMsg += username;
								tempMsg += "': ";
								tempMsg += messageBuffer;

								m_messages.push_back(messageBuffer);

								messageBuffer[0] = '\0';
							}

							// Puts the window in focus after pressing enter
							ImGui::SetKeyboardFocusHere(-1);
						}

						

						ImGui::SameLine();
						if (ImGui::Button("Send"))
						{
							
							if (std::strlen(messageBuffer) > 0 && std::any_of(messageBuffer, messageBuffer + std::strlen(messageBuffer), [](char c) { return  !std::isspace(c); }))
							{
								m_client->sendMessage(messageBuffer);

								std::string tempMsg = "'";
								tempMsg += username;
								tempMsg += "': ";
								tempMsg += messageBuffer;

								m_messages.push_back(messageBuffer);

								messageBuffer[0] = '\0';
							}
							ImGui::SetKeyboardFocusHere(-1);
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

	return 0;
}
