#include "GeometryShaderLayer.h"

using namespace Engine;

GeometryShaderLayer::GeometryShaderLayer()
	: Layer{ "TestLayer2D" }
	, m_CameraController{ 5.f, -1.f, 1.f, false }
{
	m_TestShader = Shader::Create(std::string(ASSET_PATH) + "shaders/test.glsl");

	std::vector<TestVertex> vertices =
	{
	{{0.0f, 0.0f, 0.0f}, {1, 0, 0, 1}}, // red
	{{0.5f, 0.0f, 0.0f}, {0, 1, 0, 1}}, // green
	{{-0.5f, 0.0f, 0.0f}, {0, 0, 1, 1}} // blue
	};

	uint32_t indices[3] = { 0, 1, 2 };

	m_TestVA = VertexArray::Create();
	m_TestVB = VertexBuffer::Create(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(TestVertex)));

	m_TestVB->SetLayout({
		{ShaderDataType::Float3, "a_Position"},
		{ShaderDataType::Float4, "a_Color"}
		});

	m_TestVA->AddVertexBuffer(m_TestVB);

	Ref<IndexBuffer> ib = IndexBuffer::Create(indices, 3);
	m_TestVA->SetIndexBuffer(ib);
}

void GeometryShaderLayer::OnUpdate()
{
	const FrameTimer& timer{ FrameTimer::Get() };
	const float deltaTime{ timer.GetSeconds() };

	Update(deltaTime);

	RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.f });
	RenderCommand::Clear();

	Renderer::BeginScene(m_CameraController.GetCamera());
	{
		Render();
	}
	Renderer::EndScene();
}

void GeometryShaderLayer::OnImGuiRender()
{
}

void GeometryShaderLayer::OnEvent(Event& e)
{
	m_CameraController.OnEvent(e);
}

void GeometryShaderLayer::Update(float deltaTime)
{
	m_CameraController.Update();
}

void GeometryShaderLayer::Render() const
{
	m_TestShader->Bind();
	RenderCommand::DrawIndexed(m_TestVA, 0, RenderMode::Points);
}