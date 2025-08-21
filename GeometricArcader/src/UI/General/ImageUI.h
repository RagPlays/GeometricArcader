#ifndef IMAGEUI_H
#define IMAGEUI_H

#include <Engine.h>
#include "UI/General/UI.h"

class ImageUI final
{
public:
	
	ImageUI();
	explicit ImageUI(UIAnchor anchor, const glm::vec2& margin, const glm::vec2& size);
	~ImageUI() = default;

	ImageUI(const ImageUI&) = delete;
	ImageUI(ImageUI&&) noexcept = delete;
	ImageUI& operator=(const ImageUI&) = delete;
	ImageUI& operator=(ImageUI&&) noexcept = delete;

	void OnEvent(Engine::Event& e);
	void Render() const;

	// UI Getters/Setters
	void SetAnchor(UIAnchor anchor);
	UIAnchor GetAnchor() const;

	void SetMargin(const glm::vec2& margin);
	const glm::vec2& GetMargin() const;

	void SetSize(const glm::vec2& size);
	const glm::vec2& GetSize() const;

	void SetTexture(const Engine::Ref<Engine::Texture2D> texture);
	void ClearImage();

private:

	void UpdateRenderPosition();

private:

	const Engine::Window& m_Window;

	Engine::Ref<Engine::Texture2D> m_ImageTexture;

	glm::vec2 m_RenderPosition;
	UIAnchor m_Anchor;
	glm::vec2 m_Margin;
	glm::vec2 m_Size;

};

#endif // !IMAGEUI_H
