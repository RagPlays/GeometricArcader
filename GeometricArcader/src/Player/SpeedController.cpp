#include "SpeedController.h"

using namespace Engine;

SpeedController::SpeedController()
	: m_Window{ Application::Get().GetWindow() }
	, m_LeftBotMargin{ 30.f, 30.f + 50.f + 30.f }
	, m_Size{ 75.f, 50.f }
	, m_currentState{ ControllerState::NEUTRAL }
{
}

void SpeedController::Update()
{
	m_currentState = ControllerState::NEUTRAL;

	if(Input::IsKeyPressed(Key::W) || Input::IsKeyPressed(Key::Up))
	{
		m_currentState = ControllerState::FORWARD;
	}
	else if(Input::IsKeyPressed(Key::S) || Input::IsKeyPressed(Key::Down))
	{
		m_currentState = ControllerState::REVERSE;
	}
}

void SpeedController::Render() const
{
	constexpr float barRenderLayer{ 900.f }; // max layer is 1000.f
	constexpr float barOffset{ 10.f }; // offset for the energy bar

	const glm::vec2 fullBarSize
	{
		(m_Size.x - barOffset),
		(m_Size.y - barOffset)
	};

	const glm::vec2 renderPos // centerposition of the background bar
	{
		m_LeftBotMargin.x + (m_Size.x * 0.5f) - (m_Window.GetWidth() * 0.5f),
		m_LeftBotMargin.y + (m_Size.y * 0.5f) - (m_Window.GetHeight() * 0.5f),
	};

	Renderer2D::SetDrawColor(Color::lightGray);
	Renderer2D::DrawFilledRect(glm::vec3{ renderPos, barRenderLayer }, m_Size);

	switch (m_currentState)
	{
		case ControllerState::FORWARD:
			Renderer2D::SetDrawColor(Color::blue);
			break;
		case ControllerState::NEUTRAL:
			Renderer2D::SetDrawColor(Color::darkGray);
			break;
		case ControllerState::REVERSE:
			Renderer2D::SetDrawColor(Color::red);
			break;
	}
	Renderer2D::DrawFilledRect(glm::vec3{ renderPos, barRenderLayer + 1 }, fullBarSize);
}