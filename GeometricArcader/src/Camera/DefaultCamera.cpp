#include "DefaultCamera.h"

#include <Engine.h>

using namespace Engine;

DefaultCamera::DefaultCamera()
	: m_Camera{ -1000.f, 1000.f }
{
	UpdateProjection();
}

void DefaultCamera::OnEvent(Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(DefaultCamera::OnWindowResized));
}

bool DefaultCamera::OnWindowResized(WindowResizeEvent& e)
{
	UpdateProjection();
    return false;
}

const OrthographicCamera& DefaultCamera::GetCamera() const
{
	return m_Camera;
}

void DefaultCamera::UpdateProjection()
{
	const Window& appWindow{ Application::Get().GetWindow() };
	const float windowWidth{ static_cast<float>(appWindow.GetWidth()) };
	const float windowHeight{ static_cast<float>(appWindow.GetHeight()) };

	const float halfWinWidth{ windowWidth * 0.5f };
	const float halfWinHeight{ windowHeight * 0.5f };

	m_Camera.SetProjection(-halfWinWidth, halfWinWidth, -halfWinHeight, halfWinHeight);
}