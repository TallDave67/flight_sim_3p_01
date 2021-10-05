#pragma once
#include <GL/glew.h>

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numVertices, unsigned int numIndices);
    void RenderMesh();
    void ClearMesh();

private:
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
    GLsizei vertexCount;
    GLsizei indexCount;
};

