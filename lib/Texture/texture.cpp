#include "texture.h"

/* Public */
Texture::Texture() {}

void Texture::load(const char* path, int width, int height)
{
    unsigned int texture;
    glGenTextures(1, &texture);

    if(++currentTexUnits > maxTexUnits)
    {
        std::cout << "Texture -> Maximum number of texture units per draw exceeded. Max = " << maxTexUnits << std::endl;        
    }

    glActiveTexture(GL_TEXTURE0 + (currentTexUnits - 1));
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

/* Private */
int Texture::getColorModeFromFileExt(const char* path)
{
    const char* extension = path + (strlen(path) - 3);
    int colorMode = strcmp(extension, "png") == 0 ? GL_RGBA : GL_RGB;

    return colorMode;
}