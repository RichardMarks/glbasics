#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

char* read_source(char* filename) {
  FILE* fp = fopen(filename, "rb");
  if (!fp) {
    fprintf(stderr, "Unable to read %s\n", filename);
    return 0;
  }
  fseek(fp, 0, SEEK_END);
  long len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char* buffer = malloc(len + 1);
  fread(buffer, len, 1, fp);
  fclose(fp);
  buffer[len] = '\0';
  return buffer;
}

Shader* create_shader(char* vertSourceFilename, char* fragSourceFilename) {
  Shader* shader = (Shader*)malloc(sizeof(*shader));
  if (!shader) {
    return 0;
  }

  memset(shader, 0, sizeof(*shader));

  shader->vertSource = read_source(vertSourceFilename);
  if (!shader->vertSource) {
    free(shader);
    return 0;
  }

  shader->fragSource = read_source(fragSourceFilename);
  if (!shader->fragSource) {
    if (shader->vertSource) {
      free(shader->vertSource);
    }
    free(shader);
    return 0;
  }

  int params = -1;

  shader->vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(shader->vert, 1, (const char**)&shader->vertSource, 0);
  glCompileShader(shader->vert);
  glGetShaderiv(shader->vert, GL_COMPILE_STATUS, &params);
  if (params != GL_TRUE) {
    fprintf(stderr, "Shader %u did not compile\n", shader->vert);
    int lenMax = 2048;
    int len = 0;
    char buffer[2048];
    glGetShaderInfoLog(shader->vert, lenMax, &len, buffer);
    fprintf(stderr, "Shader %u info log:\n%s\n", shader->vert, buffer);
    if (shader->vertSource) {
      free(shader->vertSource);
      shader->vertSource = 0;
    }
    if (shader->fragSource) {
      free(shader->fragSource);
      shader->fragSource = 0;
    }
    free(shader);
    return 0;
  }

  shader->frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shader->frag, 1, (const char**)&shader->fragSource, 0);
  glCompileShader(shader->frag);
  glGetShaderiv(shader->frag, GL_COMPILE_STATUS, &params);
  if (params != GL_TRUE) {
    fprintf(stderr, "Shader %u did not compile\n", shader->frag);
    int lenMax = 2048;
    int len = 0;
    char buffer[2048];
    glGetShaderInfoLog(shader->frag, lenMax, &len, buffer);
    fprintf(stderr, "Shader %u info log:\n%s\n", shader->frag, buffer);
    if (shader->vertSource) {
      free(shader->vertSource);
      shader->vertSource = 0;
    }
    if (shader->fragSource) {
      free(shader->fragSource);
      shader->fragSource = 0;
    }
    free(shader);
    return 0;
  }

  shader->program = glCreateProgram();
  glAttachShader(shader->program, shader->vert);
  glAttachShader(shader->program, shader->frag);
  glLinkProgram(shader->program);
  glGetProgramiv(shader->program, GL_LINK_STATUS, &params);
  if (params != GL_TRUE) {
    fprintf(stderr, "Linking shader program %u failed.\n", shader->program);
    int lenMax = 2048;
    int len = 0;
    char buffer[2048];
    glGetProgramInfoLog(shader->program, lenMax, &len, buffer);
    fprintf(stderr, "Program %u info log:\n%s\n", shader->program, buffer);
    if (shader->vertSource) {
      free(shader->vertSource);
      shader->vertSource = 0;
    }
    if (shader->fragSource) {
      free(shader->fragSource);
      shader->fragSource = 0;
    }
    free(shader);
    return 0;
  }

  glUseProgram(shader->program);

  return shader;
}

void use_shader(Shader* shader) {
  if (shader) {
    glUseProgram(shader->program);
  }
}

void destroy_shader(Shader* shader) {
  if (shader) {
    if (shader->vertSource) {
      free(shader->vertSource);
      shader->vertSource = 0;
    }
    if (shader->fragSource) {
      free(shader->fragSource);
      shader->fragSource = 0;
    }
    glDeleteProgram(shader->program);
    free(shader);
  }
}
