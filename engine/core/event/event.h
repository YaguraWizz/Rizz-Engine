#pragma once
#include "macro.h"
#include "typedefs.h"


namespace RIZZ {
    



    enum class EventType {
        None = 0,           // empty type
        Empty,              // empty type             
        UserEvent,          // user base type
        InputEvents,        // input base type
        WindowEvents,       // window-related events
        ApplicationEvents,  // application-wide events
    };

    class EventArgs {
    public:
        bool Hendler = false;
        virtual ~EventArgs() = default;
        virtual const char* GetName() const NOEXCEPT = 0;
        virtual std::string ToString() const NOEXCEPT { return GetName(); }
        virtual EventType GetEventType() const NOEXCEPT = 0;
        virtual bool isEmpty() const NOEXCEPT = 0;


        template <typename T>
        static T* to_cast(EventArgs* eventArgs) {
            static_assert(std::is_base_of<EventArgs, T>::value, "Error to cast. T must be derived from EventArgs");
            if (!eventArgs) {
                return nullptr;
            }

            return T::TryCast(eventArgs);
        }

        template <>
        static EventArgs* to_cast(EventArgs* eventArgs) {
            if (!eventArgs) {
                return nullptr;
            }
            return static_cast<EventArgs*>(eventArgs);
        }

        static EventArgs* Empty;
    };


    template <typename Sender, typename EventCategory>
    class EventHandler {
    public:
        template <typename Event>
        void Subscribe(std::function<void(Sender*, Event&)> callback) {
            static_assert(std::is_base_of<RIZZ::EventArgs, Event>::value, "EventType must inherit from EventArgs");
            callbacks.emplace_back([callback](Sender* sender, RIZZ::EventArgs& event) {
                if (Event* event_args = event.to_cast<Event>(&event); event_args) {
                    callback(sender, *event_args);
                    return;
                }
                else if (auto empty_event = RIZZ::EmptyEventArgs::TryCast(&event); empty_event) {
                    RZ_CORE_ERROR("Empty event received");
                }
                });
        }

        void Invoke(Sender* sender, RIZZ::EventArgs& event) {
            for (const auto& callback : callbacks) {
                callback(sender, event);
                if (event.Hendler) { break; }
            }
        }

    private:
        using CallbackWrapper = std::function<void(Sender*, RIZZ::EventArgs&)>;
        std::vector<CallbackWrapper> callbacks;
    };
 


}