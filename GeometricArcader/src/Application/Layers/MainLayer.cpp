#include "MainLayer.h"

#include "FlyFish.h"

MainLayer::MainLayer()
	: Layer{ "MainLayer" }
{
	ENGINE_INFO("MainLayer created");
}

MainLayer::~MainLayer()
{
	TriVector testTriVector{};
}

void MainLayer::OnAttach()
{
}

void MainLayer::OnUpdate()
{
	Update();
	Render();
}

void MainLayer::OnImGuiRender()
{
}

void MainLayer::OnEvent(Engine::Event& e)
{
}

void MainLayer::Update()
{
}

void MainLayer::Render() const
{
}