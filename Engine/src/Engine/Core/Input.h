#ifndef INPUT_H
#define INPUT_H

#include "Engine/Core/Base.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"

namespace Engine
{
	class Input
	{
	public:

		virtual ~Input() = default;

		inline static bool IsKeyPressed(KeyCode key) { return s_pInstance->IsKeyPressedImpl(key); }
		inline static bool IsMouseButtonPressed(MouseCode button) { return s_pInstance->IsMouseButtonPressedImpl(button); }
		inline static glm::vec2 GetMousePosition() { return s_pInstance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_pInstance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_pInstance->GetMouseYImpl(); }

	protected:

		Input() = default;

		virtual bool IsKeyPressedImpl(KeyCode key) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual glm::vec2 GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:

		static Input* s_pInstance;

	};
}

#endif // !INPUT_H