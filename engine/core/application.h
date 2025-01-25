#pragma once 

#include "core/window.h"
#include "core/layer_stack.h"


namespace RIZZ {

	class Application {
	public:
		Application();
		Application(const WindowProps& props);
		~Application();


		void Run();
		Window& Context();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void OnEvent(Window* send, EventArgs& e);
		virtual	EventHandler<Window, EventArgs>& Handler();
	private:
		EventHandler<Window, EventArgs> m_EventHandler;
		Pointer<Window> m_window;
		LayerStack m_LayerStack;
		bool m_Running;
	};

}
