#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "../lib/Shader/shader.cpp"
#include "../lib/Camera/camera.cpp"

const unsigned int width = 800, height = 600;
Camera camera;

float frameDeltaTime = 0.0f;
float lastFrameTimestamp = 0.0f;

float lastFrameMouseX = width  / 2;
float lastFrameMouseY = height / 2;
bool isFirstMouseMovement = true;

unsigned int VAO, lightVAO;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void handleKeyboardEvents(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float currentFrameTimestamp = glfwGetTime();
    frameDeltaTime = currentFrameTimestamp - lastFrameTimestamp;
    lastFrameTimestamp = currentFrameTimestamp;

    float cameraSpeed = 4.0f * frameDeltaTime;
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

   float xOffset = xPos - lastFrameMouseX;
   float yOffset = lastFrameMouseY - yPos;

   lastFrameMouseX = xPos;
   lastFrameMouseY = yPos;

   camera.processMouseMovement(xOffset, yOffset);
}

void defineCube()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
}

glm::mat4 getMVPMatrix(glm::vec3 startPos)
{
    glm::mat4 model, view, proj;
    model = view = proj = glm::mat4(1.0f);

    model = glm::translate(model, startPos);
    model = glm::scale(model, glm::vec3(0.2f));

    view = camera.getViewMatrix();

    proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

    return proj * view * model;
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouse_movement_callback);

    gladLoadGL();

    glEnable(GL_DEPTH_TEST);

    defineCube();

    Shader lightingShader("shaders/lighting.vert", "shaders/lighting.frag");
    Shader lampShader("shaders/lighting.vert", "shaders/lamp.frag");

    while (!glfwWindowShouldClose(window))
    {
        handleKeyboardEvents(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        lightingShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 model, view, proj;
        model = view = proj = glm::mat4(1.0f);

        view = camera.getViewMatrix();
        proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("proj", proj);
        lightingShader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lampShader.use();
        lampShader.setMat4("view", view);
        lampShader.setMat4("proj", proj);

        model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        lampShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}