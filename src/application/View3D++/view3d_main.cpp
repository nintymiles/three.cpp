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
#include <tiny_gltf.h>

#include <iostream>
//#include "gl_headers.h"
#include <stdio.h>
//#include <glad/glad.h>
#include <chrono>
#include "glfw_utils.h"

//使用cmake时，这个macro必须定义，xcode中则不需要，只需要imgui_impl_opengl3中定义此宏即可
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "ImGuiFileDialog.h"
#include "common_utils.h"
#include "gl_utils.h"
#include "init_application.h"

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

void drawTriangleDialog();
void drawTriangle(GLuint shaderProgram);


ImGuiIO* appGuiIO;

// ImGui state
bool show_demo_window = false;
bool show_another_window = false;
bool main_window_active = true;



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
    appGuiIO = &io;
    ImGui::StyleColorsDark();

    //const char* glsl_version = "#version 460";
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    printGLDriverInfo(std::cout);

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
    //IM_ASSERT(font != NULL);c
    std::string rootDir = threecpp::getProjectPath();
    std::string fileSeparator = threecpp::getFileSeparator();
    std::string resourceDir = rootDir.append(fileSeparator).append("asset").append(fileSeparator)
            .append("fonts").append(fileSeparator);
#ifdef __APPLE__
    float fontSize = 20.0f;
#else
    float fontSize = 36.0f;
#endif
    ImFont *cFont = io.Fonts->AddFontFromFileTTF((resourceDir+"NotoSansSC-Regular.ttf").c_str(), fontSize, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());


    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glfwGetFramebufferSize(window, &display_w, &display_h);
    framebuffer_size_callback(window, display_w, display_h);

//    // Basic vertex shader in GLSL (OpenGL Shading Language)
//    const char *vertexShaderSource =
//            "#version 300 es\n"
//            "layout (location = 0) in vec3 aPos;\n"
//            "void main() {\n"
//            "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//            "}\0";
//
//    // Basic fragment shader
//    const char *fragmentShaderSource =
//            "#version 300 es\n"
//            "precision highp float; \n"
//            "out vec4 FragColor;\n"
//            "void main() {\n"
//            "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//            "}\0";

    // Basic vertex shader in GLSL (OpenGL Shading Language)
    const char *vertexShaderSourceR = R"*(#version 300 es
            precision highp float;
            layout (location = 0) in vec3 aPos;
            void main() {
                gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
            })*";

// Basic fragment shader
    const char *fragmentShaderSourceR = R"*(#version 300 es
            precision highp float;
            out vec4 FragColor;
            void main() {
                FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
            })*";

    GLuint shaderProgram = glCreateProgram();
    loadAndCompileShader(shaderProgram,vertexShaderSourceR,fragmentShaderSourceR);

    initRenderer();

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


        ShowApplicationMenuBar();

        if (renderClass != nullptr) {

            if (!renderClass->initialized) {
                Color c = renderClass->renderer->getClearColor();
                glClearColor(c.r, c.g, c.b,1.0f);
                renderClass->renderer->clear(true, true, true);
                renderClass->init();
                glfwGetFramebufferSize(window, &display_w, &display_h);
                framebuffer_size_callback(window, display_w, display_h);
                renderClass->initialized = true;
            }

            if (!io.WantCaptureMouse) {
                renderClass->controller->update();
            }
            renderClass->render();

        }

        //ImGui未初始化之前调用，将会出现imgui Assertion failed: g.WithinFrameScope错误
        if (renderClass != nullptr) {
            renderClass->showControls();
            // save boundTextures before ImGui Render
            //currentBoundTextures = renderClass->renderer->state->currentBoundTextures;
        }

//        glfwGetFramebufferSize(window, &display_w, &display_h);
//        framebuffer_size_callback(window, display_w, display_h);


//        //main menu bar
//        if (ImGui::BeginMainMenuBar())
//        {
//            if (ImGui::BeginMenu("File"))
//            {
//                if (ImGui::MenuItem("Open..", "Ctrl+O")) {
//                    IGFD::FileDialogConfig config;
//                    config.path = ".";
//                    config.sidePaneWidth = 256.f;
//                    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*", config);
//                }
//                if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
//                if (ImGui::MenuItem("Close", "Ctrl+W"))  { main_window_active = false; }
//                ImGui::EndMenu();
//            }
//            ImGui::EndMainMenuBar();
//        }

        //drawTriangle();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // display
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
                std::cout << "filePath = " << filePath << " | name=" << filePathName << std::endl;

                std::string fileName = filePath + threecpp::getFileSeparator() + filePathName;
                renderClass->loadObj(fileName);
            }



            // close
            ImGuiFileDialog::Instance()->Close();
        }

//        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
//        {
//            static float f = 0.0f;
//            static int counter = 0;
//
//            static const int app_address_buf_size = 1024;
//            char appAddress[app_address_buf_size] = {};
//
//            /**
//             * 在IMGui中实现独占充满主窗口，比如填满glfw窗口。 https://github.com/ocornut/imgui/issues/933
//             */
//            ImGuiViewport* viewport = ImGui::GetMainViewport();
//            ImGui::SetNextWindowPos(viewport->Pos);
//            ImGui::SetNextWindowSize(viewport->Size);
//            ImGui::Begin("main window", &main_window_active, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize );
//
//            //ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//            // Create a window called "My First Tool", with a menu bar.
//            if (main_window_active) {
//                ImGui::Begin("My Main Window", &main_window_active, ImGuiWindowFlags_MenuBar);
//                if (ImGui::BeginMenuBar()) {
//                    if (ImGui::BeginMenu("File")) {
//                        if (ImGui::MenuItem("Open..", "Ctrl+O")) {
//                            IGFD::FileDialogConfig config;
//                            config.path = ".";
//                            config.sidePaneWidth = 256.f;
//                            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*", config);
//                        }
//                        if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
//                        if (ImGui::MenuItem("Close", "Ctrl+W")) { main_window_active = false; }
//                        ImGui::EndMenu();
//                    }
//                    ImGui::EndMenuBar();
//                }
//
//                ImGui::Text(
//                        "This is some useful text.");               // Display some text (you can use a format strings too)
//
//                // open Dialog Simple
//                if (ImGui::Button("Open File Dialog")) {
//                    IGFD::FileDialogConfig config;
//                    config.path = ".";
//                    config.sidePaneWidth = 256.f;
//                    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".*", config);
//                }
//


//                ImGui::InputText("App Address", appAddress, app_address_buf_size);
//                //std::cout << appAddress << std::endl;
//                ImGui::LogText(appAddress);
//
//                ImGui::Checkbox("Another Window", &show_another_window);
//                ImGui::Checkbox("Demo Window", &show_demo_window);
//
//                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//                ImGui::ColorEdit3("clear color", (float *) &clear_color); // Edit 3 floats representing a color
//
//                if (ImGui::Button(
//                        "Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//                    counter++;
//                ImGui::SameLine();
//                ImGui::Text("counter = %d", counter);
//
//                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
//                            ImGui::GetIO().Framerate);
//                ImGui::End();
//            }
//        }
//
//        {
//            //绘制三角形窗口
        //drawTriangleDialog();
//        }
//
//        // 3. Show another simple window.
//        if (show_another_window)
//        {
//            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//            ImGui::Text("Hello from another window!");
//            if (ImGui::Button("Close Me"))
//                show_another_window = false;
//            ImGui::End();
//        }


        //此处直接执行OpenGL绘制到当前FrameBuffer可能在ImGui::Render()调用时被清理
        //drawTriangleO();
        //drawTriangleDialog();

        //Rendering
        //此处ImGui frame准备完毕，最终ImGui绘制数据完成
        ImGui::Render();

        //绘制前设置viewport
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glfwGetWindowContentScale(window, &display_x_scale, &display_y_scale);
        glViewport(0, 0, display_w, display_h);
        //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);

        //若直接绘制OpenGL到当前framebuffer,则此处绘制较合理
        //drawTriangleO();
        //glViewport((display_w - 640)/2, (display_h - 640)/2, 640,640);
        //drawTriangle(shaderProgram);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

    }

    // Cleanup
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glfwDestroyWindow(window);
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
    if (renderClass == nullptr) return;
    renderClass->controller->sizeChanged(Vector4(0, 0, width, height));

    if (renderClass != nullptr)
        renderClass->renderer->setViewport(0, 0, display_w, display_h);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (renderClass == nullptr) return;
    renderClass->controller->mouseMove((int)xpos, (int)ypos);
}

void mouse_button_callback(GLFWwindow* window, int button,int action,int modes) {

    if (renderClass == nullptr) return;
    if (appGuiIO->WantCaptureMouse) return;
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
            renderClass->controller->mouseDown(0, (int)x, (int)y);//ROTATE
        }else if(rbutton_down){
            renderClass->controller->mouseDown(1, (int)x, (int)y);//DOLLY
        }else {
            renderClass->controller->mouseDown(2, (int) x, (int) y); //PAN
        }

    }
    else {
        renderClass->controller->mouseUp();
    }
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if (renderClass == nullptr) return;
    //std::cout << "mouse wheel delta: " << yoffset << std::endl;
    renderClass->controller->mouseWheel((float)(yoffset*120));
}

void drawTriangleDialog(){
    // Basic vertex shader in GLSL (OpenGL Shading Language)
    const char *vertexShaderSource =
            "#version 300 es\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main() {\n"
            "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";

// Basic fragment shader
    const char *fragmentShaderSource =
            "#version 300 es\n"
            "precision highp float;"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0";

    constexpr int fboWidth(800);
    constexpr int fboHeight(640);

    GLuint framebuffer;
    GLuint depthRenderbuffer;
    GLuint texture;
    // GLint texWidth = 480,
    // texHeight = 320;
    GLint maxRenderbufferSize;

    glGetIntegerv ( GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
    // check if GL_MAX_RENDERBUFFER_SIZE is >= texWidth and texHeight

    if ( ( maxRenderbufferSize <= fboWidth ) || ( maxRenderbufferSize <= fboHeight ) ) {
        // cannot use framebuffer objects, as we need to create
        // a depth buffer as a renderbuffer object
        // return with appropriate error

    }

    // generate the framebuffer, renderbuffer, and texture object names
    glGenFramebuffers (1, &framebuffer);
    glGenRenderbuffers (1, &depthRenderbuffer);
    glGenTextures (1, &texture);

    // bind texture and load the texture mip level 0
    // texels are RGB565
    // no texels need to be specified as we are going to draw into
    // the texture
    glBindTexture ( GL_TEXTURE_2D, texture );
    glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, fboWidth, fboHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL );

    glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );

    glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glTexParameteri ( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // bind renderbuffer and create a 16-bit depth buffer
    // width and height of renderbuffer = width and height of
    // the texture
    glBindRenderbuffer ( GL_RENDERBUFFER, depthRenderbuffer );
    glRenderbufferStorage ( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, fboWidth, fboHeight );

    // bind the framebuffer
    glBindFramebuffer ( GL_FRAMEBUFFER, framebuffer );

    // specify texture as color attachment
    glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0 );
    // specify depth_renderbuffer as depth attachment
    glFramebufferRenderbuffer ( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    glBindFramebuffer ( GL_FRAMEBUFFER, 0 );


    static float f = 0.0f;
    static int counter = 0;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glViewport(0, 0, fboWidth, fboHeight);

    constexpr GLfloat color[]{ 0.2f, 0.3f, 0.5f, 0.8f }, depth(1.0f);
    glClearBufferfv(GL_COLOR, 0, color);
    glClearBufferfv(GL_DEPTH, 0, &depth);

    int success;
    char infoLog[512];
    // Create a vertex shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach the shader source code to the shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    // Compile the vertex shader dynamically
    glCompileShader(vertexShader);

    // Check if compilation was successful
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
        std::cout << infoLog << std::endl;
    }

    // Create a fragment shader object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach the shader source code to the shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    // Compile the fragment shader dynamically
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
        std::cout << infoLog << std::endl;
    }


    // 2. Link shaders

    // Create a shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // Attach the compiled shaders to the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check if linking was successful
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n";
        std::cout << infoLog << std::endl;
    }

    // Delete shader objects if we no longer need them anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // 3. Set up vertex data and configure vertex attributes

    // Define three vertices with 3D positions
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    // Generate vertex buffer object (VBO) and vertex array object (VAO)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO, then bind VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy the vertex data into the buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set attributes that describe how OpenGL should interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Unbind so other calls won't modify VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    //ImDrawList::AddCallback(graphicsEngine->render(),nullptr);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui::Begin("OpenGL");                          // Create a window called "Hello, world!" and append into it.
    ImGui::SetWindowSize(ImVec2(fboWidth, fboHeight+60));

    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Image((void*)(intptr_t)texture, ImVec2(fboWidth, fboHeight),ImVec2(0, 1), ImVec2(1, 0));

    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::End();
    //ImGui::Render();

}

void drawTriangle(GLuint shaderProgram){

    constexpr GLfloat color[]{ 0.2f, 0.3f, 0.5f, 0.8f }, depth(1.0f);
    glClearBufferfv(GL_COLOR, 0, color);
    glClearBufferfv(GL_DEPTH, 0, &depth);


    // 3. Set up vertex data and configure vertex attributes

    // Define three vertices with 3D positions
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };

    // Generate vertex buffer object (VBO) and vertex array object (VAO)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind VAO, then bind VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy the vertex data into the buffer's memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set attributes that describe how OpenGL should interpret the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // Unbind so other calls won't modify VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

}


