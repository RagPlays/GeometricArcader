#include "Pickup.h"

#include "Player/Player.h"
#include "FlyFishTools/FlyFishUtils.h"

using namespace Engine;

Pickup::Pickup()
	: m_Window{ Application::Get().GetWindow() }
	, m_Position{}
	, m_Size{ Random::Range(15.f, 25.f) }
{
	RandomizePosition();
}

void Pickup::Render() const
{
	Renderer2D::SetDrawColor(Color::blue);
	FlyFishUtils::DrawFillRect(m_Position, m_Size);
}

void Pickup::RandomizePosition()
{
	const float windowWidth{ static_cast<float>(m_Window.GetWidth()) };
	const float windowHeight{ static_cast<float>(m_Window.GetHeight()) };
	const float halfWidth{ windowWidth * 0.5f };
	const float halfHeight{ windowHeight * 0.5f };

	const glm::vec3 randomPosition
	{
		Random::Range(0.f, m_Window.GetWidth() - m_Size.x) - halfWidth,
		Random::Range(0.f, m_Window.GetHeight() - m_Size.y) - halfHeight,
		-1.f
	};

	m_Position = TriVector{ randomPosition.x, randomPosition.y, randomPosition.z };
}

const TriVector& Pickup::GetPosition() const
{
	return m_Position;
}

const glm::vec2& Pickup::GetSize() const
{
	return m_Size;
}