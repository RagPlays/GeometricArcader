#include "MainLayer.h"

#include <imgui/imgui.h>

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
	constexpr float imGuiFontScale{ 2.f };

	// timer
	static int nrOfFrames{};
	static float elapsedTime{};
	static float lastFPS{};
	++nrOfFrames;
	elapsedTime += FrameTimer::Get().GetSeconds();
	if (elapsedTime > 1.f)
	{
		lastFPS = static_cast<float>(nrOfFrames) / elapsedTime;
		elapsedTime = 0.f;
		nrOfFrames = 0;
	}

	ImGui::Begin("RendererStats", nullptr);
	{
		ImGui::SetWindowFontScale(imGuiFontScale);
		ImGui::Text("General Stats:");
		ImGui::Text("FPS: %f", lastFPS);

		const auto& stats{ Renderer2D::GetStats() };
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Point Count: %d", stats.pointCount);
		ImGui::Text("Line Count: %d", stats.lineCount);
		ImGui::Text("Quad Count: %d", stats.quadCount);
		ImGui::Text("Circle Count %d", stats.circleCount);
		ImGui::Text("Vertices Count: %d", stats.GetTotalVerticesCount());
		ImGui::Text("Indices Count: %d", stats.GetTotalIndicesCount());
	}
	ImGui::End();
}

void MainLayer::OnEvent(Engine::Event& e)
{
	m_Camera.OnEvent(e);
	m_Game.OnEvent(e);
}