#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GLES3/gl3.h>

/**
 * loads a 2D texture from file
 * @param filename  name of the image file to load
 * @return GLint the texture's name, or 0 if failed
 */
GLuint texture_load(const char *filename);

/**
 * Deallocates a texture.
 * @param texName
 */
void texture_free(GLuint texName);


#endif