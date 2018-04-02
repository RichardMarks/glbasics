#ifndef SHADER_H
#define SHADER_H

typedef struct t_Shader {
  unsigned int vert;
  unsigned int frag;
  unsigned int program;
  char* vertSource;
  char* fragSource;
} Shader;

extern Shader* create_shader(char* vertSourceFilename, char* fragSourceFilename);
extern void use_shader(Shader* shader);
extern void destroy_shader(Shader* shader);

#endif
