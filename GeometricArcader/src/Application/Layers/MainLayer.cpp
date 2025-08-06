#include "MainLayer.h"

using namespace Engine;

MainLayer::MainLayer()
	: Layer{ "MainLayer" }
	, m_Camera{}
	, m_Game{}
{
}

MainLayer::~MainLayer()
{
}

void MainLayer::OnUpdate()
{
	const FrameTimer& timer{ FrameTimer::Get() };
	m_Game.Update(timer.GetSeconds());

	Renderer2D::ResetStats();
	RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.f });
	RenderCommand::Clear();

	Renderer2D::BeginScene(m_Camera.GetCamera());
	{
		m_Game.Render();
	}
	Renderer2D::EndScene();
}

void MainLayer::OnImGuiRender()
{
}

void MainLayer::OnEvent(Engine::Event& e)
{
	m_Camera.OnEvent(e);
}