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

		virtual void SetPosition(uint32_t x, uint32_t y) override;
		virtual uint32_t GetPositionX() const override;
		virtual uint32_t GetPositionY() const override;

		virtual void Minimize() override;
		virtual void Maximize() override;
		virtual void Restore() override;
		virtual bool IsMinimized() const override;
		virtual bool IsMaximized() const override;

		virtual bool IsFocused() const override;
		virtual bool IsHovered() const override;

		virtual void SetResizable(bool resizable) override;
		virtual void SetSize(uint32_t width, uint32_t height) override;
		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;
		virtual void SetSizeLimits(
			std::optional<uint32_t> minWidth = std::nullopt,
			std::optional<uint32_t> minHeight = std::nullopt,
			std::optional<uint32_t> maxWidth = std::nullopt,
			std::optional<uint32_t> maxHeight = std::nullopt) override;

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

		GLFWwindow* m_pWindow;
		std::unique_ptr<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string title;
			uint32_t x;
			uint32_t y;
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