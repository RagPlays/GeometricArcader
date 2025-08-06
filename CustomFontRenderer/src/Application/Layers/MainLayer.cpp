﻿#include "MainLayer.h"

#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "Parser/Font.h"
#include "Renderer/FontRenderer.h"

using namespace Engine;

MainLayer::MainLayer()
	: Layer{ "MainLayer" }
	, m_CameraController{ 2.f, -100.f, 100.f, 1.f, 100000.f, true }
	, m_InputPossible{ true }
	, m_DisplayImGui{ true }
	, m_CurrentFontRenderConfigs{}
{
	m_CameraController.SetZoom(5000.f);

	// Create Font
	m_Font = std::make_shared<Font>(std::string(ASSET_PATH) + "Fonts/JetBrainsMono-Bold.ttf");
	//m_Font = std::make_shared<Font>(std::string(ASSET_PATH) + "Fonts/ArialRoundedMTBold.ttf");

	// Create Font Renderer
	m_FontRenderer = std::make_unique<FontRenderer>(m_Font);
}

MainLayer::~MainLayer()
{
}

void MainLayer::OnAttach()
{
	m_InputPossible = true;
}

void MainLayer::OnUpdate()
{
	Update();
	Render();
}

void MainLayer::OnImGuiRender()
{
	if (!m_DisplayImGui) return;

	bool hoveringImGui{ false };

	static float imGuiFontScale{ 2.f };
	ImGui::Begin("ImGuiConfigs", nullptr, ImGuiWindowFlags_NoMove);
	{
		if (hoveringImGui || ImGui::IsWindowHovered()) hoveringImGui = true;
		ImGui::SetWindowFontScale(imGuiFontScale);
		ImGui::Text("Font Scale");
		ImGui::DragFloat("##fontScale", &imGuiFontScale, 0.5f, 1.f, 5.f, "%.2f");
	}
	ImGui::End();

	const Camera& camera{ m_CameraController.GetCamera() };
	const glm::vec3& position{ camera.GetPosition() };
	const glm::vec3& rotation{ camera.GetRotation() };
	const float cameraZoom{ m_CameraController.GetZoom() };

	ImGui::Begin("CameraInfo", nullptr, ImGuiWindowFlags_NoMove);
	{
		if (hoveringImGui || ImGui::IsWindowHovered()) hoveringImGui = true;
		ImGui::SetWindowFontScale(imGuiFontScale);
		ImGui::Text("Position");
		ImGui::Text("X: %.2f Y: %.2f Z: %.2f", position.x, position.y, position.z);
		ImGui::Text("Rotation");
		ImGui::Text("Z: %.2f", rotation.x, rotation.y, rotation.z);
		ImGui::Text("Zoom");
		ImGui::Text("%.2f", cameraZoom);
	}
	ImGui::End();

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

	ImGui::Begin("RendererStats", nullptr, ImGuiWindowFlags_NoMove);
	{
		if (hoveringImGui || ImGui::IsWindowHovered()) hoveringImGui = true;
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

	ImGui::Begin("FontRendererConfig", nullptr, ImGuiWindowFlags_NoMove);
	{
		if (hoveringImGui || ImGui::IsWindowHovered()) hoveringImGui = true;
		ImGui::SetWindowFontScale(imGuiFontScale);
		ImVec2 configWindowRes{ ImGui::GetContentRegionAvail() };

		ImGui::Text("Input Text");
		ImGui::InputTextMultiline("##inputText", &m_CurrentFontRenderConfigs.inputText, ImVec2{ configWindowRes.x, 80.f });

		ImGui::Text("DEBUG ----------------");

		ImGui::Text("Draw Lines");
		ImGui::Checkbox("##drawDebugLines", &m_CurrentFontRenderConfigs.drawDebugLines);

		ImGui::Text("Draw Points");
		ImGui::Checkbox("##drawDebugPoints", &m_CurrentFontRenderConfigs.drawDebugPoints);

		ImGui::Text("Draw Bounding Box");
		ImGui::Checkbox("##drawDedugBoundingBox", &m_CurrentFontRenderConfigs.drawDebugBoundBox);

		ImGui::Text("Use Bezier Curves");
		ImGui::Checkbox("##useBezierCurves", &m_CurrentFontRenderConfigs.useBezierCurves);

		if (m_CurrentFontRenderConfigs.useBezierCurves)
		{
			int tempBezierResolution{ static_cast<int>(m_CurrentFontRenderConfigs.bezierCurveResolution) };
			if (tempBezierResolution < 0) tempBezierResolution = 0;
			if (ImGui::InputInt("##bezierCurveResolution", &tempBezierResolution))
			{
				if (tempBezierResolution < 0) m_CurrentFontRenderConfigs.bezierCurveResolution = 0;
				else m_CurrentFontRenderConfigs.bezierCurveResolution = static_cast<uint32_t>(tempBezierResolution);
			}
		}

		ImGui::Text("----------------------");
		if (ImGui::Button("Apply", ImVec2{ configWindowRes.x, 0.f }))
		{
			m_FontRenderer->SetRenderConfigs(m_CurrentFontRenderConfigs);
		}
	}
	ImGui::End();

	ImGuiIO& io{ ImGui::GetIO() };
	const bool anyInteraction{ io.WantCaptureKeyboard || io.WantTextInput || io.MouseDrawCursor };
	m_InputPossible = !anyInteraction && !hoveringImGui;
}

void MainLayer::OnEvent(Engine::Event& e)
{
	EventDispatcher dispatcher{ e };
	dispatcher.Dispatch<KeyReleasedEvent>(ENGINE_BIND_EVENT_FN(MainLayer::OnButtonReleased));

	if (m_InputPossible) m_CameraController.OnEvent(e);
}

bool MainLayer::OnButtonReleased(KeyReleasedEvent& e)
{
	if (e.GetKeyCode() == Key::F2)
	{
		m_DisplayImGui = !m_DisplayImGui;
		if (!m_DisplayImGui) m_InputPossible = true;
	}

	return false;
}

void MainLayer::Update()
{
	if (m_InputPossible) m_CameraController.Update();
}

void MainLayer::Render() const
{
	Renderer2D::ResetStats();
	RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.f });
	RenderCommand::Clear();

	// Draw all font characters
	/*constexpr int spaceBetweenCharacters{ 500 };
	Renderer2D::BeginScene(m_CameraController.GetCamera());
	{
		glm::ivec2 offset{};
		const std::vector<GlyphData>& glyphsData{ m_Font->GetGlyphs() };
		for (const auto& glyph : glyphsData)
		{
			m_FontRenderer->DebugRenderGlyph(glyph, offset);
			offset.x += glyph.GetWidth();
		}
	}
	Renderer2D::EndScene();*/

	// Only draw "Missing Glyph"
	/*Renderer2D::BeginScene(m_CameraController.GetCamera());
	{
		m_FontRenderer->DebugRenderGlyph(m_Font->GetGlyphs()[0], {});
	}
	Renderer2D::EndScene();*/

	// Draw ImGui input Text
	Renderer2D::BeginScene(m_CameraController.GetCamera());
	{
		m_FontRenderer->Render();
	}
	Renderer2D::EndScene();
}