#ifndef MAINLAYER_H
#define	MAINLAYER_H

#include <Engine.h>

#include "Camera/DefaultCamera.h"

class Game;

class MainLayer final : public Engine::Layer
{
public:

	MainLayer();
	~MainLayer();

	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;

	virtual void OnEvent(Engine::Event& e) override;

private:

	// Window Events //
	bool OnWindowResized(Engine::WindowResizeEvent& event);
	bool OnKeyReleased(Engine::KeyReleasedEvent& event);

private:
	
	Engine::Window& m_Window;
	const unsigned int m_MinWidth;
	const unsigned int m_MinHeight;

	DefaultCamera m_Camera;
	std::unique_ptr<Game> m_pGame;

	bool m_ShowImgui;
	bool m_RestartGameRequest;
};

#endif // !MAINLAYER_H
