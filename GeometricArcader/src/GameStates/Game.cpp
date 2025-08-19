#include "Game.h"

#include "Collision/Collision2D.h"

using namespace Engine;

Game::Game()
	: m_Window{ Application::Get().GetWindow() }
	, m_BorderCollision{ 0 }
	, m_Player{}
	, m_GameTimer{}
	, m_Pickups{}
	, m_Pillar{}
	, m_Score{}
{
	ENGINE_TRACE("MainGame Started");

	SetupGameTimer();

	m_Pickups.resize(8); // Initialize 8 pickups
}

void Game::OnEvent(Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(Game::OnWindowResize));

	m_BorderCollision.OnEvent(e);
	m_Player.OnEvent(e);
	m_Pillar.OnEvent(e);
	m_GameTimer.OnEvent(e);
}

void Game::Update(float deltaTime)
{
	m_Player.Update(m_BorderCollision, deltaTime);
	m_Pillar.Update(m_Player, deltaTime);

	UpdatePickups();
	UpdateGameTimer(deltaTime);
}

void Game::Render() const
{
	m_Player.Render();
	m_Pillar.Render();

	RenderPickups();

	m_GameTimer.Render();
}

bool Game::IsComplete() const
{
	return m_GameTimer.IsFull();
}

IGameState::GameStateType Game::NextState() const
{
	return GameStateType::End;
}

uint32_t Game::GetScore() const
{
	return m_Score;
}

void Game::UpdatePickups()
{
	constexpr float energyGainOnPickup{ 1.f };
	const TriVector& playerPos{ m_Player.GetPosition() };
	const glm::vec2& playerSize{ m_Player.GetSize() };

	for (auto& pickup : m_Pickups)
	{
		if (Collision2D::HasCollisionGEOA(pickup.GetPosition(), pickup.GetSize(), playerPos, playerSize))
		{
			++m_Score;
			pickup.RandomizePosition();
			m_Player.AddEnergy(energyGainOnPickup);
		}
	}
}

void Game::RenderPickups() const
{
	for (const auto& pickup : m_Pickups)
	{
		pickup.Render();
	}
}

void Game::SetupGameTimer()
{
	const glm::vec2 winSize{ static_cast<float>(m_Window.GetWidth()), static_cast<float>(m_Window.GetHeight()) };
	constexpr float gameDuration{ 30.f };

	m_GameTimer.SetMaxValue(gameDuration);

	m_GameTimer.SetAnchor(UIAnchor::LeftTop);
	m_GameTimer.SetMargin({ 30.f, 30.f });
	m_GameTimer.SetSize({ winSize.x - (2.f * m_GameTimer.GetMargin().x), 30.f });

	m_GameTimer.SetOutlineColor(Color::lightGray);
	m_GameTimer.SetBackgroundColor(Color::darkGray);
	m_GameTimer.SetValueColor(Color::white);
}

void Game::UpdateGameTimer(float deltaTime)
{
	m_GameTimer.AddValue(deltaTime);
	if (m_GameTimer.GetProgress() > 0.9f)
	{
		m_GameTimer.SetValueColor(Color::red);
	}
}

bool Game::OnWindowResize(WindowResizeEvent& e)
{
	SetupGameTimer();

	return false;
}