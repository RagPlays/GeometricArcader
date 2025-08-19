#include "StartScreen.h"

using namespace Engine;

StartScreen::StartScreen()
	: m_Window{ Engine::Application::Get().GetWindow() }
    , m_ConfirmedStart{}
    , m_BackgroundTexture{ Texture2D::Create(std::string(ASSET_PATH) + "startScreenBackground.png") }
{
    ENGINE_TRACE("StartScreen Started");
}

void StartScreen::OnEvent(Event& e)
{
    if (e.GetEventType() == EventType::KeyReleased)
    {
        m_ConfirmedStart = true;
    }
}

void StartScreen::Update(float deltaTime)
{
}

void StartScreen::Render() const
{
    const glm::vec2 winSize{ static_cast<float>(m_Window.GetWidth()), static_cast<float>(m_Window.GetHeight()) };
	Renderer2D::DrawTexture(m_BackgroundTexture, { 0.0f, 0.0f }, winSize);
}

bool StartScreen::IsComplete() const
{
	return m_ConfirmedStart;
}

IGameState::GameStateType StartScreen::NextState() const
{
	return GameStateType::MainGame;
}