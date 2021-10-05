#pragma once

#include <vector>
#include <string>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model
{
public:
    Model();
    ~Model();

    void initialize(const char* _name);

    void LoadModel();
    void RenderModel();
    void ClearModel();

private:
    std::vector<std::unique_ptr<Mesh>> meshList;
    std::vector<std::unique_ptr<Texture>> textureList;
    std::vector<unsigned int> meshToTex;

    std::string name;

private:
    void LoadNode(aiNode *node, const aiScene *scene);
    void LoadMesh(aiMesh *mesh, const aiScene *scene);
    void LoadMaterials(const aiScene *scene);
};

