#include "Game.h"

#include "Collision/Collision2D.h"

#include "FlyFishTools/FlyFishUtils.h"

using namespace Engine;

Game::Game()
	: m_Window{ Application::Get().GetWindow() }
	, m_BorderCollision{ 0 }
	, m_Player{}
	, m_GameTimer{}
	, m_GravityTypeImage{}
	, m_PullGravityTexture{ Texture2D::Create(std::string(ASSET_PATH) + "pullGravity.png") }
	, m_PushGravityTexture{ Texture2D::Create(std::string(ASSET_PATH) + "pushGravity.png") }
	, m_Pickups{}
	, m_UltraPickup{}
	, m_Pillar{}
	, m_Score{}
{
	ENGINE_TRACE("MainGame Started");

	SetupGameTimer();
	SetupGravityTypeImage();

	m_Pickups.resize(8); // Initialize 8 pickups
}

void Game::OnEvent(Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(Game::OnWindowResize));
	dispatcher.Dispatch<KeyReleasedEvent>(ENGINE_BIND_EVENT_FN(Game::OnKeyReleased));

	m_BorderCollision.OnEvent(e);
	m_Player.OnEvent(e);
	m_Pillar.OnEvent(e);

	m_GameTimer.OnEvent(e);
	m_GravityTypeImage.OnEvent(e);
}

void Game::Update(float deltaTime)
{
	m_Player.Update(m_BorderCollision, deltaTime);
	m_Pillar.Update(m_Player, deltaTime);
	m_UltraPickup.Update(deltaTime);

	UpdatePickups();
	UpdateGameTimer(deltaTime);
}

void Game::Render() const
{
	/*const TriVector point0(-200.f, 500.f, 0.f);
	const TriVector point1(500.f, -400.f, 0.f);
	const BiVector line{ BiVector::LineFromPoints(point0.e032(), point0.e013(), point0.e021(), point1.e032(), point1.e013(), point1.e021()) };
	Renderer2D::SetDrawColor(Color::magenta);
	FlyFishUtils::DrawLine(line, FlyFishUtils::DistanceGA(point0, point1));

	Renderer2D::SetDrawColor(Color::cyan);
	FlyFishUtils::DrawFilledCircle(point0, 10.f);
	FlyFishUtils::DrawFilledCircle(point1, 10.f);*/

	//m_BorderCollision.Render();
	m_Player.Render();
	m_Pillar.Render();
	m_UltraPickup.Render();

	RenderPickups();

	m_GameTimer.Render();
	m_GravityTypeImage.Render();
}

bool Game::IsComplete() const
{
	return m_GameTimer.IsFull() || m_Player.IsDead();
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

	if (Collision2D::HasCollisionGEOA(m_UltraPickup.GetPosition(), m_UltraPickup.GetSize(), playerPos, playerSize))
	{
		m_Score += 3;
		m_UltraPickup.RandomizePosition();
		m_Player.AddEnergy(energyGainOnPickup * 2);
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

void Game::SetupGravityTypeImage()
{
	m_GravityTypeImage.SetTexture(m_Pillar.GetGravityType() ? m_PullGravityTexture : m_PushGravityTexture);
	m_GravityTypeImage.SetAnchor(UIAnchor::LeftBottom);
	m_GravityTypeImage.SetMargin({ 30.f + 75.f + 30.f, 30.f + 50.f + 30.f });
	m_GravityTypeImage.SetSize({ 75.f, 50.f });
}

bool Game::OnWindowResize(WindowResizeEvent& e)
{
	SetupGameTimer();

	return false;
}

bool Game::OnKeyReleased(KeyReleasedEvent& e)
{
	const KeyCode key{ e.GetKeyCode() };
	if (key == Key::Left || key == Key::A ||
		key == Key::Right || key == Key::D)
	{
		m_Pillar.ToggleGravityType();
		m_GravityTypeImage.SetTexture(m_Pillar.GetGravityType() ? m_PullGravityTexture : m_PushGravityTexture);
	}

	return false;
}