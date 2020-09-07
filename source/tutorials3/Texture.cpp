#include "Texture.h"

#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "GLES3/gl3.h"


GLuint texture_load(const char *filename){

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data) {
        SDL_Log("stbi_load texture %s failed \n", filename);
        return 0;
    }

    //create the texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    GLenum err = glGetError();

    if (err != GL_NO_ERROR) {
        //Failed
        glDeleteBuffers(1, &texture);
        texture = 0;
        stbi_image_free(data);
        data = NULL;
        SDL_Log("Creating texture %s failed, code %u \n", filename, err);
        return 0;
    }

    //set up the filtering
    // Note: failure to do this may result in no texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //clean up
    stbi_image_free(data);
    data = NULL;

    return texture;
}

void texture_free(GLuint texName) {
    glDeleteTextures(1, &texName);
}