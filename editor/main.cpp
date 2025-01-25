#include <iostream>
#include "RizzEngine.h"


namespace Editor {

    class UILeyer : public RIZZ::Layer {
    public:
        UILeyer() : Layer("UI Leyer") {}

        void OnUpdate() override {
            RZ_INFO("UI::OnUpdate");
        }

        void OnEvent(RIZZ::Window* sender, RIZZ::EventArgs& event) override {
            RZ_INFO("{0}", event.ToString());
        }
    };

}






int main() {
    using namespace RIZZ;
    Application app{};

    app.PushLayer(new Editor::UILeyer());

    app.Run();

    return 0;
}