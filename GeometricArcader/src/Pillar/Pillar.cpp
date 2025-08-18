#include "Pillar.h"

#include "FlyFishTools/FlyFishUtils.h"
#include "Player/Player.h"

using namespace Engine;

Pillar::Pillar()
	: m_Window{ Application::Get().GetWindow() }
	, m_Position{ 0.f, 0.f, 0.f }
	, m_Size{ 60.f, 60.f }
	, m_InfluenceRadius{ 400.f } // Radius of influence for gravity (beyond this, gravity is 0)
	, m_UsingGravity{ false }
{
	m_Position.e021() = 0.f; // Ensure z-coordinate is set to 0 (otherwise player energy could be messed up)
}

void Pillar::OnEvent(Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<MouseButtonPressedEvent>(ENGINE_BIND_EVENT_FN(Pillar::OnMousePressed));
}

void Pillar::Update(Player& player, float deltaTime)
{
	UpdatePlayerGravity(player, deltaTime);
}

void Pillar::Render() const
{
	constexpr float renderLayer{ -100.f };
	const glm::vec2 renderPos{ FlyFishUtils::ToVec2(m_Position) };

	if (m_UsingGravity)
	{
		Renderer2D::SetDrawColor(glm::vec4{ 0.8f, 0.8f, 0.8f, 1.f });
		Renderer2D::DrawCircle(glm::vec3{ renderPos, renderLayer }, m_Size.x * 0.7f);

		Renderer2D::SetDrawColor(glm::vec4{ 0.5f, 0.5f, 0.5f, 1.f });
		Renderer2D::DrawCircle(glm::vec3{ renderPos, renderLayer + 1.f }, m_Size.x * 0.9f);

		Renderer2D::SetDrawColor(glm::vec4{ 0.3f, 0.3f, 0.3f, 1.f });
		Renderer2D::DrawCircle(glm::vec3{ renderPos, renderLayer + 2.f }, m_InfluenceRadius, 0.05f);
	}

	const glm::vec4 sunColor{Color::FromRGBA(254, 241, 114) };
	Renderer2D::SetDrawColor(sunColor);
	Renderer2D::DrawFilledCircle(glm::vec3{ renderPos, renderLayer }, m_Size.x * 0.5f);
	Renderer2D::SetDrawColor(Color::black);
	FlyFishUtils::DrawFilledCircle(m_Position + TriVector{0.f, 0.f, 1.f}, m_Size.x * 0.4f);
}

const TriVector& Pillar::GetPosition() const
{
	return m_Position;
}

void Pillar::UpdatePlayerGravity(Player& player, float deltaTime)
{
	m_UsingGravity = Input::IsKeyPressed(Key::Space);
	if (!m_UsingGravity) return;

	constexpr float gravityStrength{ 80000000.f };	// Gravity constant
	constexpr float maxForce{ 5000.f };				// clamp to avoid insane speeds

	const TriVector& playerPos{ player.Get2DPosition() };
	const float dist{ FlyFishUtils::Distance(m_Position, playerPos) };
	if (dist < m_InfluenceRadius)
	{
		const glm::vec3 dir{ FlyFishUtils::GetDirection(m_Position, playerPos) };

		// Newtonian gravity: F = G / r^2
		const float strength{ gravityStrength / (dist * dist) };
		const float clampedStrength{ std::clamp(strength, 0.f, maxForce) };

		player.AddForce(FlyFishUtils::GetTranslator(dir, clampedStrength * deltaTime));
	}
}

bool Pillar::OnMousePressed(const MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == Mouse::ButtonLeft)
	{
		const glm::vec2 halfSize{ m_Size * 0.5f };
		const glm::vec2 winSize{ static_cast<float>(m_Window.GetWidth()), static_cast<float>(m_Window.GetHeight()) };
		const glm::vec2 halfWinSize{ winSize * 0.5f };
		const glm::vec2 mousePos{ Input::GetMousePosition() };
		const glm::vec2 clampedMousePos{ glm::clamp(mousePos, halfSize, winSize - halfSize) };

		const glm::vec2 newPosition
		{
			clampedMousePos.x - halfWinSize.x,
			halfWinSize.y - clampedMousePos.y   // flip Y
		};

		m_Position.e032() = newPosition.x;
		m_Position.e013() = newPosition.y;
	}

	return false;
}