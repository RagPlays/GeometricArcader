#include "Pillar.h"

#include "FlyFishTools/FlyFishUtils.h"

using namespace Engine;

Pillar::Pillar(const TriVector& position)
	: m_Position{ position }
	, m_Size{ 20.f, 20.f }
{
}

void Pillar::Render() const
{
	Renderer2D::SetDrawColor(Color::blue);
	FlyFishUtils::DrawFillRect(m_Position, m_Size);
}

const TriVector& Pillar::GetPosition() const
{
	return m_Position;
}