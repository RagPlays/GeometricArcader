#ifndef MAINLAYER_H
#define	MAINLAYER_H

#include <Engine.h>

#include "Camera/DefaultCamera.h"
#include "Application/Game.h"

class MainLayer final : public Engine::Layer
{
public:

	MainLayer();
	~MainLayer();

	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;

	virtual void OnEvent(Engine::Event& e) override;

private:

	bool OnKeyReleased(Engine::KeyReleasedEvent& event);

private:
	
	const Engine::Window& m_Window;
	DefaultCamera m_Camera;
	Game m_Game;

	bool m_ShowImgui;
};

#endif // !MAINLAYER_H
