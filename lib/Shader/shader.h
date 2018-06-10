#ifndef SHADER
#define SHADER

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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
};

#endif