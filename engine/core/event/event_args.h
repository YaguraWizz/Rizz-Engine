#pragma once
#include "macro.h"
#include "event.h"
#include "input/input_code.h"

namespace RIZZ {
    namespace detail {
        template<typename T>
        const char* type_name() {
            return typeid(T).name();  
        }
    }

    class EmptyEventArgs : public EventArgs {
    public:
        GENERATE_STATIC_METHOD(EventType, StaticEventType, EventType::Empty)
        GENERATE_GETTER(EventType, EventType, StaticEventType())
        GENERATE_OVERRIDE_METHOD(bool, isEmpty, () const NOEXCEPT, true)
        GENERATE_OVERRIDE_METHOD(const char*, GetName, () const NOEXCEPT, "EmptyEventArgs")

        static EmptyEventArgs* TryCast(EventArgs* eventArgs) {
            if (eventArgs && eventArgs->GetEventType() == StaticEventType()) {
                return static_cast<EmptyEventArgs*>(eventArgs);
            }
            return nullptr;
        }
    private:
        friend EventArgs;
        static EmptyEventArgs& GetInstance() {
            static EmptyEventArgs instance;
            return instance;
        }
    };

    template<typename ...Args>
    class UserEventArgs : public EventArgs {
    private:
        std::tuple<Args...> m_data;
    public:
        GENERATE_STATIC_METHOD(EventType, StaticEventType, EventType::UserEvent)
        GENERATE_GETTER(EventType, EventType, StaticEventType())
        GENERATE_OVERRIDE_METHOD(bool, isEmpty, () const NOEXCEPT, (sizeof...(Args) == 0))

        static UserEventArgs<Args...>* TryCast(EventArgs* eventArgs) {
            if (eventArgs && eventArgs->GetEventType() == StaticEventType()) {
                return static_cast<UserEventArgs<Args...>*>(eventArgs);
            }
            return nullptr;
        }

        UserEventArgs(Args&&... args)
            : m_data(std::forward<Args>(args)...) {
        }

        template<size_t N>
        auto get() -> decltype(std::get<N>(m_data)) {
            auto&& value = std::get<N>(m_data);
            return value;
        }


        virtual const char* GetName() const NOEXCEPT override {
            static std::string name = GetClassName();
            return name.c_str();
        }

    private:
        const char* GetClassName() const {
            static std::string name = TypeNameHelper<Args...>();
            return name.c_str();
        }

        template<typename... Ts>
        std::string TypeNameHelper() const {
            std::string result = "UserEventArgs<";
            ((result += typeid(Ts).name() + std::string(", ")), ...);
            result.back() = '>';  
            return result;
        }
    };


#pragma region [Input Event Args]
    class InputEventArgs : public EventArgs {
    public:
        GENERATE_STATIC_METHOD(EventType, StaticEventType, EventType::InputEvents)
        GENERATE_GETTER(EventType, EventType, StaticEventType())
                 
        static InputEventArgs* TryCast(EventArgs* eventArgs) {
            if (eventArgs && eventArgs->GetEventType() == StaticEventType()) {
                return static_cast<InputEventArgs*>(eventArgs);
            }
            return nullptr;
        }

        enum class InputDevice {
            Mouse,
            Keyboard
        };
        virtual InputDevice GetInputDevice() const NOEXCEPT = 0;

        enum class ActionDevice
        {
            Release, Press, Repeat
        };    
    };

#pragma region [Mouse Event Args]
    class MouseEventArgs : public InputEventArgs {
    public:
        GENERATE_STATIC_METHOD(InputDevice, StaticEventDevice, InputDevice::Mouse)
        GENERATE_GETTER(InputDevice, InputDevice, StaticEventDevice())

        static MouseEventArgs* TryCast(EventArgs* eventArgs) {
            InputEventArgs* inputEvent = InputEventArgs::TryCast(eventArgs);
            if (inputEvent && inputEvent->GetInputDevice() == StaticEventDevice()) {
                return static_cast<MouseEventArgs*>(inputEvent);
            }
            return nullptr;
        }

        enum class MouseAction {
            Button,
            Move,
            Scroll,
        };
        virtual MouseAction GetMouseAction() const NOEXCEPT = 0;
    };


    class MouseMoveEventArgs : public MouseEventArgs {
    public:
        GENERATE_STATIC_METHOD(MouseAction, StaticMouseAction, MouseAction::Move)
        GENERATE_GETTER(MouseAction, MouseAction, StaticMouseAction())
        GENERATE_OVERRIDE_METHOD(bool, isEmpty, () const NOEXCEPT,  m_position.isEmpty())
        GENERATE_OVERRIDE_METHOD(const char*, GetName, () const NOEXCEPT, "MouseMoveEventArgs")

        static MouseMoveEventArgs* TryCast(EventArgs* eventArgs) {
            MouseEventArgs* mouseEventArgs = MouseEventArgs::TryCast(eventArgs);
            if (mouseEventArgs && mouseEventArgs->GetMouseAction() == StaticMouseAction()) {
                return static_cast<MouseMoveEventArgs*>(mouseEventArgs);
            }
            return nullptr;
        }

        MouseMoveEventArgs(Point position) NOEXCEPT
            : m_position(position) {
        }  
        MouseMoveEventArgs(double xpos, double ypos) NOEXCEPT 
            : m_position(xpos, ypos) { 
        }

        GENERATE_GETTER(Point, Location, m_position)
        GENERATE_SETTER(Point, Location, m_position)
    private:
        RIZZ::Point m_position;
    };

    class MouseScrolledEventArgs : public MouseEventArgs {
    public:
        GENERATE_STATIC_METHOD(MouseAction, StaticMouseAction, MouseAction::Scroll)
        GENERATE_GETTER(MouseAction, MouseAction, StaticMouseAction())
        GENERATE_OVERRIDE_METHOD(bool, isEmpty, () const NOEXCEPT, false)
        GENERATE_OVERRIDE_METHOD(const char*, GetName, () const NOEXCEPT, "MouseScrolledEventArgs")

        static MouseScrolledEventArgs* TryCast(EventArgs* eventArgs) {
            MouseEventArgs* mouseEventArgs = MouseEventArgs::TryCast(eventArgs);
            if (mouseEventArgs && mouseEventArgs->GetMouseAction() == StaticMouseAction()) {
                return static_cast<MouseScrolledEventArgs*>(mouseEventArgs);
            }
            return nullptr;
        }

        MouseScrolledEventArgs(double xOfset, double yOfset) NOEXCEPT : m_xOfset(xOfset), m_yOfset(yOfset) {}
        
        GENERATE_GETTER(double, XOfset, m_xOfset)
        GENERATE_SETTER(double, XOfset, m_xOfset)

        GENERATE_GETTER(double, YOfset, m_yOfset)
        GENERATE_SETTER(double, YOfset, m_yOfset)
    private:
        double m_xOfset;
        double m_yOfset;
    };


    class MouseButtonEventArgs : public MouseEventArgs {
    public:
        GENERATE_STATIC_METHOD(MouseAction, StaticMouseAction, MouseAction::Button)
        GENERATE_GETTER(MouseAction, MouseAction, StaticMouseAction())
        GENERATE_OVERRIDE_METHOD(bool, isEmpty, () const NOEXCEPT, m_isEmpty)
        GENERATE_OVERRIDE_METHOD(const char*, GetName, () const NOEXCEPT, "MouseButtonEventArgs")

        static MouseButtonEventArgs* TryCast(EventArgs* eventArgs) {
            MouseEventArgs* mouseEventArgs = MouseEventArgs::TryCast(eventArgs);
            if (mouseEventArgs && mouseEventArgs->GetMouseAction() == StaticMouseAction()) {
                return static_cast<MouseButtonEventArgs*>(mouseEventArgs);
            }
            return nullptr;
        }
        MouseButtonEventArgs(MouseCode button, ActionDevice action, KeyCode mods) NOEXCEPT
            : m_button(button), m_action(action), m_mods(mods), m_isEmpty(true) {}

        GENERATE_GETTER(MouseCode, MouseCode, m_button)
        GENERATE_GETTER(ActionDevice, EventActionDevice, m_action)
        GENERATE_GETTER(KeyCode, MouseMods, m_mods)

        GENERATE_SETTER(MouseCode, MouseCode, m_button)
        GENERATE_SETTER(ActionDevice, EventActionDevice, m_action)
        GENERATE_SETTER(KeyCode, MouseMods, m_mods)
    private:
        MouseCode m_button;
        ActionDevice m_action;
        KeyCode m_mods;
        bool m_isEmpty;
    };
#pragma endregion
#pragma region [Keyboard Event Args]
    class KeyboardEventArgs : public InputEventArgs {
    public:
        GENERATE_STATIC_METHOD(InputDevice, StaticEventDevice, InputDevice::Keyboard)
        GENERATE_GETTER(InputDevice, InputDevice, StaticEventDevice())
        GENERATE_OVERRIDE_METHOD(bool, isEmpty, () const NOEXCEPT, false)
        GENERATE_OVERRIDE_METHOD(const char*, GetName, () const NOEXCEPT, "KeyboardEventArgs")

        static KeyboardEventArgs* TryCast(EventArgs* eventArgs) {
            InputEventArgs* keyboardEventArgs = InputEventArgs::TryCast(eventArgs);
            if (keyboardEventArgs && keyboardEventArgs->GetInputDevice() == StaticEventDevice()) {
                return static_cast<KeyboardEventArgs*>(keyboardEventArgs);
            }
            return nullptr;
        }

        KeyboardEventArgs(KeyCode key, ActionDevice action, KeyCode mods)
            : m_key(key), m_action(action), m_mods(mods) {}

        GENERATE_GETTER(KeyCode, KeyCode, m_key)
        GENERATE_GETTER(ActionDevice, EventActionDevice, m_action)
        GENERATE_GETTER(KeyCode, KeyMods, m_mods)

        GENERATE_SETTER(KeyCode, KeyCode, m_key)
        GENERATE_SETTER(ActionDevice, EventActionDevice, m_action)
        GENERATE_SETTER(KeyCode, KeyMods, m_mods)
    private:
        KeyCode m_key;
        ActionDevice m_action;
        KeyCode m_mods;
    };
#pragma endregion
#pragma endregion


#pragma region [WindowEvents]
    class WindowEvents : public EventArgs {
    public:
        GENERATE_STATIC_METHOD(EventType, StaticEventType, EventType::WindowEvents)
        GENERATE_GETTER(EventType, EventType, StaticEventType())
        static WindowEvents* TryCast(EventArgs* eventArgs) {
            if (eventArgs && eventArgs->GetEventType() == StaticEventType()) {
                return static_cast<WindowEvents*>(eventArgs);
            }
            return nullptr;
        }

        enum class WindowEventCategory {
            Resize, Close
        };
        virtual WindowEventCategory GetEventCategory() const NOEXCEPT = 0;
    };

    class ResizeEvent : public WindowEvents {
    public:
        GENERATE_STATIC_METHOD(WindowEventCategory, StaticEventCategory, WindowEventCategory::Resize)
        GENERATE_GETTER(WindowEventCategory, EventCategory, StaticEventCategory())
        GENERATE_OVERRIDE_METHOD(bool, isEmpty, () const NOEXCEPT, m_Location.isEmpty() && m_Size.isEmpty())
        GENERATE_OVERRIDE_METHOD(const char*, GetName, () const NOEXCEPT, "ResizeEvent")

        static ResizeEvent* TryCast(EventArgs* eventArgs) {
            WindowEvents* resizeEventArgs = WindowEvents::TryCast(eventArgs);
            if (resizeEventArgs && resizeEventArgs->GetEventCategory() == StaticEventCategory()) {
                return static_cast<ResizeEvent*>(resizeEventArgs);
            }
            return nullptr;
        }
        
        explicit ResizeEvent(Point location, Size  size) NOEXCEPT
            : m_Location(location), m_Size(size) {
        }

    private:
        Point m_Location;
        Size m_Size;
    };

    class WindowCloseEvent : public WindowEvents {
    public:
        GENERATE_STATIC_METHOD(WindowEventCategory, StaticEventCategory, WindowEventCategory::Close)
        GENERATE_GETTER(WindowEventCategory, EventCategory, StaticEventCategory())
        GENERATE_OVERRIDE_METHOD(bool, isEmpty, () const NOEXCEPT, (true))
        GENERATE_OVERRIDE_METHOD(const char*, GetName, () const NOEXCEPT, "WindowCloseEvent")

        static ResizeEvent* TryCast(EventArgs* eventArgs) {
            WindowEvents* resizeEventArgs = WindowEvents::TryCast(eventArgs);
            if (resizeEventArgs && resizeEventArgs->GetEventCategory() == StaticEventCategory()) {
                return static_cast<ResizeEvent*>(resizeEventArgs);
            }
            return nullptr;
        }
    };
#pragma endregion

#pragma region [AppliactionEvents]
    class AppliactionEvents : public EventArgs {
    public:
        GENERATE_STATIC_METHOD(EventType, StaticEventType, EventType::ApplicationEvents)
        GENERATE_GETTER(EventType, EventType, StaticEventType())
        static AppliactionEvents* TryCast(EventArgs* eventArgs) {
            if (eventArgs && eventArgs->GetEventType() == StaticEventType()) {
                return static_cast<AppliactionEvents*>(eventArgs);
            }
            return nullptr;
        }
    };
#pragma endregion



   

}