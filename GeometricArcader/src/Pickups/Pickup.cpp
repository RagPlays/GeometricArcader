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
	const glm::vec2 winSize{ static_cast<float>(m_Window.GetWidth()), static_cast<float>(m_Window.GetHeight()) };
	const glm::vec2 halfWinSize{ winSize * 0.5f };

	const glm::vec2 randomPosition
	{
		Random::Range(m_Size.x, winSize.x - m_Size.x) - halfWinSize.x,
		Random::Range(m_Size.y, winSize.y - m_Size.y) - halfWinSize.y,
	};

	m_Position = TriVector{ randomPosition.x, randomPosition.y, -1.f };
}

const TriVector& Pickup::GetPosition() const
{
	return m_Position;
}

const glm::vec2& Pickup::GetSize() const
{
	return m_Size;
}