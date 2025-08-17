#include "MainLayer.h"

#include <imgui/imgui.h>

#include "Game.h"

using namespace Engine;

MainLayer::MainLayer()
	: Layer{ "MainLayer" }
	, m_Window{ Application::Get().GetWindow() }
	, m_MinWidth{ 1080 }
	, m_MinHeight{ 720 }
	, m_Camera{}
	, m_Game{ std::make_unique<Game>() }
	, m_ShowImgui{}
	, m_RestartGameRequest{}
{
	m_Window.SetSizeLimits(m_MinWidth, m_MinHeight);
}

MainLayer::~MainLayer()
{
}

void MainLayer::OnUpdate()
{
	if (m_RestartGameRequest) 
	{
		m_Game = std::make_unique<Game>();
		m_RestartGameRequest = false;
	}

	const FrameTimer& timer{ FrameTimer::Get() };
	m_Game->Update(timer.GetSeconds());

	Renderer2D::ResetStats();
	RenderCommand::SetClearColor({ 0.075f, 0.075f, 0.075f, 0.075f });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_Camera.GetCamera());
	m_Game->Render();
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
	ImGui::SetNextWindowSize(ImVec2(300.f, 350.f), ImGuiCond_Always);
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
		ImGui::Text("VSync: %s", m_Window.IsVSync() ? "On" : "Off");

		ImGui::Spacing();
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
	dispatcher.Dispatch<KeyReleasedEvent>(ENGINE_BIND_EVENT_FN(MainLayer::OnKeyReleased));

	m_Camera.OnEvent(e);
	m_Game->OnEvent(e);
}

bool MainLayer::OnKeyReleased(KeyReleasedEvent& e)
{
	const bool iPressedThisFrame{ e.GetKeyCode() == Key::I };
	const bool leftCtrlPressed{ Input::IsKeyPressed(Key::LeftControl) };
	const bool leftAltPressed{ Input::IsKeyPressed(Key::LeftAlt) };

	// ImGui Toggle
	if (iPressedThisFrame && leftCtrlPressed)
	{
		m_ShowImgui = !m_ShowImgui;
	}

	// Game Reset
	if (e.GetKeyCode() == Key::R)
	{
		m_RestartGameRequest = true;
	}

	// Fullscreen Toggle
	if(e.GetKeyCode() == Key::Enter && leftAltPressed)
	{
		m_Window.SetFullscreen(!m_Window.IsFullscreen());
	}

	return false;
}