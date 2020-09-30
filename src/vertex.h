#ifndef __VERTEX_H__
#define __VERTEX_H__

#include "angle_gl.h"

#define name_length 31

typedef struct {
    GLchar name[name_length];               // position
    int number_of_data;       // For example, position is (1.2f, 3.4f, 5.6f), the number_of_data is 3;
    int size_of_type;         // For example, position is (1.2f, 3.4f, 5.6f), the size_of_type is sizeof(float);
    GLuint _vertex_location;
    int _stride;
    int _offset;
    void *_next;
} vertex_attribute;

vertex_attribute * vertex_layout_create(int num, ...);

GLuint
vertex_data_load(GLuint shader_program, vertex_attribute *vertex_attributes, void *vertices_array, int vertices_array_size,
            const int *indices_array, int indices_array_size);


#endif