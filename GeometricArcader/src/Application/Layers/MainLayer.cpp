#include "MainLayer.h"

#include <imgui/imgui.h>

using namespace Engine;

MainLayer::MainLayer()
	: Layer{ "MainLayer" }
	, m_Window{ Application::Get().GetWindow() }
	, m_MinWidth{ 500 }
	, m_MinHeight{ 500 }
	, m_Camera{}
	, m_Game{}
	, m_ShowImgui{}
{
}

MainLayer::~MainLayer()
{
}

void MainLayer::OnUpdate()
{
	const FrameTimer& timer{ FrameTimer::Get() };
	m_Game.Update(timer.GetSeconds());

	Renderer2D::ResetStats();
	RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.f });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_Camera.GetCamera());
	{
		m_Game.Render();
	}
	Renderer2D::EndScene();
}

void MainLayer::OnImGuiRender()
{
	constexpr float imGuiFontScale{ 2.f };

	// timer
	static int nrOfFrames{};
	static float elapsedTime{};
	static float lastFPS{};
	++nrOfFrames;
	elapsedTime += FrameTimer::Get().GetSeconds();
	if (elapsedTime > 1.f)
	{
		lastFPS = static_cast<float>(nrOfFrames) / elapsedTime;
		elapsedTime = 0.f;
		nrOfFrames = 0;
	}

	if (!m_ShowImgui) return;

	const ImGuiViewport* const mainViewport{ ImGui::GetMainViewport() };
	ImGui::SetNextWindowPos(ImVec2{ mainViewport->Pos.x + 10.f, mainViewport->Pos.y + 10.f }, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(300.f, 330.f), ImGuiCond_Always);
	ImGui::SetNextWindowViewport(mainViewport->ID);

	const auto flags
	{
		ImGuiWindowFlags_NoMove |		// Prevent moving
		ImGuiWindowFlags_NoResize |		// Prevent resizing
		ImGuiWindowFlags_NoCollapse |	// Prevent collapsing
		ImGuiWindowFlags_NoTitleBar		// Remove title bar if you want
	};

	ImGui::Begin("RendererStats", nullptr, flags);
	{
		const auto& stats{ Renderer2D::GetStats() };

		ImGui::SetWindowFontScale(imGuiFontScale);
		ImGui::Text("General Stats:");
		ImGui::Text("FPS: %f", lastFPS);

		ImGui::Spacing();

		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Point Count: %d", stats.pointCount);
		ImGui::Text("Line Count: %d", stats.lineCount);
		ImGui::Text("Quad Count: %d", stats.quadCount);
		ImGui::Text("Circle Count %d", stats.circleCount);
		ImGui::Text("Vertices Count: %d", stats.GetTotalVerticesCount());
		ImGui::Text("Indices Count: %d", stats.GetTotalIndicesCount());
	}
	ImGui::End();
}

void MainLayer::OnEvent(Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(MainLayer::OnWindowResized));
	dispatcher.Dispatch<KeyReleasedEvent>(ENGINE_BIND_EVENT_FN(MainLayer::OnKeyReleased));

	m_Camera.OnEvent(e);
	m_Game.OnEvent(e);
}

bool MainLayer::OnWindowResized(WindowResizeEvent& event)
{
	const unsigned int width{ event.GetWidth() };
	const unsigned int height{ event.GetHeight() };

	if (width == 0 && height == 0) return false; // Window Is Minimized

	if (width < m_MinWidth || height < m_MinHeight)
	{
		m_Window.SetWindowSize(std::max(width, m_MinWidth), std::max(height, m_MinHeight));
	}

	return false;
}

bool MainLayer::OnKeyReleased(KeyReleasedEvent& e)
{
	const bool iPressedThisFrame{ e.GetKeyCode() == Key::I };
	const bool ctrlPressed{ Input::IsKeyPressed(Key::LeftControl) };

	if (iPressedThisFrame && ctrlPressed)
	{
		m_ShowImgui = !m_ShowImgui;
	}

	return false;
}