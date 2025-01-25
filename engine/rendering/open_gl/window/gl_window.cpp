#include "gl_window.h"
#include "core/event/event_args.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace RIZZ {

	static void GLFWErrorCallback(int error, const char* description)
	{
		RZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	class GLFWContext {
	private:
		bool is_Initialized = false;
	public:
		GLFWContext() {
			if (!is_Initialized) {
				int success = glfwInit();

				glfwSetErrorCallback(GLFWErrorCallback);

				RZ_CORE_ASSERT(success, "Could not initialized GLFW!");
				is_Initialized = true;
			}
		}

		~GLFWContext() {
			if (is_Initialized) {
				glfwTerminate();
			}
		}
	};


	GLWindow::GLWindow(const WindowProps& props)
	{
		m_Data.m_Title = props.m_title;
		m_Data.m_Size = props.m_size;
		m_Data.m_Location = props.m_location;
		m_Data.m_ptr_this = this;

		RZ_CORE_INFO("Window Location: ({0}, {1})", m_Data.m_Location.GetX(), m_Data.m_Location.GetX());
		RZ_CORE_INFO("Create window {0} ({1},{2})", props.m_title, props.m_size.GetWidth(), props.m_size.GetHeight());

		static GLFWContext s_GLFWContext = GLFWContext();

		m_Window = glfwCreateWindow(m_Data.m_Size.GetWidth(), m_Data.m_Size.GetHeight(), m_Data.m_Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RZ_CORE_ASSERT(status, "Failed to initilize Glad!");


		glfwSetWindowUserPointer(m_Window, &m_Data);
		Subscribe();
		SetVSync(m_Data.m_VSync);
		glfwSetWindowPos(m_Window, RIZZ::to_cast<int>(m_Data.m_Location.GetX()), RIZZ::to_cast<int>(m_Data.m_Location.GetY()));
	}


	GLWindow::~GLWindow()
	{
		glfwDestroyWindow(m_Window);
	}


	void GLWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	Size GLWindow::GetSize() const NOEXCEPT
	{
		return m_Data.m_Size;
	}

	Point GLWindow::GetLocation() const NOEXCEPT 
	{
		return m_Data.m_Location;
	}

	void GLWindow::SetVSync(bool enable)
	{
		glfwSwapInterval(enable ? 1 : 0);
		m_Data.m_VSync = enable;
	}

	void RIZZ::GLWindow::Subscribe()
	{
		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.m_Size = Size(width, height);


			int xPos, yPos;
			glfwGetWindowPos(window, &xPos, &yPos);
			data.m_Location = Point(xPos, yPos);

	
			ResizeEvent event(data.m_Location, data.m_Size);
			data.m_EventHandler.Invoke(data.m_ptr_this, event);
			});


		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.m_EventHandler.Invoke(data.m_ptr_this, WindowCloseEvent{});
			});


		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyboardEventArgs event(MapingKeyCodeGLFW(key), InputEventArgs::ActionDevice::Press, MapingKeyCodeGLFW(mods));
					data.m_EventHandler.Invoke(data.m_ptr_this, event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyboardEventArgs event(MapingKeyCodeGLFW(key), InputEventArgs::ActionDevice::Release, MapingKeyCodeGLFW(mods));
					data.m_EventHandler.Invoke(data.m_ptr_this, event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyboardEventArgs event(MapingKeyCodeGLFW(key), InputEventArgs::ActionDevice::Repeat, MapingKeyCodeGLFW(mods));
					data.m_EventHandler.Invoke(data.m_ptr_this, event);
					break;
				}
			}

			});


		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			if (action == GLFW_PRESS) {
				MouseButtonEventArgs event(MapingMouseCodeGLFW(button), InputEventArgs::ActionDevice::Press, MapingKeyCodeGLFW(mods));
				data.m_EventHandler.Invoke(data.m_ptr_this, event);
			}
			else if (action == GLFW_RELEASE) {
				MouseButtonEventArgs event(MapingMouseCodeGLFW(button), InputEventArgs::ActionDevice::Release, MapingKeyCodeGLFW(mods));
				data.m_EventHandler.Invoke(data.m_ptr_this, event);
			}
			else if (action == GLFW_REPEAT) {
				MouseButtonEventArgs event(MapingMouseCodeGLFW(button), InputEventArgs::ActionDevice::Repeat, MapingKeyCodeGLFW(mods));
				data.m_EventHandler.Invoke(data.m_ptr_this, event);
			}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEventArgs event(xOffset, yOffset);
			data.m_EventHandler.Invoke(data.m_ptr_this, event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMoveEventArgs event(xpos, ypos);
			data.m_EventHandler.Invoke(data.m_ptr_this, event);
			});
	}


	EventHandler<Window, EventArgs>& GLWindow::Handler()
	{
		return m_Data.m_EventHandler;
	}

	const char* GLWindow::ContextName() const NOEXCEPT {
		glfwMakeContextCurrent(m_Window); 

		int major = glfwGetWindowAttrib(m_Window, GLFW_CONTEXT_VERSION_MAJOR);
		int minor = glfwGetWindowAttrib(m_Window, GLFW_CONTEXT_VERSION_MINOR);
		int profile = glfwGetWindowAttrib(m_Window, GLFW_OPENGL_PROFILE);

		static std::string contextInfo;
		contextInfo = "OpenGL Version: " + std::to_string(major) + "." + std::to_string(minor);

		if (profile == GLFW_OPENGL_CORE_PROFILE) {
			contextInfo += " (Core Profile)";
		}
		else if (profile == GLFW_OPENGL_COMPAT_PROFILE) {
			contextInfo += " (Compatibility Profile)";
		}
		else {
			contextInfo += " (Unknown Profile)";
		}

		return contextInfo.data();
	}

	bool GLWindow::IsVSync() const NOEXCEPT
	{
		return m_Data.m_VSync;
	}

	Window::TypeWindowContext GLWindow::GetTypeWindowContext() const NOEXCEPT {
		return TypeWindowContext::GLFW;
	}
}

