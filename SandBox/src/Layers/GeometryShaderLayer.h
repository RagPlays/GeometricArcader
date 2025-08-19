#ifndef GEOMETRYSHADERLAYER_H
#define GEOMETRYSHADERLAYER_H

#include <Engine.h>

#include "CameraControllers/OrthoCamController.h"

struct TestVertex
{
	glm::vec3 position;
	glm::vec4 color;
};

class GeometryShaderLayer final : public Engine::Layer
{
public:

	GeometryShaderLayer();
	virtual ~GeometryShaderLayer() = default;

	virtual void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& e) override;

private:

	// Basics
	void Update(float deltaTime);
	void Render() const;

private:

	OrthoCamController m_CameraController;

	Engine::Ref<Engine::VertexArray> m_TestVA;
	Engine::Ref<Engine::VertexBuffer> m_TestVB;
	Engine::Ref<Engine::Shader> m_TestShader;

	//glm::vec4 m_SquareColor;
	//float m_TriangleRotationZ;
	////Engine::Scope<Engine::Model> m_TriangleModel;
	////Engine::Scope<Engine::Model> m_SquareModel;
	//Engine::Ref<Engine::Texture2D> m_Texture;
	//Engine::Ref<Engine::Texture2D> m_TransparentTexture;

	//// Shaders
	//Engine::Ref<Engine::Shader> m_BasicShader;
	//Engine::Ref<Engine::Shader> m_FlatColorShader;
	//Engine::Ref<Engine::Shader> m_TextureShader;

};

#endif // !GEOMETRYSHADERLAYER_H
