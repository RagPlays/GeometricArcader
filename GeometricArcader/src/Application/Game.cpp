#include "Game.h"

#include "FlyFishTools/FlyFishUtils.h"

using namespace Engine;

Game::Game()
	: m_Window{ Application::Get().GetWindow() }
	, m_BorderCollision{ 0 }
	, m_Player{}
{
	ENGINE_TRACE("Game Created");
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
	m_Player.Render();
	//m_BorderCollision.Render();
}