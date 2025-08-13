#include "Game.h"

#include "FlyFishTools/FlyFishUtils.h"

using namespace Engine;

Game::Game()
	: m_Window{ Application::Get().GetWindow() }
	, m_BorderCollision{ 50 }
	, m_Player{}
{
}

void Game::OnEvent(Event& e)
{
	m_BorderCollision.OnEvent(e);
	m_Player.OnEvent(e);
}

void Game::Update(float deltaTime)
{
	m_Player.Update(deltaTime);

	if(m_BorderCollision.IsPlayerColliding(m_Player))
	{
		ENGINE_TRACE("Player is colliding with the border!");
	}
}

void Game::Render() const
{
	DrawAxes();
	m_Player.Render();
	m_BorderCollision.Render();
}

void Game::DrawAxes() const
{
	const float windowWidth{ static_cast<float>(m_Window.GetWidth()) };
	const float windowHeight{ static_cast<float>(m_Window.GetHeight()) };

	Renderer2D::SetDrawColor(Color::red);
	FlyFishUtils::DrawLine(FlyFishUtils::xAxis, windowWidth * 0.75f);
	Renderer2D::SetDrawColor(Color::green);
	FlyFishUtils::DrawLine(FlyFishUtils::yAxis, windowHeight * 0.75f);
}