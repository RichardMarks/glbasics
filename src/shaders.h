#ifndef SHADERS_H
#define SHADERS_H

static const char* vertSource[] = {
  "#version 410 core\n"
  "in vec3 vp;"
  "void main(void){\n"
  "  gl_Position = vec4(vp, 1.0);\n"
  "}"
};

static const char* fragSource[] = {
  "#version 410 core\n"
  "out vec4 color;\n"
  "void main(void){\n"
  "  color = vec4(1.0, 0.3, 0.3, 1.0);\n"
  "}"
};

#endif
