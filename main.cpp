#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

/* B defined classes */
#include "lib/Shader/shader.cpp"
#include "lib/Texture/texture.cpp"

/* Define window resize callback to adjust viewport */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

void createRectangle()
{
    float vertices[] = {
         /* Positions */        /* Colors */       /* Texture Co-ords */
         0.75f,  0.75f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,      /* Top Right */
         0.75f, -0.75f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f,      /* Bottom Right */
        -0.75f, -0.75f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f,      /* Bottom Left */
        -0.75f,  0.75f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f       /* Top Left */
    };

    /* Vertex indices to avoid the need to define the same vertex multiple times
    in order to draw two triangles to make up a rectangle */
    unsigned int indices[] = {
        3, 0, 1,
        1, 2, 3,
    };

    /* Generate and bind Vertex Array, Vertex Buffer and Element Buffer Objects */
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    /* Copy vertex data to buffer memory */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* Position attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    /* Color attribute */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    /* Texture Co-ordinate attribute */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);
}

/* Compute the multiplication of the Model - View - Projection matrices */
glm::mat4 getMVPMatrix(const unsigned int width, const unsigned int height)
{
    glm::mat4 model, view, proj;
    model = view = proj = glm::mat4(1.0f);

    /* Rotate rectangle around X axis */
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    /* Simulate moving camera backwards by pushing scene forwards */
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));

    /* Apply perspective transformation */
    proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

    return proj * view * model;
}

int main(void)
{
    GLFWwindow* window;
    const unsigned int width = 800, height = 600;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
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

    /* Create the rectangle and load its vertices into buffers */
    createRectangle();

    Texture myTextures;
    myTextures.load("assets/Textures/wood_container.jpg", 512, 512);
    myTextures.load("assets/Textures/awesome_face.png", 512, 512);

    /* Compile and load shaders */
    Shader myShaders("shaders/v.vert", "shaders/f.frag");
    myShaders.use();

    /* Specify which sampler to load each texture into */
    myShaders.setInt("texture0", 0);
    myShaders.setInt("texture1", 1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Clear screen color */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 mvpMatrix = getMVPMatrix(width, height);

        /* Pass Model View Projection matrix into vertex shader */
        int mvpUniformLoc = glGetUniformLocation(myShaders.shaderProgramID, "mvp");
        glUniformMatrix4fv(mvpUniformLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

        /* Render rectangle using the indices in the Element Buffer Object and custom shaders */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}