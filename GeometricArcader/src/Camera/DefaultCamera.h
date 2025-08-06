#ifndef DEFAULTCAMERA_H
#define DEFAULTCAMERA_H

#include "Engine/Renderer/OrthographicCamera.h"

namespace Engine
{
	class Event;
	class WindowResizeEvent;
}

class DefaultCamera final
{
public:

	DefaultCamera();
	~DefaultCamera() = default;

	void OnEvent(Engine::Event& e);

	const Engine::OrthographicCamera& GetCamera() const;

private:

	bool OnWindowResized(Engine::WindowResizeEvent& e);
	void UpdateProjection();

private:

	Engine::OrthographicCamera m_Camera;

};

#endif // !DEFAULTCAMERA_H