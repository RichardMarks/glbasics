#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "mesh.h"

Mesh* create_mesh(int numVertices, float* vertices) {
  Mesh* mesh = (Mesh*)malloc(sizeof(*mesh));
  if (mesh) {
    memset(mesh, 0, sizeof(*mesh));

    mesh->numVertices = numVertices;
    mesh->vertices = calloc(numVertices * 3, sizeof(float));

    memcpy(mesh->vertices, vertices, sizeof(float) * (3 * numVertices));
  }

  return mesh;
}

void destroy_mesh(Mesh* mesh) {
  if (mesh) {
    if (mesh->numVertices && mesh->vertices) {
      free(mesh->vertices);
      mesh->vertices = 0;
    }
    mesh->numVertices = 0;
    free(mesh);
  }
}
