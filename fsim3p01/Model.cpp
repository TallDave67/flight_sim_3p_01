#include "Model.h"

#include "Constants.h"

Model::Model()
{

}

Model::~Model()
{

}

void Model::initialize(const char* _name)
{
    name = _name;
}

void Model::LoadModel()
{
    // make model path from object name
    std::string model_path = std::string(PATH_INPUT) + std::string(PATH_MODELS) + name + std::string(MODEL_EXTENSION);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(model_path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!scene)
    {
        printf("Model %s failed to load: %s\n", model_path.c_str(), importer.GetErrorString());
        return;
    }

    LoadNode(scene->mRootNode, scene);
    LoadMaterials(scene);
}

void Model::RenderModel()
{
    unsigned int num_textures = static_cast<unsigned int>(textureList.size());
    for (size_t i = 0; i < meshList.size(); i++)
    {
        unsigned int material_idx = meshToTex[i];
        if (material_idx < num_textures && textureList[material_idx])
        {
            textureList[material_idx]->UseTexture();
        }

        meshList[i]->RenderMesh();

    }
}

void Model::ClearModel()
{
    // because our Mesh and Texture objects are stored in unique_ptrs in the vectors
    // when the vectors go out of scope these objects will be deleted
    // so this function should not be called from the destructor
    std::for_each(meshList.begin(), meshList.end(), [](auto & item) { item.reset(); });
    std::for_each(textureList.begin(), textureList.end(), [](auto & item) { item.reset(); });
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        LoadNode(node->mChildren[i], scene);
    }
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    // vertices
    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
        if (mesh->mTextureCoords[0])
        {
            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
        }
        else
        {
            vertices.insert(vertices.end(), { 0.0f, 0.0f });
        }
        vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
    }

    // indices of the faces
    for (size_t j = 0; j < mesh->mNumFaces; j++)
    {
        aiFace face = mesh->mFaces[j];
        for (size_t k = 0; k < face.mNumIndices; k++)
        {
            indices.push_back(face.mIndices[k]);
        }
    }

    // add a new mesh object to our list and then call its create method
    meshList.push_back(std::make_unique<Mesh>());
    meshList[meshList.size() - 1]->CreateMesh(&vertices[0], &indices[0], 
        static_cast<unsigned int>(vertices.size()), static_cast<unsigned int>(indices.size()));

    // connect the mesh to its texture
    meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
    // set the size of the vector of shared_ptrs
    // each shared_ptr will have its internal object ptr initialized to nullptr
    textureList.resize(scene->mNumMaterials);
    
    // textures
    for (size_t i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* material = scene->mMaterials[i];

        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                // get just the file name
                size_t idx = std::string(path.data).rfind("\\");
                std::string filename = std::string(path.data).substr(idx + 1);

                // and again if path written with "/"
                idx = filename.rfind("/");
                filename = filename.substr(idx + 1);

                // make the file path
                std::string textPath = std::string(PATH_INPUT) + std::string(PATH_TEXTURES) + name + "/" + filename;

                // add the texture
                textureList[i] = std::make_unique<Texture>();
                textureList[i]->initialize(textPath.c_str());
                if (!textureList[i]->LoadTexture(GL_RGB))
                {
                    printf("Failed to load texture at %s\n", textPath.c_str());
                    textureList[i].reset();
                }
            }
        }

        // use our default texture if none exists
        if (!textureList[i])
        {
            textureList[i] = std::make_unique<Texture>();
            std::string textPath = std::string(PATH_INPUT) + std::string(PATH_TEXTURES) + TEXTURE_DEFAULT;
            textureList[i]->initialize(textPath.c_str());
            textureList[i]->LoadTexture(GL_RGBA);
        }
    }
}

