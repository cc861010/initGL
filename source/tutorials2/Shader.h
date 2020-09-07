#ifndef __SHADER_H__
#define __SHADER_H__

#include "angle_gl.h"


/**
 * load a vertex and fragment shader from disk and compiles (&links) them into a shader program
 * this will print any errors to the console.
 * @param vertFilename
 * @param fragFilename
 * @return
 */
GLuint shaderProgramsLoad(const char *vertFilename, const char *fragFilename);
/**
 * Destroy a shader program
 * @param shaderProg
 */
void shaderProgramsDestroy(GLuint shaderProgram);

#endif