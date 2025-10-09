#include "renderer.h"
#include "window.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "layermenu.h"
#include "layerclearcolor.h"
#include "layertexture.h"
#include "layerbcifs.h"

int main() {
    Window window;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigDockingTransparentPayload = true;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.window(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Renderer renderer;
    renderer.initOpenGL();
    renderer.setClearColor(0.45f, 0.55f, 0.6f, 1.0f);

    LayerMenu layerMenu;
    layerMenu.registerLayer<LayerClearColor>("Clear Color");
    layerMenu.registerLayer<LayerTexture>("Texture");
    layerMenu.registerLayer<LayerBcifs>("BC-IFS");

    while (!window.shouldClose()) {
        renderer.clear();

        // ImGui stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_AutoHideTabBar);
        ImGui::ShowDemoWindow();

        layerMenu.onRender();
        layerMenu.onUpdate(0.0f);
        layerMenu.onImGuiRender();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffers();
        window.pollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}