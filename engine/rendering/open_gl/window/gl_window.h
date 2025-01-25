#pragma once 
#include "core/window.h"

struct GLFWwindow;

namespace RIZZ {

	class GLWindow : public Window {
	public:
		GLWindow(const WindowProps& props);
		~GLWindow();

		virtual void OnUpdate() override;
		virtual EventHandler<Window, EventArgs>& Handler() override;
		virtual void SetVSync(bool enable) override;
		virtual bool IsVSync() const NOEXCEPT override;
		virtual const char* ContextName() const NOEXCEPT override;
		virtual Size GetSize() const NOEXCEPT override;
		virtual Point GetLocation() const NOEXCEPT override;
		virtual TypeWindowContext GetTypeWindowContext() const NOEXCEPT override;
		
		static GLWindow* TryCast(Window* eventArgs) {
			if (eventArgs && eventArgs->GetTypeWindowContext() == TypeWindowContext::GLFW) {
				return static_cast<GLWindow*>(eventArgs);
			}
			return nullptr;
		}
	private:
		struct WindowData {
			GLWindow* m_ptr_this;
			String m_Title;
			Size m_Size;
			Point m_Location;
			bool m_VSync;
			EventHandler<Window, EventArgs> m_EventHandler;
		} m_Data;

		GLFWwindow* m_Window;

		void Subscribe();
	};


}