#ifndef ENDSCREEN_H
#define ENDSCREEN_H

#include "GameStates/IGameState.h"

class EndScreen final : public IGameState
{
public:

	explicit EndScreen(uint32_t score);
	~EndScreen() = default;

	EndScreen(const EndScreen&) = delete;
	EndScreen(EndScreen&&) noexcept = delete;
	EndScreen& operator=(const EndScreen&) = delete;
	EndScreen& operator=(EndScreen&&) noexcept = delete;

	virtual void OnEvent(Engine::Event& e) override;
	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	virtual bool IsComplete() const override;
	virtual GameStateType NextState() const override;

private:

	const Engine::Window& m_Window;

	const uint32_t m_Score;

	Engine::Ref<Engine::Texture2D> m_BackgroundTexture;
	bool m_PressedEnter;

};

#endif // !ENDSCREEN_H