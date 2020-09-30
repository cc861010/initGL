#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <GLES3/gl3.h>

//    select the shader program to use (glUseProgram)
//    for each texture activate in turn the texture unit (glActiveTexture(GL_TEXTURE_0+i) and bind the texture ('glBindTexture`)
//    set the sampler uniforms to the texture units to use (glUniform1i(…, i)).
//    draw the geometry.


/**
 * loads a 2D texture from file
 * @param filename  name of the image file to load
 * @return GLint the texture's name, or 0 if failed
 */
GLuint texture_load(const char *filename);

/**
 * used before drawn in the main loop
 */
void texture_active_Bind(GLuint);

/**
 * Deallocates a texture.
 * @param texName
 */
void texture_free(GLuint texName);


#endif