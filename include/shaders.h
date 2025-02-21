/**
 * @file shaders.h
 * @brief Shader management functions and structures.
 */

#ifndef IMSDL_SHADERS_H
#define IMSDL_SHADERS_H

#include <GL/glew.h>

char* imsdl_read_shader(const char* filepath);
GLuint imsdl_compile_shader(const char* source, GLenum type);

#endif // IMSDL_SHADERS_H
