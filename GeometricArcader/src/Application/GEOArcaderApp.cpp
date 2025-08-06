#include "GEOArcaderApp.h"

#include "Layers/MainLayer.h"

Engine::Application* Engine::CreateApplication(const ApplicationCommandLineArgs& args)
{
	return new GEOArcaderApp{};
}

GEOArcaderApp::GEOArcaderApp()
	: Application{ Engine::WindowProps{ "Geometric Arcader", 1920, 1080, true, false} }
{
	MainLayer* mainLayer{ new MainLayer{}};
	AddLayer(mainLayer);
}

GEOArcaderApp::~GEOArcaderApp()
{
}