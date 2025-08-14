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
	m_Player.Update(m_BorderCollision, deltaTime);
}

void Game::Render() const
{
	DrawAxes();
	m_Player.Render();
	m_BorderCollision.Render();


	///////////////////////////
	// PLANE TRANLATION TEST //

	Vector plane{ 0.f, 1.0f, 0.0f, 0.0f }; // plane normal -> right (1, 0, 0) direction
	FlyFishUtils::Translate(plane, 80.f);
	const BiVector planeLine{ plane ^ FlyFishUtils::e3 };

	Renderer2D::SetDrawColor(Color::cyan);
	FlyFishUtils::DrawLine(planeLine, 500.f);

	if(FlyFishUtils::SignedDistanceToPlane(plane, m_Player.GetPosition()) < 0.f)
	{
		Renderer2D::DrawFilledCircle({ 0.f, 0.f, 1.f }, 20.f);
	}
	///////////////////////////
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