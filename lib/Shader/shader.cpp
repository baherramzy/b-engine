#include "shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    std::string vertexShaderCode;
    std::string fragmentShaderCode;
    const char* vShaderCString;
    const char* fShaderCString;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vStream, fStream;

        /* Read file buffer contents into streams */
        vStream << vShaderFile.rdbuf();
        fStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        /* Store stream contents in string format */
        vertexShaderCode = vStream.str();
        fragmentShaderCode = fStream.str();

        /* Convert to C strings for glShaderSource compatibility */
        vShaderCString = vertexShaderCode.c_str();
        fShaderCString = fragmentShaderCode.c_str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR: Shader -> Unable to load file(s) " << std::endl;
    }

    unsigned int vertexShader, fragmentShader;
    int success;
    char infoLog[512];

    /* Compile vertex shader */
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCString, NULL);
    glCompileShader(vertexShader);

    /* Report compilation errors, if any */
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR: Shader -> Vertex shader compilation failed\n" << infoLog << std::endl;
    }

    /* Compile fragment shader */
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCString, NULL);
    glCompileShader(fragmentShader);

    /* Report compilation errors, if any */
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR: Shader -> Fragment shader compilation failed\n" << infoLog << std::endl;
    }

    /* Create and link shader program */
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);    
    glLinkProgram(shaderProgramID);

    /* Report linking errors, if any */
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        std::cout << "ERROR: Shader -> Program linking failed\n" << infoLog << std::endl;
    }

    /* Remove shaders after linking */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(shaderProgramID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, glm::vec3 vec) const
{
    glUniform3fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, &vec[0]);
}

void Shader::setMat4(const std::string &name, glm::mat4 mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}