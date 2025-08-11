#include "Game.h"

#include "FlyFishTools/FlyFishUtils.h"

using namespace Engine;

Game::Game()
	: m_Window{ Application::Get().GetWindow() }
	, m_Player{}
{
}

void Game::OnEvent(Engine::Event& e)
{
	m_Player.OnEvent(e);
}

void Game::Update(float deltaTime)
{
	m_Player.Update(deltaTime);
}

void Game::Render() const
{
	DrawAxes();
	m_Player.Render();
	DrawLineTest();
}

void Game::DrawAxes() const
{
	const float windowWidth{ static_cast<float>(m_Window.GetWidth()) };
	const float windowHeight{ static_cast<float>(m_Window.GetHeight()) };
	const float halfWidth{ windowWidth * 0.9f };
	const float halfHeight{ windowHeight * 0.9f };

	const BiVector xAxis{ BiVector::LineFromPoints(-1, 0.f, 0.f, 1, 0.f, 0.f) };
	const BiVector yAxis{ BiVector::LineFromPoints(0.f, -1, 0.f, 0.f, 1, 0.f) };

	Renderer2D::SetDrawColor(Color::red);
	FlyFishUtils::DrawLine(xAxis, halfWidth);
	Renderer2D::SetDrawColor(Color::green);
	FlyFishUtils::DrawLine(yAxis, halfHeight);
}

void Game::DrawLineTest() const
{
	Renderer2D::SetDrawColor(Color::magenta);
	const glm::vec3 p1(-200.f, 200.f, 0.f);
	const glm::vec3 p2(425.f, 150.f, 0.f);
	float distance = glm::distance(p1, p2);
	const BiVector testLine = BiVector::LineFromPoints(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
	FlyFishUtils::DrawLine(testLine, distance);
	Renderer2D::SetDrawColor(Color::cyan);
	Renderer2D::DrawFilledCircle(p1, 5.f);
	Renderer2D::DrawFilledCircle(p2, 5.f);
}