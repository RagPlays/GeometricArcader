#ifndef ENERGYBAR_H
#define ENERGYBAR_H

#include <Engine.h>

class EnergyBar final
{
public:

	explicit EnergyBar(float maxEnergy);
	~EnergyBar() = default;

	EnergyBar(const EnergyBar&) = delete;
	EnergyBar(EnergyBar&&) noexcept = delete;
	EnergyBar& operator=(const EnergyBar&) = delete;
	EnergyBar& operator=(EnergyBar&&) noexcept = delete;

	void Render() const;

	void SetEnergy(float energy);
	float GetEnergy() const;

private:

	const Engine::Window& m_Window;

	float m_CurrentEnergy;
	const float m_MaxEnergy;

	const glm::vec2 m_LeftBotMargin;
	const glm::vec2 m_Size;
};

#endif // !ENERGYBAR_H