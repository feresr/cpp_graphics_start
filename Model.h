#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
    public:
        Model(char* path) { loadModel(path); }
        void Draw(Shader& shader);
        private:
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        std::vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName);
        private:
        std::vector<Texture> textures_loaded; 

};