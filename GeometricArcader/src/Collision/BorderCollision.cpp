#include "BorderCollision.h"

using namespace Engine;

BorderCollision::BorderCollision(float borderOffset)
	: m_borderOffset{ borderOffset }
{
}

void BorderCollision::OnEvent(Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(BorderCollision::OnWindowResized));
}

bool BorderCollision::OnWindowResized(WindowResizeEvent& e)
{
	return false;
}