#include "SandboxApp.h"

#include "Layers/SandboxMainLayer.h"
#include "Layers/SandboxTestLayer2D.h"
#include "Layers/GeometryShaderLayer.h"

Engine::Application* Engine::CreateApplication(const ApplicationCommandLineArgs& args)
{
	return new SandboxApp{};
}

SandboxApp::SandboxApp()
{
	/*SandboxMainLayer* mainLayer{ new SandboxMainLayer{} };
	AddLayer(mainLayer);*/
	
	/*SandboxTestLayer2D* testLayer2D{ new SandboxTestLayer2D{} };
	AddLayer(testLayer2D);*/

	GeometryShaderLayer* testLayer2D{ new GeometryShaderLayer{} };
	AddLayer(testLayer2D);
}