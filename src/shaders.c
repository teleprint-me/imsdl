/**
 * @file shaders.c
 * @brief Shader management functions.
 */

#include "shaders.h"
#include "logger.h"

#include <stdio.h>

char* imsdl_read_shader(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        LOG_ERROR("Failed to open file: %s", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(length + 1);
    buffer[length] = '\0';
    fread(buffer, 1, length, file);

    fclose(file);
    return buffer;
}

GLuint imsdl_compile_shader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOG_ERROR("Shader Compilation Failed: %s", infoLog);
    }

    return shader;
}

GLuint imsdl_create_shader_program(const char* vertex_file, const char* fragment_file) {
    char* vertex_source = imsdl_read_shader(vertex_file);
    char* fragment_source = imsdl_read_shader(fragment_file);

    GLuint vertex_shader = imsdl_compile_shader(vertex_source, GL_VERTEX_SHADER);
    GLuint fragment_shader = imsdl_compile_shader(fragment_source, GL_FRAGMENT_SHADER);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        LOG_ERROR("Shader Program Linking Failed: %s", infoLog);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    free(vertex_source);
    free(fragment_source);

    return shader_program;
}
