#pragma once
#include "macro.h"
#include "event/event.h"
#include "typedefs.h"


namespace RIZZ {

	struct WindowProps {
		String m_title;
		Size m_size;
		Point m_location;
		WindowProps(const String& title = "Rizz Engine", unsigned int xpos = 100, unsigned int ypos = 100, unsigned int wigth = 1280, unsigned int height = 720)
			: m_title(title), m_size(wigth, height), m_location(xpos, ypos) {
		}

		WindowProps(Point location, Size size, const String& title = "Rizz Engine")
			: m_title(title), m_size(size), m_location(location) {
		}
	};


	class Window {
	public:
		~Window() = default;
		enum class TypeWindowContext
		{
			None, GLFW, 
		};


		virtual void OnUpdate() = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const NOEXCEPT = 0;
		virtual	EventHandler<Window, EventArgs>& Handler() = 0;
		virtual const char* ContextName() const NOEXCEPT = 0;
		virtual Size GetSize() const NOEXCEPT = 0;
		virtual Point GetLocation() const NOEXCEPT = 0;
		virtual TypeWindowContext GetTypeWindowContext() const NOEXCEPT = 0;

		static Pointer<Window> Create(const WindowProps& props = WindowProps());


		template <typename T>
		static T* to_cast(Window* eventArgs) {
			static_assert(std::is_base_of<Window, T>::value, "Error to cast. T must be derived from EventArgs");
			if (!eventArgs) {
				return nullptr;
			}
			return T::TryCast(eventArgs);
		}

		template <>
		static Window* to_cast(Window* eventArgs) {
			if (!eventArgs) {
				return nullptr;
			}
			return static_cast<Window*>(eventArgs);
		}
	};

} 