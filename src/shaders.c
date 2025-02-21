/**
 * @file shaders.c
 * @brief Shader management functions.
 */

#include "shaders.h"
#include "logger.h"

#include <stdio.h>

char* imsdl_read_shader(const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (!file) {
        LOG_ERROR("Failed to open file: %s", filepath);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(length + 1);
    if (!buffer) {
        LOG_ERROR("Failed to allocate memory for file content.");
        fclose(file);
        return NULL;
    }

    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

GLuint imsdl_compile_shader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    if (!shader) {
        LOG_ERROR("Failed to create shader.");
        return 0;
    }
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOG_ERROR("Shader compilation failed:\n%s", infoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
