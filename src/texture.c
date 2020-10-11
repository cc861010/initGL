#include "texture.h"

#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <stdbool.h>
#include "GLES3/gl3.h"


GLuint texture_create(const char *filename) {

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *img_data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (img_data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        SDL_Log("Failed to load texture");
    }
    stbi_image_free(img_data);
    //clean up
    img_data = NULL;

    return texture;
}

void texture_use(GLuint n) {
    glActiveTexture(GL_TEXTURE0 + n);
    glBindTexture(GL_TEXTURE_2D, n);
}

void texture_free(GLuint texName) {
    glDeleteTextures(1, &texName);
}