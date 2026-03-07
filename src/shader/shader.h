#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
typedef struct {
    unsigned int ID;

} Shader;

Shader *shader_create(const char *vertexPath, const char *fragmentPath);
void shader_use(Shader *shader);
void shader_setBool(Shader *shader, const char *name, bool value);
void shader_setInt(Shader *shader, const char *name, int value);
void shader_setFloat(Shader *shader, const char *name, float value);

#endif
