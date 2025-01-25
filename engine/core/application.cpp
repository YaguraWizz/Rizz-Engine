#include "application.h"
#include "event/event_args.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1,  std::placeholders::_2)

namespace RIZZ {
	
	Application::Application()
	{
		RIZZ::Logger::Initialized();
		RZ_CORE_INFO("Logger initialized");

		m_window = RIZZ::Window::Create();
		RZ_CORE_INFO("Create window: {0}", m_window->ContextName());

		m_window->Handler().Subscribe<EventArgs>(BIND_EVENT_FN(Application::OnEvent));
		m_Running = true;
	}
	

	Application::Application(const WindowProps& props) {
		RIZZ::Logger::Initialized();
		RZ_CORE_INFO("Logger initialized");

		m_window = RIZZ::Window::Create(props);
		RZ_CORE_INFO("Create window: {0}", m_window->ContextName());

		m_window->Handler().Subscribe<EventArgs>(BIND_EVENT_FN(Application::OnEvent));
		m_Running = true;
	}


	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Window* sender, EventArgs& event) {
		// the events from the windows come first to the layers and then to the user
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(sender, event);
			if (event.Hendler) { break; }
		}
		m_EventHandler.Invoke(sender, event);
	}


	Application::~Application()
	{
		RZ_CORE_INFO("Destroying the application");
	}

	Window& Application::Context() {
		return *m_window;
	}

	EventHandler<Window, EventArgs>& Application::Handler()
	{
		return m_EventHandler;
	}

	void Application::Run()
	{
		RZ_CORE_INFO("Run application");
		while (m_Running) {

			for (const auto& lauer : m_LayerStack) {
				lauer->OnUpdate();
			}

			m_window->OnUpdate();
		}
	}
}

