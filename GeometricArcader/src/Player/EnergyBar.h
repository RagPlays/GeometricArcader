#ifndef ENERGYBAR_H
#define ENERGYBAR_H

#include <Engine.h>

enum class UIAnchor
{
	Middle,
	LeftBottom,
	RightBottom,
	LeftTop,
	RightTop
};

class ProgressBar final
{
public:

	explicit ProgressBar(UIAnchor anchor, const glm::vec2& margin, const glm::vec2& size);
	~ProgressBar() = default;

	ProgressBar(const ProgressBar&) = delete;
	ProgressBar(ProgressBar&&) noexcept = delete;
	ProgressBar& operator=(const ProgressBar&) = delete;
	ProgressBar& operator=(ProgressBar&&) noexcept = delete;

	void Render() const;

	void SetValue(float energy);
	float GetValue() const;

	void SetMaxValue(float maxEnergy);
	float GetMaxValue() const;

private:

	const Engine::Window& m_Window;

	float m_Value;
	float m_MaxValue;

	const UIAnchor m_Anchor;
	const glm::vec2 m_Margin;
	const glm::vec2 m_Size;
};

#endif // !ENERGYBAR_H