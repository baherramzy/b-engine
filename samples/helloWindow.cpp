#include <glad/glad.h>
#include <glfw3.h>

/* Define window resize callback to adjust viewport */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    GLFWwindow* window;
    unsigned int shaderProgram;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Register resize callback */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Load OpenGL function pointers from glad lib */
    gladLoadGL();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Clear screen color */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}