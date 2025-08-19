#include "ProgressBar.h"

using namespace Engine;

ProgressBar::ProgressBar()
	: ProgressBar{ UIAnchor::Middle, glm::vec2{}, glm::vec2{ 200.f, 50.f } }
{
}

ProgressBar::ProgressBar(UIAnchor anchor, const glm::vec2& margin, const glm::vec2& size)
	: m_Window{ Application::Get().GetWindow() }
    , m_RenderPosition{}
	, m_Value{}
	, m_MaxValue{ 100.f } // Default Max Value
	, m_Anchor{ anchor }
	, m_Margin{ margin }
	, m_Size{ size }
    , m_BackgroundColor{ Color::darkGray }
    , m_OutlineColor{ Color::lightGray }
	, m_ValueColor{ Color::green }
{
    UpdateRenderPosition();
}

void ProgressBar::OnEvent(Event& e)
{
    if (e.IsInCategory(EventCategoryApplication))
    {
        if(e.GetEventType() == EventType::WindowResize)
        {
            const WindowResizeEvent& resizeEvent{ static_cast<const WindowResizeEvent&>(e) };
			UpdateRenderPosition();
		}
    }
}

void ProgressBar::Render() const
{
    constexpr float barRenderLayer{ 500.f }; // max layer is 1000.f
    constexpr float barOffset{ 10.f };       // offset for the energy bar

    const float valueRatio{ m_Value / m_MaxValue };
    const glm::vec2 fullBarSize{ m_Size.x - barOffset, m_Size.y - barOffset };
    const glm::vec2 valueBarSize{ fullBarSize.x * valueRatio, fullBarSize.y };
    const glm::vec2 halfSize{ m_Size * 0.5f };

    const glm::vec2 barRenderPos
    {
        m_RenderPosition.x - halfSize.x + (barOffset * 0.5f) + (valueBarSize.x * 0.5f),
        m_RenderPosition.y
    };

    // Draw outline
    Renderer2D::SetDrawColor(m_OutlineColor);
    Renderer2D::DrawFilledRect(glm::vec3{ m_RenderPosition, barRenderLayer }, m_Size);

    // Draw background
    Renderer2D::SetDrawColor(m_BackgroundColor);
    Renderer2D::DrawFilledRect(glm::vec3{ m_RenderPosition, barRenderLayer + 1 }, fullBarSize);

    // Draw value bar
    Renderer2D::SetDrawColor(m_ValueColor);
    Renderer2D::DrawFilledRect(glm::vec3{ barRenderPos, barRenderLayer + 2 }, valueBarSize);
}

void ProgressBar::SetAnchor(UIAnchor anchor)
{
	m_Anchor = anchor;
    UpdateRenderPosition();
}

UIAnchor ProgressBar::GetAnchor() const
{
    return m_Anchor;
}

void ProgressBar::SetMargin(const glm::vec2& margin)
{
	m_Margin = margin;
    UpdateRenderPosition();
}

const glm::vec2& ProgressBar::GetMargin() const
{
	return m_Margin;
}

void ProgressBar::SetSize(const glm::vec2& size)
{
	m_Size = size;
	UpdateRenderPosition();
}

const glm::vec2& ProgressBar::GetSize() const
{
	return m_Size;
}

void ProgressBar::SetBackgroundColor(const glm::vec4& color)
{
	m_BackgroundColor = color;
}

const glm::vec4& ProgressBar::GetBackgroundColor() const
{
	return m_BackgroundColor;
}

void ProgressBar::SetOutlineColor(const glm::vec4& color)
{
	m_OutlineColor = color;
}

const glm::vec4& ProgressBar::GetOutlineColor() const
{
	return m_OutlineColor;
}

void ProgressBar::SetValueColor(const glm::vec4& color)
{
	m_ValueColor = color;
}

const glm::vec4& ProgressBar::GetValueColor() const
{
	return m_ValueColor;
}

void ProgressBar::SetValue(float newValue)
{
	m_Value = newValue;
    m_Value = std::clamp(m_Value, 0.f, m_MaxValue);
}

void ProgressBar::AddValue(float value)
{
	m_Value += value;
    m_Value = std::clamp(m_Value, 0.f, m_MaxValue);
}

float ProgressBar::GetValue() const
{
	return m_Value;
}

float ProgressBar::GetProgress() const
{
	return (m_MaxValue > 0.f) ? (m_Value / m_MaxValue) : 0.f; // Avoid division by zero
}

void ProgressBar::SetMaxValue(float maxValue)
{
    m_MaxValue = maxValue;
}

float ProgressBar::GetMaxValue() const
{
    return m_MaxValue;
}

bool ProgressBar::IsFull() const
{
	return m_Value >= m_MaxValue;
}

void ProgressBar::UpdateRenderPosition()
{
    const glm::vec2 halfSize{ m_Size * 0.5f };
    const glm::vec2 winSize{ static_cast<float>(m_Window.GetWidth()), static_cast<float>(m_Window.GetHeight()) };
    const glm::vec2 winHalfSize{ winSize * 0.5f };

    switch (m_Anchor)
    {
    case UIAnchor::Middle:
        m_RenderPosition = m_Margin;
        return;

    case UIAnchor::LeftBottom:
        m_RenderPosition = m_Margin + halfSize - winHalfSize;
        return;

    case UIAnchor::LeftTop:
        m_RenderPosition = glm::vec2{ 0.f, winSize.y } + glm::vec2{ m_Margin.x, -m_Margin.y } + glm::vec2{ halfSize.x, -halfSize.y } - winHalfSize;
        return;

    case UIAnchor::RightBottom:
        m_RenderPosition = glm::vec2{ winSize.x, 0.f } + glm::vec2{ -m_Margin.x, m_Margin.y } + glm::vec2{ -halfSize.x, halfSize.y } - winHalfSize;
        return;

    case UIAnchor::RightTop:
        m_RenderPosition = winSize - m_Margin - halfSize - winHalfSize;
        return;

    default:
        ENGINE_ASSERT_MSG(false, "Invalid UIAnchor type");
		m_RenderPosition = glm::vec2{}; // Fallback, should never happen
        return; 
    }
}