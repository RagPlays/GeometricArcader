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
	
	DefaultCamera m_Camera;
	Game m_Game;
};

#endif // !MAINLAYER_H
