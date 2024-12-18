// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

//使用cmake时，这个macro必须定义，xcode中则不需要，只需要imgui_impl_opengl3中定义此宏即可
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <iostream>

#include "teapot.h"

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

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 300 es"; //version必须匹配
    //EGL也必须在mac上指定，否则glfw提示不存在，能找到EGL lib，意味着从/usr/local/lib中获得
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 300";
//    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,1);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
    
    
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    float teapotRotation = 50;
    bool rotateSync = true;
    int deltaX = -5.0, deltaY = -5.0;
    float deltaZoom = 5.5f;

    Teapot teapot;
    teapot.init();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    
    std::cout << "GL_VERSION                  : " << glGetString( GL_VERSION ) << std::endl;
    std::cout << "GL_VENDOR                   : " << glGetString( GL_VENDOR ) << std::endl;
    std::cout << "GL_RENDERER                 : " << glGetString( GL_RENDERER ) << std::endl;
    std::cout << "GL_SHADING_LANGUAGE_VERSION : " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;
    
    std::cout << "GLFW_Info                 : " << glfwGetVersionString() << std::endl;
    
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
    
    // graphicsEngine->initializeScenes();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
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

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
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
            static float f = 0.0f;
            static int counter = 0;

            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

            glViewport(0, 0, fboWidth, fboHeight);

//            constexpr GLfloat color[]{ 0.2f, 0.3f, 0.5f, 0.8f }, depth(1.0f);
//            glClearBufferfv(GL_COLOR, 0, color);
//            glClearBufferfv(GL_DEPTH, 0, &depth);
//
//            int success;
//            char infoLog[512];
//            // Create a vertex shader object
//            unsigned int vertexShader;
//            vertexShader = glCreateShader(GL_VERTEX_SHADER);
//
//            // Attach the shader source code to the shader object
//            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//
//            // Compile the vertex shader dynamically
//            glCompileShader(vertexShader);
//
//            // Check if compilation was successful
//            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//            if (!success) {
//                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
//                std::cout << infoLog << std::endl;
//            }
//
//            // Create a fragment shader object
//            unsigned int fragmentShader;
//            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//
//            // Attach the shader source code to the shader object
//            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//
//            // Compile the fragment shader dynamically
//            glCompileShader(fragmentShader);
//            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//            if (!success) {
//                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
//                std::cout << infoLog << std::endl;
//            }
//
//
//            // 2. Link shaders
//
//            // Create a shader program
//            unsigned int shaderProgram;
//            shaderProgram = glCreateProgram();
//
//            // Attach the compiled shaders to the shader program
//            glAttachShader(shaderProgram, vertexShader);
//            glAttachShader(shaderProgram, fragmentShader);
//            glLinkProgram(shaderProgram);
//
//            // Check if linking was successful
//            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//            if (!success) {
//                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//                std::cout << "ERROR::PROGRAM::LINKING_FAILED\n";
//                std::cout << infoLog << std::endl;
//            }
//
//            // Delete shader objects if we no longer need them anymore
//            glDeleteShader(vertexShader);
//            glDeleteShader(fragmentShader);
//
//
//            // 3. Set up vertex data and configure vertex attributes
//
//            // Define three vertices with 3D positions
//            float vertices[] = {
//                -0.5f, -0.5f, 0.0f,
//                0.5f, -0.5f, 0.0f,
//                0.0f,  0.5f, 0.0f
//            };
//
//            // Generate vertex buffer object (VBO) and vertex array object (VAO)
//            unsigned int VBO, VAO;
//            glGenVertexArrays(1, &VAO);
//            glGenBuffers(1, &VBO);
//
//            // Bind VAO, then bind VBO
//            glBindVertexArray(VAO);
//            glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//            // Copy the vertex data into the buffer's memory
//            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//            // Set attributes that describe how OpenGL should interpret the vertex data
//            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//            glEnableVertexAttribArray(0);
//
//
//            // Unbind so other calls won't modify VBO and VAO
//            glBindBuffer(GL_ARRAY_BUFFER, 0);
//            glBindVertexArray(0);
//
//            glUseProgram(shaderProgram);
//                    glBindVertexArray(VAO);
//                    glDrawArrays(GL_TRIANGLES, 0, 3);

            //ImDrawList::AddCallback(graphicsEngine->render(),nullptr);bool done = false;


                {
                    ImGui::Begin("Teapot controls");
                    ImGui::SliderFloat("Teapot rotation", &teapotRotation, 0, 2 * 3.14);
                    ImGui::Checkbox("Rotate synchronously", &rotateSync);
                    ImGui::Text("Zoom value: %f", teapot.zoomValue());
                    ImGui::End();
                }



                // Rendering
                //glViewport(0, 0, (int) ImGui::GetIO().DisplaySize.x, (int) ImGui::GetIO().DisplaySize.y);
                glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                teapot.rotateTo(teapotRotation);
                if (rotateSync)
                    teapot.rotateCameraTo(teapotRotation);

                if (!ImGui::IsWindowHovered())
                {
                    if (std::abs(deltaZoom) > 0.001f)
                        teapot.zoomBy(deltaZoom);
                    if ((deltaX != 0) || (deltaY != 0))
                        teapot.rotateCameraBy(deltaX * 0.005f, deltaY * 0.005f);
                }
                teapot.draw();

            
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
