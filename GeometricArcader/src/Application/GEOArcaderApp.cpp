#include "GEOArcaderApp.h"

#include "MainLayer.h"

Engine::Application* Engine::CreateApplication(const ApplicationCommandLineArgs& args)
{
	return new GEOArcaderApp{};
}

GEOArcaderApp::GEOArcaderApp()
	: Application{ Engine::WindowProps{ "Geometric Arcader", 1920, 1080, true, false } }
{
	ENGINE_TRACE("GEOArcaderApp Created");

	AddLayer(new MainLayer{}); // Ownership to LayerManager
}

GEOArcaderApp::~GEOArcaderApp()
{
	ENGINE_TRACE("GEOArcaderApp Destroyed");
}