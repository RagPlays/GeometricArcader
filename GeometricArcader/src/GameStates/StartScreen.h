#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include "GameStates/IGameState.h"

class StartScreen final : public IGameState
{
public:

	StartScreen();
	~StartScreen() = default;

	StartScreen(const StartScreen&) = delete;
	StartScreen(StartScreen&&) noexcept = delete;
	StartScreen& operator=(const StartScreen&) = delete;
	StartScreen& operator=(StartScreen&&) noexcept = delete;

	virtual void OnEvent(Engine::Event& e) override;
	virtual void Update(float deltaTime) override;
	virtual void Render() const override;

	virtual bool IsComplete() const override;
	virtual GameStateType NextState() const override;

private:

	const Engine::Window& m_Window;

	bool m_ConfirmedStart;

	Engine::Ref<Engine::Texture2D> m_BackgroundTexture;

};

#endif // !STARTSCREEN_H