#include <glad/glad.h>
#include <glfw3.h>
#include <math.h>

#include <iostream>

/* Define window resize callback to adjust viewport */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int loadShaders()
{
    /* Define shader source code */
    const char *vertexShaderSource = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0f);\n"
    "   vertexColor = aColor;\n"
    "}\0";

    const char *fragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "in vec3 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(vertexColor, 1.0f);\n"
    "}\n\0";

    /* Load and compile vertex shader source */
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    /* Load and compile fragment shader source  */
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    /* Create, compile, link and execute combined shader program */
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    /* Remove unecessary shader objects now that they're linked */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void createTriangle()
{
    float vertices[] = {
        /* Positions */          /* Colors */
        -0.75f, -0.75f, 0.0f,    1.0f, 0.0f, 0.0f,
         0.75f, -0.75f, 0.0f,    0.0f, 1.0f, 0.0f,
         0.0f,  0.75f,  0.0f,    0.0f, 0.0f, 1.0f
    };

    /* Generate and bind a Vertex Array and a Vertex Buffer Object */
    /* Vertex Array Objects allow for easier state management across multiple objects */
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    /* Copy vertex data to buffer memory */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Map vertex attributes to vertex data in the vertex buffer */
    /* Vertex attributes and locations defined by the shaders */

    /* Applies to the data currently bound to GL_ARRAY_BUFFER, currently VBO */
    /* <x, y, z> => 3 positional float values per vertex */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
    glEnableVertexAttribArray(0);

    /* <r, g, b> => 3 color float values per vertex */
    /* Offset of 3 floats denotes the position of the first color attribute in memory */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    /*                  Memory currently looks as follows:                       */
    /* |       VERTEX 1        |       VERTEX 2        |       VERTEX 3        | */
    /* | X | Y | Z | R | G | B | X | Y | Z | R | G | B | X | Y | Z | R | G | B | */
    /* Stride length (i.e. distance between attribute values) is now 6 floats */

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

    /* Create the triangle and load its vertices into buffers */
    createTriangle();

    /* Compile and load shaders */
    shaderProgram = loadShaders();

    /* Activate shader only once since attributes don't change */
    glUseProgram(shaderProgram);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Clear screen color */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Render triangle using custom shaders */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}