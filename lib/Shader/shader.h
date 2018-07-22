#ifndef SHADER
#define SHADER

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

class Shader
{
    public:
        unsigned int shaderProgramID;

        /* Constructor */
        Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

        /* Activator method */
        void use();

        /* Utility methods */
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec3(const std::string &name, glm::vec3 vec) const;
        void setMat4(const std::string &name, glm::mat4 mat) const;
};

#endif