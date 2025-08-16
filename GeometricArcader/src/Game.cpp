#include "Game.h"

#include "Collision/Collision.h"

using namespace Engine;

Game::Game()
	: m_Window{ Application::Get().GetWindow() }
	, m_BorderCollision{ 0 }
	, m_Player{}
	, m_Pickups{}
{
	ENGINE_TRACE("Game Started");

	m_Pickups.resize(20); // Initialize 5 pickups
}

void Game::OnEvent(Event& e)
{
	m_BorderCollision.OnEvent(e);
	m_Player.OnEvent(e);
}

void Game::Update(float deltaTime)
{
	m_Player.Update(m_BorderCollision, deltaTime);

	const TriVector& playerPos{ m_Player.GetPosition() };
	const glm::vec2& playerSize{ m_Player.GetSize() };

	for (auto& pickup : m_Pickups)
	{
		if (Collision::HasCollision(pickup.GetPosition(), pickup.GetSize(), playerPos, playerSize))
		{
			ENGINE_TRACE("Pickup collected!");
			pickup.RandomizePosition();
		}
	}
}

void Game::Render() const
{
	m_Player.Render();

	for (const auto& pickup : m_Pickups)
	{
		pickup.Render();
	}
}