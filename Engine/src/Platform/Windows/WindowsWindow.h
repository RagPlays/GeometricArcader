#ifndef WINDOWSWINDOW_H
#define WINDOWSWINDOW_H

#include <memory>

#include "Engine/Core/Window.h"

struct GLFWwindow;

namespace Engine
{
	class GraphicsContext;

	class WindowsWindow final : public Window
	{
	public:

		explicit WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;
		virtual void SetWindowSize(uint32_t width, uint32_t height) override;

		virtual void SetFullscreen(bool fullscreen) override;
		virtual bool IsFullscreen() const override;

		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual void SetEventCallback(const EventCallbackFn& callback) override;
		virtual void* GetNativeWindow() const override;

	private:

		void Init(const WindowProps& props);
		void InitCallbacks();
		void Shutdown();
		
	private:

		GLFWwindow* m_Window;
		std::unique_ptr<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string title;
			uint32_t width;
			uint32_t height;
			bool vSync;
			bool fullScreen;

			EventCallbackFn eventCallback;
		};

		int m_LastWindowedPosX;
		int m_LastWindowedPosY;
		int m_LastWindowedSizeX;
		int m_LastWindowedSizeY;

		WindowData m_Data;
	};
}

#endif // !WINDOWSWINDOW_H