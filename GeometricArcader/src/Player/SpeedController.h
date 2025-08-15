#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

#include <Engine.h>

class SpeedController final
{
public:

	enum class ControllerState
	{
		FORWARD,
		NEUTRAL,
		REVERSE
	};

	SpeedController();
	~SpeedController() = default;

	SpeedController(const SpeedController&) = delete;
	SpeedController(SpeedController&&) noexcept = delete;
	SpeedController& operator=(const SpeedController&) = delete;
	SpeedController& operator=(SpeedController&&) noexcept = delete;
	
	void Update();
	void Render() const;

	ControllerState GetCurrentState() const;

private:

	const Engine::Window& m_Window;

	const glm::vec2 m_LeftBotMargin;
	const glm::vec2 m_Size;

	ControllerState m_currentState;
};

#endif // !SPEEDCONTROLLER_H
