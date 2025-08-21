#include "ImageUI.h"

using namespace Engine;

ImageUI::ImageUI()
	: ImageUI{ UIAnchor::Middle, {}, { 100.f, 100.f } }
{
}

ImageUI::ImageUI(UIAnchor anchor, const glm::vec2& margin, const glm::vec2& size)
	: m_Window{ Application::Get().GetWindow() }
	, m_ImageTexture{}
	, m_RenderPosition{}
	, m_Anchor{ anchor }
	, m_Margin{ margin }
	, m_Size{ size }
{
    UpdateRenderPosition();
}

void ImageUI::OnEvent(Event& e)
{
    if (e.IsInCategory(EventCategoryApplication))
    {
        if (e.GetEventType() == EventType::WindowResize)
        {
            const WindowResizeEvent& resizeEvent{ static_cast<const WindowResizeEvent&>(e) };
            UpdateRenderPosition();
        }
    }
}

void ImageUI::Render() const
{
    constexpr float renderLayer{ 500.f }; // max layer is 1000.f, min is -1000.f
    Renderer2D::DrawTexture(m_ImageTexture, glm::vec3{ m_RenderPosition, renderLayer }, m_Size);
}

void ImageUI::SetAnchor(UIAnchor anchor)
{
	m_Anchor = anchor;
    UpdateRenderPosition();
}

UIAnchor ImageUI::GetAnchor() const
{
	return m_Anchor;
}

void ImageUI::SetMargin(const glm::vec2& margin)
{
	m_Margin = margin;
    UpdateRenderPosition();
}

const glm::vec2& ImageUI::GetMargin() const
{
	return m_Margin;
}

void ImageUI::SetSize(const glm::vec2& size)
{
	m_Size = size;
    UpdateRenderPosition();
}

const glm::vec2& ImageUI::GetSize() const
{
	return m_Size;
}

void ImageUI::SetTexture(const Engine::Ref<Engine::Texture2D> texture)
{
	m_ImageTexture = texture;
}

void ImageUI::ClearImage()
{
	m_ImageTexture = nullptr;
}

void ImageUI::UpdateRenderPosition()
{
    const glm::vec2 winSize{ static_cast<float>(m_Window.GetWidth()), static_cast<float>(m_Window.GetHeight()) };
    const glm::vec2 winHalfSize{ winSize * 0.5f };
    const glm::vec2 halfSize{ m_Size * 0.5f };

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