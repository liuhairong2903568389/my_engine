#define GLEW_STATIC
#include<GLEW/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

#include"MyLog.h"
#include"Renderer.h"

void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    };
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    {
        float indices[] = {
        -0.5f,-0.5f, 0.0f, 0.0, 0.0,
         0.5f,-0.5f, 0.0f, 1.0, 0.0,
         0.5f, 0.5f, 0.0f, 1.0, 1.0,
         -0.5f, 0.5f,0.0f, 0.0, 1.0
        };
        unsigned int index[] = {
            0,1,2,
            2,3,0
        };


        my_engine::VertexArray va;
        my_engine::VertexBuffer vb(indices, sizeof(indices));
        my_engine::IndexBuffer ib(index, sizeof(index));
        va.layout_push<float>(3);
        va.layout_push<float>(2);
        va.addLayout();

        my_engine::Shader shader("res/shader/basic.shader");
        my_engine::Texture texture0("res/texture/1.png");
        my_engine::Texture texture1("res/texture/10.jpg");
        CheckGL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        CheckGL(glEnable(GL_BLEND));

        shader.SetUniform1i("mytexture1", 0);
        shader.SetUniform1i("mytexture2", 1);
        texture0.bind();
        texture1.bind(1);
        



        while (!glfwWindowShouldClose(window))
        {
            glfwSwapBuffers(window);

            float time = (cos(glfwGetTime()) + 1.0) / 2;
            shader.SetUniform1f("T_color", time);

            glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            CheckGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void*)0));


            glfwSetKeyCallback(window, key_callback);



            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}