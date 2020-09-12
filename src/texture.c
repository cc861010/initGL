#include "Texture.h"

#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <stdbool.h>
#include "GLES3/gl3.h"


GLuint texture_load(const char *filename){

    //load and create textures
    //-------------------------------------------------------------------
    unsigned int textured1;
    // texture 1
    glGenTextures(1, &textured1);
    glBindTexture(GL_TEXTURE_2D, textured1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, STBI_rgb);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        SDL_Log("Couldn't load texture");
    }
    //clean up
    stbi_image_free(data);
    data = NULL;

    return textured1;
}

void texture_free(GLuint texName) {
    glDeleteTextures(1, &texName);
}