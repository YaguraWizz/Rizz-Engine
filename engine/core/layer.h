#pragma once
#include "event/event_args.h"
#include "window.h"


namespace RIZZ {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Window* sender, EventArgs& event) {}
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};


}