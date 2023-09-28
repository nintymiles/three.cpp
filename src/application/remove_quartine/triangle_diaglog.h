#ifndef SRC_APPLICATION_REMOVE_QUARTINE_TRIANGLE_DIAGLOG_H_
#define SRC_APPLICATION_REMOVE_QUARTINE_TRIANGLE_DIAGLOG_H_

#include "app_env.h"

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

void drawTriangleDialog(){
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

    
    
    {
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
}


#endif /* SRC_APPLICATION_REMOVE_QUARTINE_TRIANGLE_DIAGLOG_H_ */
