// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
//#define IMGUI_IMPL_OPENGL_ES3
#include "imgui_impl_opengl3.h"

#define TINYGLTF_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
//#include "gl_headers.h"
#include <stdio.h>
//#include <glad/glad.h>
#include <chrono>
#include "glfw_utils.h"

//使用cmake时，这个macro必须定义，xcode中则不需要，只需要imgui_impl_opengl3中定义此宏即可
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h> // Will drag system OpenGL headers


#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// settings
int display_w=1102;
int display_h=999;
float display_x_scale=2.0f;
float display_y_scale=2.0f;

GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

ImGuiIO* demoIO;

int main(){
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit())
        return 1;

    const char* glsl_version = "#version 300 es"; //version必须匹配
    //EGL也必须在mac上指定，否则glfw提示不存在，能找到EGL lib，意味着从/usr/local/lib中获得
//    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,1);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
//    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//    const char* glsl_version = "#version 300 es";

    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0); //目前angle最高可以指定到opengl es 3.1,mac上只能到3.0

//    const char* glsl_version = "#version 460";
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 3
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // 3
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(1280,720, "View3D++", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 0;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetScrollCallback(window, mouse_scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return 0;
//    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    demoIO = &io;
    ImGui::StyleColorsDark();

    //const char* glsl_version = "#version 460";
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    printGLDriverInfo(std::cout);
    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            static const int app_address_buf_size = 1024;
            char appAddress[app_address_buf_size] = {};

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            
            ImGui::InputText("App Address",appAddress,app_address_buf_size);
            //std::cout << appAddress << std::endl;
            ImGui::LogText(appAddress);
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        
        {
            //绘制三角形窗口
            //drawTriangleDialog();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    //glViewport(0, 0, width, height);
    display_w = width;
    display_h = height;
//    if (currentDemoClass == nullptr) return;
//    currentDemoClass->controller->sizeChanged(Vector4(0, 0, width, height));
//
//    if (currentDemoClass != nullptr)
//        currentDemoClass->renderer->setViewport(0, 0, display_w, display_h);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
//    if (currentDemoClass == nullptr) return;
//    currentDemoClass->controller->mouseMove((int)xpos, (int)ypos);
}

void mouse_button_callback(GLFWwindow* window, int button,int action,int modes) {

//    if (currentDemoClass == nullptr) return;
    if (demoIO->WantCaptureMouse) return;
    bool lbutton_down = false;
    bool rbutton_down = false;
    bool mbutton_down = false;
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT :
            if (GLFW_PRESS == action)
                lbutton_down = true;
            else
                lbutton_down = false;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT :
            if (GLFW_PRESS == action)
                rbutton_down = true;
            else
                rbutton_down = false;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            if (GLFW_PRESS == action)
                mbutton_down = true;
            else
                mbutton_down = false;
            break;
    }

    if (lbutton_down || rbutton_down || mbutton_down) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (lbutton_down) {
//            currentDemoClass->controller->mouseDown(0, (int)x, (int)y);//ROTATE
        }else if(rbutton_down){
//            currentDemoClass->controller->mouseDown(1, (int)x, (int)y);//DOLLY
        }else {
//            currentDemoClass->controller->mouseDown(2, (int) x, (int) y); //PAN
        }

    }
    else {
//        currentDemoClass->controller->mouseUp();
    }
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
//    if (currentDemoClass == nullptr) return;
//    //std::cout << "mouse wheel delta: " << yoffset << std::endl;
//    currentDemoClass->controller->mouseWheel((float)(yoffset*120));
}

