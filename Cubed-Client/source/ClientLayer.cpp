#include "ClientLayer.h"

#include "Walnut/Input/Input.h"
#include "Walnut/ImGui/ImGuiTheme.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

namespace Cubed {
	static void DrawRect(glm::vec2 position, glm::vec2 size, uint32_t color)
	{
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		ImVec2 min = ImGui::GetWindowPos() + ImVec2(position.x, position.y);
		ImVec2 max = min + ImVec2(size.x, size.y);
		drawList->AddRectFilled(min, max, color);
	}

	void ClientLayer::OnAttach()
	{
		m_Client.SetDataReceivedCallback([this](const Walnut::Buffer buffer) { OnDataReceived(buffer); });
	}

	void ClientLayer::OnDetach()
	{
		
	}

	void ClientLayer::OnUpdate(float ts)
	{
		if (m_Client.GetConnectionStatus() != Walnut::Client::ConnectionStatus::Connected)
			return;

		glm::vec2 dir = { 0.0f, 0.0f };
		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::W))
			dir.y = -1.0f;
		else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::S))
			dir.y = 1.0f;

		if (Walnut::Input::IsKeyDown(Walnut::KeyCode::A))
			dir.x = -1.0f;
		else if (Walnut::Input::IsKeyDown(Walnut::KeyCode::D))
			dir.x = 1.0f;


		if (glm::length(dir) > 0.0f)
		{
			dir = glm::normalize(dir);

			const float speed = 50.0f;
			m_PlayerVelocity = dir * speed;
		}

		m_PlayerPosition += m_PlayerVelocity * ts;

		glm::mix(m_PlayerPosition, glm::vec2(0, 0), 10.0f * ts);
	}

	void ClientLayer::OnRender()
	{

	}

	void ClientLayer::OnUIRender()
	{
		Walnut::Client::ConnectionStatus connectionStatus = m_Client.GetConnectionStatus();
		if (connectionStatus == Walnut::Client::ConnectionStatus::Connected)
		{
			// play game
			DrawRect(m_PlayerPosition, glm::vec2(50, 50), 0xFFFF00FF);
		}
		else
		{
			ImGui::Begin("Connect to Server");

			/*if (readOnly)
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);*/

			ImGui::InputText("Server address", &m_ServerAddress);
			if (connectionStatus == Walnut::Client::ConnectionStatus::FailedToConnect)
				ImGui::TextColored(ImColor(Walnut::UI::Colors::Theme::error), "Failed to connect to server");
			if (connectionStatus == Walnut::Client::ConnectionStatus::Connecting)
				ImGui::TextColored(ImColor(Walnut::UI::Colors::Theme::textDarker), "Connecting...");
			if (ImGui::Button("Connect"))
			{
				m_Client.ConnectToServer(m_ServerAddress);
			}

			ImGui::End();
		}

		ImGui::ShowDemoWindow();
	}
	void ClientLayer::OnDataReceived(const Walnut::Buffer buffer)
	{
	}
}