#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"
#include "glutwrapper.h"
#include "BMPLoader.h"

unsigned int LoadTexture(const char* filename)
{
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 3);
    if (!data) {
        printf("Failed to load texture: %s\n", filename);
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    printf("Loaded texture: %s (%dx%d)\n", filename, width, height);
    return tex;
}

unsigned int LoadTextureBMP(const char* filename)
{
    BMPClass bmp;
    BMPError err = BMPLoad(filename, bmp);
    if (err != BMPNOERROR) {
        printf("Failed to load texture %s: %s\n", filename, TranslateBMPError(err).c_str());
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bmp.width, bmp.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bmp.bytes);

    printf("Loaded texture: %s (%dx%d)\n", filename, bmp.width, bmp.height);
    return tex;
}

