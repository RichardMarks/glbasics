#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "mesh.h"

void on_glfw_error(int error, const char* description) {
  fprintf(stderr, "GLFW Error: %s\n", description);
}

int wireframe = 0;

void on_glfw_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  } else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
    wireframe = wireframe == 0 ? 1 : 0;
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
  }
}

int main (int argc,char* argv[]) {
  float vertices[] = {
    0.0f, 0.5f, 0.0f,
    0.5f,-0.5f, 0.0f,
   -0.5f,-0.5f, 0.0f
  };
  Mesh* mesh = create_mesh(3, vertices);

  GLFWwindow* mainWindowPtr = 0;
  if (!glfwInit()) {
    fprintf(stderr, "Unable to init GLFW\n");
    return -1;
  }
  glfwSetErrorCallback(on_glfw_error);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
  mainWindowPtr = glfwCreateWindow(640, 480, "GL Basics", 0, 0);
  if (!mainWindowPtr) {
    fprintf(stderr, "Unable to create window.\n");
    destroy_mesh(mesh);
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(mainWindowPtr);

  glfwSetKeyCallback(mainWindowPtr, on_glfw_key);

  printf("OpenGL version supported: %s\n", glGetString(GL_VERSION));
  printf("OpenGL renderer: %s\n", glGetString(GL_RENDERER));

  // backface culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glewExperimental = GL_TRUE;
  glewInit();

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh->numVertices * 3 * sizeof(float), mesh->vertices, GL_STATIC_DRAW);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  Shader* simpleShader = create_shader(
    "../shaders/vs.glsl",
    "../shaders/fs.glsl"
  );

  if (!simpleShader) {
    printf("failed to create shader\n");
  }

  GLfloat bgColor[] = { 0.2f, 0.4f, 0.6f, 1.0f };

  while (!glfwWindowShouldClose(mainWindowPtr)) {
    glClearBufferfv(GL_COLOR, 0, bgColor);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(mainWindowPtr);
    glfwPollEvents();
  }
  glDeleteVertexArrays(1, &vao);

  glfwTerminate();

  destroy_shader(simpleShader);

  destroy_mesh(mesh);

  return 0;
}
