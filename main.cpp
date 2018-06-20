#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "lib/stb_image/stb_image.cpp"

#include <iostream>

#include "lib/Shader/shader.cpp"

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

int getColorModeFromFileExt(const char* path)
{
    const char* extension = path + (strlen(path) - 3);
    int colorMode = strcmp(extension, "png") == 0 ? GL_RGBA : GL_RGB;

    return colorMode;
}

void loadTexture(const char* path, int width, int height, GLenum textureUnit)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);

    /* ------------------- Set texture wrapping and filtering options ------------------- */
    /*  Wrapping options define what the behavior should be when co-ordinates outside the
        texture range are specified. S and T correspond to the X and Y axes respectively, as does
        R to Z if working with 3D textures. */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*  Filtering options define the approximation method when mapping texture co-ordinates,
        usually floating point values, to discrete screen pixels. Filtering options for minifying
        (scaling down) and magnifiying (scaling up) can be defined separately. */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* Load and generate the texture */
    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data)
    {
        int colorMode = getColorModeFromFileExt(path);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    else
    {
        std::cout << "Texture -> Failed to load texture at " << path << std::endl;
    }

    /* Free image memory */
    stbi_image_free(data);
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

    /* Create the rectangle and load its vertices into buffers */
    createRectangle();

    loadTexture("assets/Textures/wood_container.jpg", 512, 512, GL_TEXTURE0);
    loadTexture("assets/Textures/awesome_face.png", 512, 512, GL_TEXTURE1);

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

        /* Scale down to half size, and rotate counter-clockwise by 90 degrees */
        /* (Operations take place in reverse, i.e. "bottom up", due to the non commutative nature of matrix multiplication) */
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(-0.33f, 0.0f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

        myShaders.use();

        /* Pass transformation matrix into vertex shader */
        int transformUniformLocation = glGetUniformLocation(myShaders.shaderProgramID, "transform");
        glUniformMatrix4fv(transformUniformLocation, 1, GL_FALSE, glm::value_ptr(transform));

        /* Render rectangle using the indices in the Element Buffer Object and custom shaders */
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);





        /* Scale down to half size, and rotate counter-clockwise by 90 degrees */
        /* (Operations take place in reverse, i.e. "bottom up", due to the non commutative nature of matrix multiplication) */
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.33f, 0.0f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, -1.0f));
        transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));

        myShaders.use();

        /* Pass transformation matrix into vertex shader */
        glUniformMatrix4fv(transformUniformLocation, 1, GL_FALSE, glm::value_ptr(transform));

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