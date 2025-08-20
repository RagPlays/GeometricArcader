#include "EndScreen.h"

using namespace Engine;

EndScreen::EndScreen(uint32_t score)
	: m_Window{ Engine::Application::Get().GetWindow() }
	, m_Score{ score }
	, m_BackgroundTexture{ Texture2D::Create(std::string(ASSET_PATH) + "endScreenBackground.png") }
	, m_PressedEnter{}
{
	ENGINE_TRACE("EndScreen Started");
}

void EndScreen::OnEvent(Event& e)
{
	if (e.GetEventType() == EventType::KeyReleased)
	{
		auto& keyEvent{ static_cast<KeyReleasedEvent&>(e) };
		if (keyEvent.GetKeyCode() == Key::Enter)
		{
			m_PressedEnter = true;
		}
	}
}

void EndScreen::Update(float deltaTime)
{
}

void EndScreen::Render() const
{
	const glm::vec2 winSize{ static_cast<float>(m_Window.GetWidth()), static_cast<float>(m_Window.GetHeight()) };
	Renderer2D::DrawTexture(m_BackgroundTexture, { 0.0f, 0.0f }, winSize);

	Renderer2D::SetDrawColor(Color::blue);

	// parameters
	constexpr int cols{ 8 };         // quads per row
	constexpr float quadSize{ 25.f }; // size of quad
	constexpr float halfQuadSize{ quadSize * 0.5f };
	constexpr float gap{ 20.f };        // spacing between quads

	// full cell size including gap
	const float cellSize{ quadSize + gap };
	const int totalRows{ (static_cast<int>(m_Score) + cols - 1) / cols };
	const float gridW{ cols * cellSize };
	const float gridH{ totalRows * cellSize };

	for (int idx{}; idx < static_cast<int>(m_Score); ++idx)
	{
		const int row{ idx / cols };
		const int col{ idx % cols };

		// how many columns in this row
		int rowCols = cols;
		if (row == totalRows - 1)
		{
			int remaining = static_cast<int>(m_Score) - row * cols;
			rowCols = remaining;
		}

		float rowW = rowCols * cellSize;

		// center this row separately
		float x = col * cellSize - rowW * 0.5f + halfQuadSize;
		float y = -(row * cellSize - gridH * 0.5f + halfQuadSize);

		Renderer2D::DrawFilledRect({ x, y, 1.0f }, { quadSize, quadSize });
	}
}

bool EndScreen::IsComplete() const
{
	return m_PressedEnter;
}

IGameState::GameStateType EndScreen::NextState() const
{
	return GameStateType::MainGame;
}