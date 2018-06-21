#ifndef TEXTURE
#define TEXTURE

#include <glad/glad.h>
// #include <glfw3.h>
#include <iostream>
#include "stb_image.cpp"

class Texture
{
    public:
        Texture();
        void load(const char* path, int width, int height);
    private:
        int currentTexUnits = 0;
        int maxTexUnits = 32;
        int getColorModeFromFileExt(const char* path);
};

#endif