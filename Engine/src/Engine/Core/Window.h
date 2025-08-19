#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <string>
#include <functional>

namespace Engine
{
	class Event;

	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;
		bool vsync;
		bool fullScreen;

		WindowProps()
			: title{ "Engine" }
			, width{ 1600 }
			, height{ 900 }
			, vsync{ false }
			, fullScreen{ false }
		{
		}

		explicit WindowProps(const std::string& name)
			: title{ name }
			, width{ 1600 }
			, height{ 900 }
			, vsync{ false }
			, fullScreen{ false }
		{
		}

		explicit WindowProps(const std::string& title, uint32_t width, uint32_t height, bool vsync, bool fullScreen)
			: title{ title }
			, width{ width }
			, height{ height }
			, vsync{ vsync }
			, fullScreen{ fullScreen }
		{
		}
	};

	class Window
	{
	public:

		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual void SetPosition(uint32_t x, uint32_t y) = 0;
		virtual uint32_t GetPositionX() const = 0;
		virtual uint32_t GetPositionY() const = 0;

		virtual void Minimize() = 0;
		virtual void Maximize() = 0;
		virtual void Restore() = 0;
		virtual bool IsMinimized() const = 0;
		virtual bool IsMaximized() const = 0;

		virtual bool IsFocused() const = 0;
		virtual bool IsHovered() const = 0;

		virtual void SetResizable(bool resizable) = 0;
		
		virtual void SetSize(uint32_t width, uint32_t height) = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void SetSizeLimits(
			std::optional<uint32_t> minWidth = std::nullopt,
			std::optional<uint32_t> minHeight = std::nullopt,
			std::optional<uint32_t> maxWidth = std::nullopt,
			std::optional<uint32_t> maxHeight = std::nullopt) = 0;
		virtual void SetAspectRatio(uint32_t numerator, uint32_t denominator) = 0;

		virtual void SetFullscreen(bool fullscreen) = 0;
		virtual bool IsFullscreen() const = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void* GetNativeWindow() const = 0;

		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps{});

	};

}

#endif // !WINDOW_H
