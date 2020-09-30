#include "vertex.h"
#include "angle_gl.h"
#include "stdarg.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>



/**
 * vertex_create(9,"aPos",3,sizeof(float),"aColor",3,sizeof(float),"aTexCoord",2,sizeof(float))
 * @param num
 * @param ...
 * @return
 */
vertex_attribute * vertex_layout_create(int num, ...) {
    if (num == 0 || num % 3 != 0) {
        printf("The number of parameters must be 2n, but it is %d,", num);
        return 0;
    }
    vertex_attribute *attrs = malloc((num / 2) * sizeof(vertex_attribute));
    va_list valist;
    /* initialize valist for num number of arguments */
    va_start(valist, num);
    int stride = 0;
    int offset = 0;
    /* access all the arguments assigned to valist */
    for (int i = 0; i < num / 3; i++) {
        char *name = va_arg(valist, GLchar *);
        if (strlen(name) + 1 > name_length) {
            printf("error, exceed the max length of the name: %s", name);
            exit(-1);
        }
        int l = sizeof(name);
        memcpy(attrs + i, name, strlen(name) + 1);
        attrs[i].number_of_data = va_arg(valist, int);
        attrs[i].size_of_type = va_arg(valist, int);
        attrs[i]._vertex_location = 0;
        attrs[i]._stride = 0;
        attrs[i]._offset = stride;
        offset = attrs[i].number_of_data * attrs[i].size_of_type;
        stride = stride + offset;
        attrs[i]._next = (i == (num / 3 - 1)) ? 0 : attrs + i + 1;
    }
    for (int i = 0; i < num / 3; i++) {
        attrs[i]._stride = stride;
    }
    /* clean memory reserved for valist */
    va_end(valist);
    return attrs;
};

/**
 *
 * @param shader_program
 * @param vertex_attributes
 * @param vertex_array
 * @param vertex_indices
 * @return
 */
GLuint
vertex_data_load(GLuint shader_program, vertex_attribute *vertex_attributes, void *vertices_array, int vertices_array_size,
            const int *indices_array, int indices_array_size) {

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_array_size, vertices_array, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_array_size, indices_array, GL_STATIC_DRAW);

    vertex_attribute *temp = vertex_attributes;
    do {
        temp->_vertex_location = glGetAttribLocation(shader_program, temp->name);
        //todo assert if the location is not found.
        glVertexAttribPointer(temp->_vertex_location, temp->number_of_data, GL_FLOAT, GL_FALSE, temp->_stride,
                              (void *) (temp->_offset));
        //printf("%s=%d,n=%d,stride=%d,offset=%d\n", temp->name, temp->_vertex_location, temp->number_of_data,
//               temp->_stride, temp->_offset);
        glEnableVertexAttribArray(temp->_vertex_location);
        temp = temp->_next;
    } while (temp != 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return VAO;
}