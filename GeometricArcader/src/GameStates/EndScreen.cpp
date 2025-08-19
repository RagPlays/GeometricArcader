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
}

bool EndScreen::IsComplete() const
{
	return m_PressedEnter;
}

IGameState::GameStateType EndScreen::NextState() const
{
	return GameStateType::MainGame;
}