#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "GLCall.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Limit framerate to VSync/monitor's refresh rate
    glfwSwapInterval(1);

    /* Initialize GLEW */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    // Print OpenGL version
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  
        /* Blend explanation:
        Blending combines the color values of what the fragment shader outputs(source) and the colors already in the screen(destination).
        It does through the following equation (if BlendEquation is set to ADD):

        For every value of RGBA (sourceValue * sourceFactor) + (destinationValue * destinationFactor) = resultingValue

        BlendEquation decides what equation to use to blend the values, the default is adding them, but you can also substract or get te MIN or MAX
        BlendFunc changes the values of sourceFactor and destinationFactor. For example, we use GL_SRC_ALPHA on the sourceFactor 
        and the inverse of it, GL_ONE_MINUS_SRC_ALPHA, in the destinationFactor
        */


        float positions[]{ // unique triangle vertices
            -5.0f,  -5.0f,  0.0f, 0.0f, 0.0f, // 0
             5.0f,  -5.0f,  0.0f, 1.0f, 0.0f, // 1
             5.0f,   5.0f,  0.0f, 1.0f, 1.0f, // 2
            -5.0f,   5.0f,  0.0f, 0.0f, 1.0f // 3
        };

        unsigned int indices[] = { // order of triangle vertices
            0, 1, 2,
            2, 3, 0
        };

        // Create vertex array
        VertexArray va;

        // Create vertex buffer
        VertexBuffer vb(positions, 4 * 5 * sizeof(float) /*four vertices with 5 floats each*/);

        // Create vertex buffer layout
        VertexBufferLayout layout;
        layout.Push<float>(3); // Three floats per vertice coordinate
        layout.Push<float>(2); // Two floats for texture mapping

        // Add buffer and layout to vertex array
        va.AddBuffer(vb, layout);

        // Create index buffer
        IndexBuffer ib(indices, 6);





        // Create shader
        Shader shader = Shader("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");
        shader.Bind();

        // Create texture
        Texture texture("res/textures/bob.png");
        texture.Bind();

        // Send texture slot in an Uniform
        shader.SetUniform1i("u_Texture", 0);
       
        // Unbind everything
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        // Create renderer
        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();


        // IMGUI variables
        float fov = 1.22173f;
        glm::vec3 translation(0.0f, 0.0f, -20.0f);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */   
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Create projection matrix
            glm::mat4 proj = glm::perspective(fov, 4.0f / 3.0f, 0.1f, 100.0f);
            //proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

            // Create view matrix
            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

            // Create model matrix
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);

            // Create model view projection matrix
            glm::mat4 mvp = proj * view * model;

            // Bind our shader
            shader.Bind();

            // Pass projection matrix as an Uniform
            shader.SetUniformMat4f("u_MVP", mvp);

            // Draw triangles
            renderer.Draw(va, ib, shader);


            // IMGUI window
            {
                ImGui::Begin("Hello, world!");

                ImGui::SliderFloat("FOV", &fov, 0.52f, 1.57f);
                ImGui::SliderFloat2("x and y", &translation.x, -10.0f, 10.0f);
                ImGui::SliderFloat("z", &translation.z, -100.0f, -1.0f);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            // IMGUI render thingies
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
