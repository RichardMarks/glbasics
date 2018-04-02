#ifndef MESH_H
#define MESH_H

typedef struct t_Mesh {
  float* vertices;
  unsigned int numVertices;
} Mesh;

extern Mesh* create_mesh(int numVertices, float* vertices);
extern void destroy_mesh(Mesh* mesh);

#endif
