#include "Game.h"

using namespace Engine;

Game::Game()
	: m_Window{ Application::Get().GetWindow() }
	, m_BorderCollision{ 0 }
	, m_Player{}
	, m_Enemies{}
{
	ENGINE_TRACE("Game Started");

	CreateEnemies();
}

void Game::OnEvent(Event& e)
{
	m_BorderCollision.OnEvent(e);
	m_Player.OnEvent(e);
}

void Game::Update(float deltaTime)
{
	m_Player.Update(m_BorderCollision, deltaTime);

	for (auto& enemy : m_Enemies)
	{
		enemy.Update(m_BorderCollision, deltaTime);
	}
}

void Game::Render() const
{
	m_Player.Render();

	for (const auto& enemy : m_Enemies)
	{
		enemy.Render();
	}
}

void Game::CreateEnemies()
{
	const float halfWidth{ m_Window.GetWidth() * 0.5f };
	const float halfHeight{ m_Window.GetHeight() * 0.5f };
	const float quarterWidth{ halfWidth * 0.5f };
	const float quarterHeight{ halfHeight * 0.5f };

	for (int i = 0; i < 10000; ++i)
	{
		const glm::vec2 randomSize{ Random::Vec2(50.f, 50.f) };
		const float randomX{ Random::Range(quarterWidth, halfWidth - (randomSize.x * 0.5f)) * (Random::Bool() ? -1.f : 1.f) };
		const float randomY{ Random::Range(quarterHeight, halfHeight - (randomSize.y * 0.5f)) * (Random::Bool() ? -1.f : 1.f) };
		const TriVector position{ 0.f, 0.f, 0.f };

		m_Enemies.push_back(Enemy{ position, randomSize });
	}
}