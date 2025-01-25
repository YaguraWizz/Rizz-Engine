#include <iostream>
#include "RizzEngine.h"
#include <GLFW/glfw3.h>


namespace Editor {

    class ImGuiLeyer : public RIZZ::Layer {
    public:
        ImGuiLeyer(RIZZ::Application& app) : Layer("ImGuiLeyer"), m_app(app) {}

        virtual void OnAttach() override {
            ImGui::CreateContext();
            ImGui::StyleColorsDark();

            ImGuiIO& io = ImGui::GetIO();
            io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
            io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

            ImGui_ImplOpenGL3_Init("#version 410");
        }

        virtual void OnDetach() override {
        

        }

        virtual void OnUpdate() override {
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(RIZZ::to_cast<float>(m_app.Context().GetSize().GetWidth()), RIZZ::to_cast<float>(m_app.Context().GetSize().GetHeight()));
           
            float time = (float)glfwGetTime();
            io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0f / 60.0f);
            m_Time = time;


            ImGui_ImplOpenGL3_NewFrame();
            ImGui::NewFrame();

            static bool show = true;
            ImGui::ShowDemoWindow();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            RZ_INFO("New Frame: {0}", (1000.0f * io.DeltaTime));
        }

        virtual void OnEvent(RIZZ::Window* sender, RIZZ::EventArgs& event) override {
            RIZZ::EventHandler<RIZZ::Window, RIZZ::EventArgs> hendler;

            RZ_INFO(event.ToString());

            hendler.Subscribe<RIZZ::MouseButtonEventArgs>([](RIZZ::Window* sender, RIZZ::MouseButtonEventArgs event) {
                ImGuiIO& io = ImGui::GetIO();

                if (event.GetEventActionDevice() == RIZZ::MouseButtonEventArgs::ActionDevice::Press) {
                    io.MouseDown[RIZZ::to_cast<int>(event.GetMouseCode())] = true;
                    event.Hendler = false;
                }
                else if (event.GetEventActionDevice() == RIZZ::MouseButtonEventArgs::ActionDevice::Release) {
                    io.MouseDown[RIZZ::to_cast<int>(event.GetMouseCode())] = false;
                    event.Hendler = false;
                }
                else {

                }
             
                });

            hendler.Subscribe<RIZZ::MouseMoveEventArgs>([](RIZZ::Window* sender, RIZZ::MouseMoveEventArgs event) {
                ImGuiIO& io = ImGui::GetIO();
                io.MousePos = ImVec2(RIZZ::to_cast<float>(event.GetLocation().GetX()), RIZZ::to_cast<float>(event.GetLocation().GetY()));
                });

            hendler.Subscribe<RIZZ::MouseScrolledEventArgs>([](RIZZ::Window* sender, RIZZ::MouseScrolledEventArgs event) {
                ImGuiIO& io = ImGui::GetIO();
                io.MouseWheel = RIZZ::to_cast<float>(event.GetXOfset());
                io.MouseWheelH = RIZZ::to_cast<float>(event.GetYOfset());
                });

            hendler.Subscribe<RIZZ::KeyboardEventArgs>([](RIZZ::Window* sender, RIZZ::KeyboardEventArgs event) {
                ImGuiIO& io = ImGui::GetIO();

                });

            hendler.Invoke(sender, event);
        }
    private:
     

    private:
        float m_Time = 0.0f;
        RIZZ::Application& m_app;
    };






}



int main() {
    using namespace RIZZ;
    Application app{};

    app.PushLayer(new Editor::ImGuiLeyer(app));

    app.Run();

    return 0;
}