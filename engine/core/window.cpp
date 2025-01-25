#include "typedefs.h"
#include "window.h"
#include "rendering/open_gl/window/gl_window.h"


namespace RIZZ {
	Pointer<Window> Window::Create(const WindowProps& props)
	{
#ifdef PLATFORM_WINDOWS
		return CreatePointer<GLWindow>(props);
#else
		RZ_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

	
}