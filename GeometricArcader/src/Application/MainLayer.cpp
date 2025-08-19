#include "MainLayer.h"

#include <imgui/imgui.h>

#include "GameStates/Game.h"
#include "GameStates/StartScreen.h"
#include "GameStates/EndScreen.h"

using namespace Engine;

MainLayer::MainLayer()
	: Layer{ "MainLayer" }
	, m_Window{ Application::Get().GetWindow() }
	, m_MinWidth{ 1280 }
	, m_MinHeight{ 720 }
	, m_Camera{}
	, m_StartScreen{}
	, m_MainGame{}
	, m_EndScreen{}
	, m_pCurrentGameState{}
	, m_CurrentGameStateType{ IGameState::GameStateType::None }
	, m_ShowImgui{}
	, m_RestartGameRequest{}
{
	m_Window.SetSizeLimits(m_MinWidth, m_MinHeight);
	m_Window.SetAspectRatio(16, 9);

	UpdateGameStateChange();
}

MainLayer::~MainLayer()
{
}

void MainLayer::OnUpdate()
{
	// Update //
	const FrameTimer& timer{ FrameTimer::Get() };
	UpdateGameStateChange();
	ENGINE_ASSERT_MSG(m_pCurrentGameState, "No current game state set!");
	m_pCurrentGameState->Update(timer.GetSeconds());

	// Render //
	Renderer2D::ResetStats();
	RenderCommand::SetClearColor({ 0.075f, 0.075f, 0.075f, 0.075f });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_Camera.GetCamera());
	{
		m_pCurrentGameState->Render();
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

	if (m_pCurrentGameState) m_pCurrentGameState->OnEvent(e);
}

void MainLayer::UpdateGameStateChange()
{
	if (m_pCurrentGameState && m_pCurrentGameState->IsComplete())
	{
		m_CurrentGameStateType = m_pCurrentGameState->NextState();
		switch (m_CurrentGameStateType)
		{
		case IGameState::GameStateType::Start:
			m_StartScreen = std::make_unique<StartScreen>();
			m_pCurrentGameState = m_StartScreen.get();
			break;
		case IGameState::GameStateType::MainGame:
			m_MainGame = std::make_unique<Game>();
			m_pCurrentGameState = m_MainGame.get();
			break;
		case IGameState::GameStateType::End:
			ENGINE_ASSERT_MSG(m_MainGame != nullptr, "MainGame cant be nullptr here!");
			m_EndScreen = std::make_unique<EndScreen>(m_MainGame->GetScore());
			m_pCurrentGameState = m_EndScreen.get();
			break;
		default:
			ENGINE_ASSERT_MSG(false, "Unknown game state type encountered!");
			break;
		}
	}

	// if no active state yet (first run)
	if (!m_pCurrentGameState)
	{
		m_StartScreen = std::make_unique<StartScreen>();
		m_pCurrentGameState = m_StartScreen.get();
		m_CurrentGameStateType = IGameState::GameStateType::Start;
	}
}

bool MainLayer::OnKeyReleased(KeyReleasedEvent& e)
{
	const bool iPressedThisFrame{ e.GetKeyCode() == Key::I };
	const bool escPressedThisFrame{ e.GetKeyCode() == Key::Escape };
	const bool leftCtrlPressed{ Input::IsKeyPressed(Key::LeftControl) };
	const bool leftAltPressed{ Input::IsKeyPressed(Key::LeftAlt) };

	// ImGui Toggle
	if (iPressedThisFrame && leftCtrlPressed)
	{
		m_ShowImgui = !m_ShowImgui;
	}

	// Fullscreen Toggle
	if(e.GetKeyCode() == Key::Enter && leftAltPressed)
	{
		m_Window.SetFullscreen(!m_Window.IsFullscreen());
	}

	// Quit Application
	if (escPressedThisFrame)
	{
		Application::Get().Close();
		return true;
	}

	return false;
}