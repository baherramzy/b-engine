#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "../lib/Shader/shader.cpp"
#include "../lib/Texture/texture.cpp"
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
        /* Vertex Position */ /* Normal Vector */ /* Texture Co-ordinates */
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
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
    lightingShader.use();
    // Light source properties
    lightingShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
    lightingShader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // Light attenuation parameters
    lightingShader.setFloat("light.attConstant", 1.0f);
    lightingShader.setFloat("light.attLinear", 0.07f);
    lightingShader.setFloat("light.attQuadratic", 0.017f);

    // Material light reflection properties
    lightingShader.setVec3("material.specular", glm::vec3(0.628281f, 0.555802f, 0.366065f));
    lightingShader.setFloat("material.shine", 0.4f * 128.0f);

    // Load diffuse map texture
    Texture diffuseMap;
    const unsigned int diffuseMapID = diffuseMap.load("assets/Textures/diffuse_wood_container.png", 500, 500);
    lightingShader.setInt("material.diffuse", 0);

    // Load specular map texture
    Texture specularMap;
    const unsigned int specularMapID = specularMap.load("assets/Textures/specular_wood_container.png", 500, 500);
    lightingShader.setInt("material.specular", 1);

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // Individual point light definitions
    lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
    lightingShader.setVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    lightingShader.setVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    lightingShader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("pointLights[0].attConstant", 1.0f);
    lightingShader.setFloat("pointLights[0].attLinear", 0.09);
    lightingShader.setFloat("pointLights[0].attQuadratic", 0.032);

    lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
    lightingShader.setVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    lightingShader.setVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    lightingShader.setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("pointLights[1].attConstant", 1.0f);
    lightingShader.setFloat("pointLights[1].attLinear", 0.09);
    lightingShader.setFloat("pointLights[1].attQuadratic", 0.032);

    lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
    lightingShader.setVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    lightingShader.setVec3("pointLights[2].diffuse",glm::vec3( 0.8f, 0.8f, 0.8f));
    lightingShader.setVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("pointLights[2].attConstant", 1.0f);
    lightingShader.setFloat("pointLights[2].attLinear", 0.09);
    lightingShader.setFloat("pointLights[2].attQuadratic", 0.032);

    lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
    lightingShader.setVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    lightingShader.setVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
    lightingShader.setVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    lightingShader.setFloat("pointLights[3].attConstant", 1.0f);
    lightingShader.setFloat("pointLights[3].attLinear", 0.09);
    lightingShader.setFloat("pointLights[3].attQuadratic", 0.032);

    lightingShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    lightingShader.setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    lightingShader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    lightingShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

    Shader lampShader("shaders/lamp.vert", "shaders/lamp.frag");

    while (!glfwWindowShouldClose(window))
    {
        handleKeyboardEvents(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        glm::mat4 model, view, proj;
        view = proj = glm::mat4(1.0f);

        view = camera.getViewMatrix();
        proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

        // glm::vec3 lightPos(sin(glfwGetTime() * 1.2f) * 2.0f, sin(glfwGetTime() * 1.2f) * 1.5f, cos(glfwGetTime() * 1.2f) * 2.0f);
        glm::vec3 lightPos(1.0f, 2.0f, 2.0f);

        lightingShader.use();
        lightingShader.setVec3("light.position", camera.pos);
        lightingShader.setVec3("light.direction", camera.front);
        lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

        lightingShader.setVec3("cameraPos", camera.pos);

        lightingShader.setMat4("view", view);
        lightingShader.setMat4("proj", proj);

        // Diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMapID);

        // Specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMapID);

        glBindVertexArray(VAO);

        for(int i = 0; i < 10; ++i)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lampShader.use();
        lampShader.setMat4("view", view);
        lampShader.setMat4("proj", proj);

        // model = glm::translate(model, lightPos);
        // model = glm::scale(model, glm::vec3(0.2f));
        // lampShader.setMat4("model", model);
        for(int i = 0; i < 4; ++i)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lampShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}