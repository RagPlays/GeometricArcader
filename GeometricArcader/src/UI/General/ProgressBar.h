#ifndef ENERGYBAR_H
#define ENERGYBAR_H

#include <Engine.h>

#include "UI/General/UI.h"

class ProgressBar final
{
public:

	ProgressBar();
	explicit ProgressBar(UIAnchor anchor, const glm::vec2& margin, const glm::vec2& size);
	~ProgressBar() = default;

	ProgressBar(const ProgressBar&) = delete;
	ProgressBar(ProgressBar&&) noexcept = delete;
	ProgressBar& operator=(const ProgressBar&) = delete;
	ProgressBar& operator=(ProgressBar&&) noexcept = delete;

	void OnEvent(Engine::Event& e);
	void Render() const;

	// UI Getters/Setters
	void SetAnchor(UIAnchor anchor);
	UIAnchor GetAnchor() const;

	void SetMargin(const glm::vec2& margin);
	const glm::vec2& GetMargin() const;

	void SetSize(const glm::vec2& size);
	const glm::vec2& GetSize() const;

	void SetBackgroundColor(const glm::vec4& color);
	const glm::vec4& GetBackgroundColor() const;

	void SetOutlineColor(const glm::vec4& color);
	const glm::vec4& GetOutlineColor() const;

	void SetValueColor(const glm::vec4& color);
	const glm::vec4& GetValueColor() const;

	// Value Getters/Setters
	void SetValue(float newValue);
	void AddValue(float value);
	float GetValue() const;
	float GetProgress() const; // Returns the progress as a percentage (0.0 to 1.0)

	void SetMaxValue(float maxValue);
	float GetMaxValue() const;

	bool IsFull() const;

private:

	void UpdateRenderPosition();

private:

	const Engine::Window& m_Window;

	glm::vec2 m_RenderPosition;

	float m_Value;
	float m_MaxValue;

	UIAnchor m_Anchor;
	glm::vec2 m_Margin;
	glm::vec2 m_Size;

	glm::vec4 m_BackgroundColor;
	glm::vec4 m_OutlineColor;
	glm::vec4 m_ValueColor;
};

#endif // !ENERGYBAR_H