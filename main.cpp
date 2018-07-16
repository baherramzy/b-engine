#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

/* B defined classes */
#include "lib/Shader/shader.cpp"
#include "lib/Texture/texture.cpp"
#include "lib/Camera/camera.cpp"

const unsigned int width = 800, height = 600;
Camera camera;

float frameDeltaTime = 0.0f;
float lastFrameTimestamp = 0.0f;

float lastFrameMouseX = width  / 2;
float lastFrameMouseY = height / 2;
bool isFirstMouseMovement = true;

/* Define window resize callback to adjust viewport */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/* Keyboard event handler */
void handleKeyboardEvents(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float currentFrameTimestamp = glfwGetTime();
    frameDeltaTime = currentFrameTimestamp - lastFrameTimestamp;
    lastFrameTimestamp = currentFrameTimestamp;

    float cameraSpeed = 5.0f * frameDeltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveBackward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveLeft(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRight(cameraSpeed);
}

void mouse_movement_callback(GLFWwindow *window, double xPos, double yPos)
{
    if (isFirstMouseMovement)
    {
        lastFrameMouseX = xPos;
        lastFrameMouseY = yPos;
        isFirstMouseMovement = false;
    }

    /*
    To compute direction vector:
        - Calculate offset since last frame
        - Add offset values to the camera's yaw / pitch values
        - Constrain max and min yaw / pitch values
        - Compute front vector using Euler angles and trigonometry
    */

   float sensitivity = 0.05f;
   float xOffset = (xPos - lastFrameMouseX) * sensitivity;
   float yOffset = (lastFrameMouseY - yPos) * sensitivity;

   lastFrameMouseX = xPos;
   lastFrameMouseY = yPos;

   camera.addYaw(xOffset);
   camera.addPitch(yOffset);

   glm::vec3 mouseFront;
   float cameraPitch = camera.getPitch(), cameraYaw = camera.getYaw();

   mouseFront.x = cos(glm::radians(cameraPitch)) * cos(glm::radians(cameraYaw));
   mouseFront.y = sin(glm::radians(cameraPitch));
   mouseFront.z = cos(glm::radians(cameraPitch)) * sin(glm::radians(cameraYaw));

   camera.setFront(glm::normalize(mouseFront));
}

void defineTriangle()
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

void defineRectangle()
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

void defineCube()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    /* Generate and bind Vertex Array, Vertex Buffer and Element Buffer Objects */
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    /* Copy vertex data to buffer memory */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
glm::mat4 getMVPMatrix(glm::vec3 startPos)
{
    glm::mat4 model, view, proj;
    model = view = proj = glm::mat4(1.0f);

    /* Position model and rotate around X axis */
    model = glm::translate(model, startPos);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0.0f));

    view = camera.getViewMatrix();

    /* Apply perspective transformation */
    proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

    return proj * view * model;
}

int main(void)
{
    GLFWwindow* window;

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

    /* Capture cursor, i.e. keep in window while in focus */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Handle cursor movements */
    glfwSetCursorPosCallback(window, mouse_movement_callback);

    /* Load OpenGL function pointers from glad lib */
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    /* Define the cube and load its vertices into buffers */
    defineCube();

    /* World space cube positions */
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

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
        /* Listen for keyboard events */
        handleKeyboardEvents(window);

        /* Clear screen color */
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < 10; ++i)
        {
            glm::mat4 mvpMatrix = getMVPMatrix(cubePositions[i]);

            /* Pass Model View Projection matrix into vertex shader */
            int mvpUniformLoc = glGetUniformLocation(myShaders.shaderProgramID, "mvp");
            glUniformMatrix4fv(mvpUniformLoc, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}