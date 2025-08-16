#include "EnergyBar.h"

using namespace Engine;

ProgressBar::ProgressBar(UIAnchor anchor, const glm::vec2& margin, const glm::vec2& size)
	: m_Window{ Application::Get().GetWindow() }
	, m_Value{}
	, m_MaxValue{ 100.f } // Default Max Value
	, m_Anchor{ anchor }
	, m_Margin{ margin }
	, m_Size{ size }
{
}

void ProgressBar::Render() const
{
    constexpr float barRenderLayer{ 900.f }; // max layer is 1000.f
    constexpr float barOffset{ 10.f };       // offset for the energy bar
    const float energyRatio{ m_Value / m_MaxValue };

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

    // Determine base position based on anchor
    glm::vec2 basePos{};

    switch (m_Anchor)
    {
    case UIAnchor::Middle:
        basePos.x = m_Window.GetWidth() * 0.5f;
        basePos.y = m_Window.GetHeight() * 0.5f;
        break;

    case UIAnchor::LeftBottom:
        basePos.x = 0.f;
        basePos.y = 0.f;
        break;

    case UIAnchor::RightBottom:
        basePos.x = m_Window.GetWidth();
        basePos.y = 0.f;
        break;

    case UIAnchor::LeftTop:
        basePos.x = 0.f;
        basePos.y = m_Window.GetHeight();
        break;

    case UIAnchor::RightTop:
        basePos.x = m_Window.GetWidth();
        basePos.y = m_Window.GetHeight();
        break;
    default:
        break;
    }

    // Now adjust basePos by margins and size so we get the *center* of the background bar
    glm::vec2 renderPos{};

    // Horizontal alignment
    if (m_Anchor == UIAnchor::Middle)
        renderPos.x = m_Margin.x;
    else if (m_Anchor == UIAnchor::LeftBottom || m_Anchor == UIAnchor::LeftTop)
        renderPos.x = basePos.x + m_Margin.x + (m_Size.x * 0.5f) - (m_Window.GetWidth() * 0.5f);
    else // right-anchored
        renderPos.x = basePos.x - m_Margin.x - (m_Size.x * 0.5f) - (m_Window.GetWidth() * 0.5f);

    // Vertical alignment
    if (m_Anchor == UIAnchor::Middle)
        renderPos.y = m_Margin.y;
	else if (m_Anchor == UIAnchor::LeftBottom || m_Anchor == UIAnchor::RightBottom)
        renderPos.y = basePos.y + m_Margin.y + (m_Size.y * 0.5f) - (m_Window.GetHeight() * 0.5f);
    else // top-anchored
        renderPos.y = basePos.y - m_Margin.y - (m_Size.y * 0.5f) - (m_Window.GetHeight() * 0.5f);

    glm::vec2 barRenderPos
    {
        renderPos.x - (m_Size.x * 0.5f) + (barOffset * 0.5f) + (valueBarSize.x * 0.5f),
        renderPos.y
    };

    // Draw background
    Renderer2D::SetDrawColor(Color::lightGray);
    Renderer2D::DrawFilledRect(glm::vec3{ renderPos, barRenderLayer }, m_Size);

    // Draw full bar background
    Renderer2D::SetDrawColor(Color::darkGray);
    Renderer2D::DrawFilledRect(glm::vec3{ renderPos, barRenderLayer + 1 }, fullBarSize);

    // Draw value bar
    Renderer2D::SetDrawColor(glm::vec4{ 1.f - energyRatio, energyRatio, 0.f, 1.f });
    Renderer2D::DrawFilledRect(glm::vec3{ barRenderPos, barRenderLayer + 2 }, valueBarSize);
}

void ProgressBar::SetValue(float energy)
{
	m_Value = energy;
}

float ProgressBar::GetValue() const
{
	return m_Value;
}

void ProgressBar::SetMaxValue(float maxEnergy)
{
}

float ProgressBar::GetMaxValue() const
{
	return 0.0f;
}
