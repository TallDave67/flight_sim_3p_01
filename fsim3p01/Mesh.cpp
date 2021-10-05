#include "Mesh.h"

#include "Constants.h"

Mesh::Mesh() :
    VAO(0), VBO(0), IBO(0), vertexCount(0), indexCount(0)
{
}

Mesh::~Mesh()
{
    ClearMesh();
}

void Mesh::CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numVertices, unsigned int numIndices)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    vertexCount = static_cast<GLsizei>(numVertices);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, GL_STATIC_DRAW);

    if (indices)
    {
        indexCount = static_cast<GLsizei>(numIndices);
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);
    }

    glVertexAttribPointer(0, VERTEX_INFO_COORD_NUM, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * VERTEX_INFO_TOTAL_NUM, 0);
    glEnableVertexAttribArray(0);
    // note that this offset will be passed into glVertexAttribPointer as a void* implying it is a memory address
    // but it is not a memory address, for an explanation of the unfortunate evolution of this function
    // see https://stackoverflow.com/questions/28063307/in-opengl-why-does-glvertexattribpointer-require-the-pointer-argument-to-be-p
    // note that we cannot use static_cast<void*> because the conversion between these two types is actually not acceptable
    unsigned long long offsetC = sizeof(vertices[0]) * static_cast<unsigned long long>(VERTEX_INFO_COORD_NUM);
    glVertexAttribPointer(1, VERTEX_INFO_TEXTURE_NUM, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * VERTEX_INFO_TOTAL_NUM, (void*)(offsetC));
    glEnableVertexAttribArray(1);
    unsigned long long offsetCT = sizeof(vertices[0]) * static_cast<unsigned long long>(VERTEX_INFO_COORD_NUM + VERTEX_INFO_TEXTURE_NUM);
    glVertexAttribPointer(2, VERTEX_INFO_NORMAL_NUM, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * VERTEX_INFO_TOTAL_NUM, (void*)(offsetCT));
    glEnableVertexAttribArray(2);

    if (indices)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Mesh::RenderMesh()
{
    if (VAO && IBO && indexCount)
    {
        //printf("RenderMesh: glDrawElements\n");
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        //printf("error = %d\n", glGetError());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    else if (VAO)
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glBindVertexArray(0);
    }
}

void Mesh::ClearMesh()
{
    if (IBO)
    {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
        indexCount = 0;
    }

    if (VBO)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
        vertexCount = 0;
    }

    if (VAO)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
}
