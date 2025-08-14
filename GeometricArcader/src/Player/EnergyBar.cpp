#include "EnergyBar.h"

using namespace Engine;

EnergyBar::EnergyBar(float maxEnergy)
	: m_Window{ Application::Get().GetWindow() }
	, m_CurrentEnergy{ 0.f }
	, m_MaxEnergy{ maxEnergy }
	, m_LeftBotMargin{ 30.f, 30.f }
	, m_Size{ 500.f, 50.f }
{
}

void EnergyBar::Render() const
{
	constexpr float barRenderLayer{ 900.f }; // max layer is 1000.f
	constexpr float barOffset{ 10.f }; // offset for the energy bar
	const float energyRatio{ m_CurrentEnergy / m_MaxEnergy };

	const glm::vec2 fullBarSize
	{
		(m_Size.x - barOffset),
		(m_Size.y - barOffset)
	};

	const glm::vec2 valueBarSize
	{
		fullBarSize.x * energyRatio,
		fullBarSize.y
	};

	const glm::vec2 renderPos // centerposition of the background bar
	{
		m_LeftBotMargin.x + (m_Size.x * 0.5f) - (m_Window.GetWidth() * 0.5f),
		m_LeftBotMargin.y + (m_Size.y * 0.5f) - (m_Window.GetHeight() * 0.5f),
	};

	const glm::vec2 barRenderPos
	{
		renderPos.x - (m_Size.x * 0.5f) + (barOffset * 0.5f) + (valueBarSize.x * 0.5f),
		renderPos.y
	};

	Renderer2D::SetDrawColor(Color::lightGray);
	Renderer2D::DrawFilledRect(glm::vec3{ renderPos, barRenderLayer }, m_Size);
	Renderer2D::SetDrawColor(Color::darkGray);
	Renderer2D::DrawFilledRect(glm::vec3{ renderPos, barRenderLayer + 1 }, fullBarSize);

	Renderer2D::SetDrawColor(glm::vec4{ 1.f - energyRatio, energyRatio, 0.f, 1.f });
	Renderer2D::DrawFilledRect(glm::vec3{ barRenderPos, barRenderLayer + 2 }, valueBarSize);
}

void EnergyBar::SetEnergy(float energy)
{
	m_CurrentEnergy = energy;
}

float EnergyBar::GetEnergy() const
{
	return m_CurrentEnergy;
}