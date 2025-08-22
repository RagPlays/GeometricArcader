#include "UltraPickup.h"

#include "FlyFishTools/FlyFishUtils.h"

using namespace Engine;

UltraPickup::UltraPickup()
	: m_Window{Application::Get().GetWindow() }
	, m_Position{}
	, m_RotatePositions{}
	, m_Size{ 50.f, 50.f }
	, m_RenderCubeSize{ m_Size * 0.5f }
{
	RandomizePosition();
}

void UltraPickup::Update(float deltaTime)
{
	constexpr float rotateSpeed{ 30.f };
	const float deltaSpeed{ rotateSpeed * deltaTime };
	for (auto& pos : m_RotatePositions)
	{
		FlyFishUtils::RotateAroundPoint(pos, m_Position, deltaSpeed);
	}
}

void UltraPickup::Render() const
{
	constexpr float degToRad{ std::numbers::pi_v<float> / 180.0f };
	constexpr float deg45InRad{ degToRad * 45.f };

	Renderer2D::SetDrawColor(Color::FromRGBA(75, 75, 255));
	FlyFishUtils::DrawFillRect(m_Position, m_RenderCubeSize);
	for (const auto& pos : m_RotatePositions)
	{
		const BiVector dirLine{ FlyFishUtils::GetDirectionLineGA(m_Position, pos) };
		const float angleRad{ FlyFishUtils::GetAngleBetweenLinesGA(dirLine, FlyFishUtils::xAxis) };
		const float signedAngleRad{ dirLine.e31() > 0.f ? angleRad : -angleRad };
		FlyFishUtils::DrawFillRect(pos, m_RenderCubeSize, deg45InRad + signedAngleRad);
	}
}

const TriVector& UltraPickup::GetPosition() const
{
	return m_Position;
}

const glm::vec2& UltraPickup::GetSize() const
{
	return m_Size;
}

void UltraPickup::RandomizePosition()
{
	const glm::vec2 winSize{ static_cast<float>(m_Window.GetWidth()), static_cast<float>(m_Window.GetHeight()) };
	const glm::vec2 halfWinSize{ winSize * 0.5f };

	const glm::vec2 randomPosition
	{
		Random::Range(m_Size.x, winSize.x - m_Size.x) - halfWinSize.x,
		Random::Range(m_Size.y, winSize.y - m_Size.y) - halfWinSize.y,
	};

	m_Position = TriVector{ randomPosition.x, randomPosition.y, -1.f };

	const float offsetDist{ m_Size.x * 0.4f };
	m_RotatePositions =
	{
		TriVector{ m_Position.e032() + offsetDist, m_Position.e013(), m_Position.e021() },
		TriVector{ m_Position.e032() - offsetDist, m_Position.e013(), m_Position.e021() },
		TriVector{ m_Position.e032(), m_Position.e013() + offsetDist, m_Position.e021() },
		TriVector{ m_Position.e032(), m_Position.e013() - offsetDist, m_Position.e021() }
	};
}