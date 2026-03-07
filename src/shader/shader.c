#include "shader.h"
#include "../../dependencies/glad/include/glad/glad.h"
#include <stdio.h>
#include <stdlib.h>

char *readFileContent(const char *filePath) {
    FILE *f = fopen(filePath, "r");
    if (f == NULL) {
        printf("failed to open the file  \n");
        return NULL;
    };
    fseek(f, 0, SEEK_END);
    int len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *content = malloc(len + 1);
    if (content != NULL) {
        fread(content, 1, len, f);
        content[len] = '\0';
    } else {
        printf("failed to alloc the memory for content");
        fclose(f);
        return NULL;
    }
    fclose(f);
    return content;
};

Shader *shader_create(const char *vertexPath, const char *fragmentPath) {

    char *vertexContent = readFileContent(vertexPath);
    if (vertexContent == NULL) {
        perror("Error");
        return NULL;
    }
    char *fragContent = readFileContent(fragmentPath);
    if (fragContent == NULL) {
        perror("Error");
        return NULL;
    }
    unsigned int shaders[2];
    shaders[0] = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaders[0], 1, (const char **)&vertexContent, NULL);
    glCompileShader(shaders[0]);

    int success[2];
    char infoLog[512];
    glGetShaderiv(shaders[0], GL_COMPILE_STATUS, &success[0]);
    if (!success[0]) {
        glGetShaderInfoLog(shaders[0], 512, NULL, infoLog);
        printf("failed to compile the vertex Shader %s\n", infoLog);
        glDeleteShader(shaders[0]);
        free(vertexContent);
        free(fragContent);
        return NULL;
    }

    shaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaders[1], 1, (const char **)&fragContent, NULL);
    glCompileShader(shaders[1]);

    glGetShaderiv(shaders[1], GL_COMPILE_STATUS, &success[1]);
    if (!success[1]) {
        glGetShaderInfoLog(shaders[1], 512, NULL, infoLog);
        printf("failed to compile the fragment shader %s\n", infoLog);
        glDeleteShader(shaders[0]);
        glDeleteShader(shaders[1]);
        free(vertexContent);
        free(fragContent);
        return NULL;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, shaders[0]);
    glAttachShader(shaderProgram, shaders[1]);
    glLinkProgram(shaderProgram);

    int successLinking;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successLinking);
    if (!successLinking) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("failed to link the shaders  %s\n", infoLog);
        glDeleteShader(shaders[0]);
        glDeleteShader(shaders[1]);
        glDeleteProgram(shaderProgram);
        free(vertexContent);
        free(fragContent);
        return NULL;
    }
    glDeleteShader(shaders[0]);
    glDeleteShader(shaders[1]);
    free(vertexContent);
    free(fragContent);
    Shader *s = malloc(sizeof(Shader));
    if (s == NULL) {
        return NULL;
    }
    s->ID = shaderProgram;
    return s;
};

void shader_use(Shader *shader) { glUseProgram(shader->ID); };

void shader_setBool(Shader *shader, const char *name, bool value) {
    glUniform1i(glGetUniformLocation(shader->ID, name), (int)value);
};
void shader_setFloat(Shader *shader, const char *name, float value) {
    glUniform1f(glGetUniformLocation(shader->ID, name), value);
};
void shader_setInt(Shader *shader, const char *name, int value) {
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
};
