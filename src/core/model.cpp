/* GLEP - OpenGL Engine Platform
 * Copyright (C) 2025 Jasper Devir <jasperdevir.jd@gmail.com>
 *
 * GLEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * GLEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GLEP.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <GLEP/core/model.hpp>

namespace GLEP{

    Model::Model(){}

    Model::Model(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material){
        _meshes.push_back(std::make_shared<Mesh>(geometry, material));
    }

    Model::Model(std::shared_ptr<Mesh> mesh){
        _meshes.push_back(mesh);
    }
            
    Model::Model(std::vector<std::shared_ptr<Mesh>> meshes){
        _meshes = meshes;
    }

    Model::Model(std::shared_ptr<Model> model){
        _meshes = model->GetMeshes();
    }

    void Model::CalculateNormals(){
        _calculateNormalsNeeded = false;

        for(std::shared_ptr<Mesh> m : _meshes){
            m->GeometryData->CalculateNormals();
        }

    }

    std::vector<std::shared_ptr<Mesh>> Model::GetMeshes(){ return _meshes; }

    json Model::ToJson(){
        json j;
        j["type"] = "model";
        j["object_data"] = SceneObject::ToJson();
        j["meshes"] = json::array();
        for(auto& m : _meshes){
            j["meshes"].push_back(m->ToJson());
        }
        return j;
    }

    std::shared_ptr<Model> Model::FromJson(const json& data){
        if(data["type"] == "import_model"){
            return ImportModel::FromJson(data);
        } else if (data["type"] == "import_geometry_model"){
            return ImportGeometryModel::FromJson(data);
        } else if (data["type"] == "model"){
            if(!data["meshes"].is_array()){
                Print(PrintCode::ERROR, "MODEL", "Model meshes must be an array.");
                return nullptr;
            }

            std::vector<std::shared_ptr<Mesh>> meshes;

            for(int i = 0; i < data["meshes"].size(); i++){
                meshes.push_back(Mesh::FromJson(data["meshes"][i]));
            }

            return std::make_shared<Model>(meshes);
        } else {
            Print(PrintCode::ERROR, "MODEL", "Unknown Model type: " + data["type"]);
            return nullptr;
        }
        
    }

    ImportModelTexture::ImportModelTexture(std::filesystem::path filePath){
        _filePath = filePath;

        initialize();
    }

    void ImportModelTexture::initialize(){
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(_filePath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            Print(PrintCode::ERROR, "MODEL", "Assimp Error: \n" + std::string(import.GetErrorString()));
            return;
        }

        processNode(scene->mRootNode, scene);
    }

    void ImportModelTexture::processNode(aiNode *node, const aiScene *scene)
    {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
            _textureMaps.push_back(processTextureMap(mesh, scene));			
        }

        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    } 

    std::shared_ptr<TextureMap> ImportModelTexture::processTextureMap(aiMesh *mesh, const aiScene *scene){
        aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex]; 

        std::shared_ptr<Texture> diffuseMap = loadMaterialTexture(aiMaterial, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        std::shared_ptr<Texture> specularMap = loadMaterialTexture(aiMaterial, aiTextureType_SPECULAR, TextureType::SPECULAR);
        std::shared_ptr<Texture> normalMap = loadMaterialTexture(aiMaterial, aiTextureType_NORMALS, TextureType::NORMAL);
        std::shared_ptr<Texture> heightMap = loadMaterialTexture(aiMaterial, aiTextureType_HEIGHT, TextureType::HEIGHT);

        return std::make_shared<TextureMap>(diffuseMap, specularMap, normalMap, heightMap);
    }

    std::shared_ptr<Texture> ImportModelTexture::loadMaterialTexture(aiMaterial *mat, aiTextureType aiType, TextureType type){
        for(unsigned int i = 0; i < mat->GetTextureCount(aiType); i++)
        {
            aiString str;
            mat->GetTexture(aiType, i, &str);
            std::string texPath = _filePath.parent_path().string() + "/" + str.C_Str();
            for(unsigned int j = 0; j < _loadedTextures.size(); j++)
            {
                if(_loadedTextures[j]->GetFilePath().string() == texPath)
                {
                    return _loadedTextures[j];
                }
            }
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(texPath, type);
            _loadedTextures.push_back(texture);
            return texture;
        }

        return nullptr;
    }

    std::filesystem::path ImportModelTexture::GetFilePath(){
        return _filePath;
    }

    std::vector<std::shared_ptr<TextureMap>> ImportModelTexture::GetTextureMaps(){
        return _textureMaps;
    }

    ImportGeometryModel::ImportGeometryModel(std::filesystem::path modelPath, std::shared_ptr<Material> baseMaterial, bool copyUniforms){
        _importGeometry = std::make_shared<ImportGeometry>(modelPath);
        _baseMaterial = baseMaterial;

        initialize(copyUniforms);
    }

    ImportGeometryModel::ImportGeometryModel(std::shared_ptr<ImportGeometry> geometry, std::shared_ptr<Material> baseMaterial, bool copyUniforms){
        _importGeometry = geometry;
        _baseMaterial = baseMaterial;

        initialize(copyUniforms);
    }

    void ImportGeometryModel::initialize(bool copyUniforms){
        for(auto& g : _importGeometry->GetGeometry()){
            _meshes.push_back(std::make_shared<Mesh>(g, std::make_shared<Material>(_baseMaterial, copyUniforms)));
        }
    }

    std::shared_ptr<ImportGeometry> ImportGeometryModel::GetImportGeometry(){
        return _importGeometry;
    }

    std::shared_ptr<Material> ImportGeometryModel::GetBaseMaterial(){
        return _baseMaterial;
    }

    void ImportGeometryModel::ApplyImportTextures(std::shared_ptr<ImportModelTexture> importTexture){
        std::vector<std::shared_ptr<TextureMap>> textureMaps = importTexture->GetTextureMaps();
        for(int i = 0; i < textureMaps.size(); i++){
            if(i > _meshes.size() - 1){
                Print(PrintCode::ERROR, "IMPORT_GEOMETRY_MODEL", "Failed applying import textures: Texture map and meshes size unaligned");
                return;
            }

            std::shared_ptr<Texture> diffuseTex = textureMaps[i]->Diffuse;
            std::shared_ptr<Texture> specularTex = textureMaps[i]->Specular;
            std::shared_ptr<Texture> normalTex = textureMaps[i]->Normal;
            std::shared_ptr<Texture> heightTex = textureMaps[i]->Height;

            std::shared_ptr<Material> meshMat = _meshes[i]->MaterialData;

            if(diffuseTex != nullptr){
                if(specularTex != nullptr){
                    meshMat->AddUniform<float>("uMaterial.type", 3);
                } else {
                    meshMat->AddUniform<float>("uMaterial.type", 2);
                    if(meshMat->SetUniformValue<Color>("uMaterial.specularColor", Color::WHITE) == nullptr){
                        meshMat->AddUniform<Color>("uMaterial.specularColor", Color::WHITE);
                    }
                    //Print(PrintCode::INFO, "IMPORT_GEOMETRY_MODEL", "Specular texture is null. Applying Color::WHITE.");
                }
            } else {
                meshMat->AddUniform<float>("uMaterial.type", 1);
                if(meshMat->SetUniformValue<Color>("uMaterial.diffuseColor", Color::CLEAR) == nullptr){
                    meshMat->AddUniform<Color>("uMaterial.diffuseColor", Color::CLEAR);
                }
                if(meshMat->SetUniformValue<Color>("uMaterial.specularColor", Color::CLEAR) == nullptr){
                    meshMat->AddUniform<Color>("uMaterial.specularColor", Color::CLEAR);
                }
                Print(PrintCode::ERROR, "IMPORT_GEOMETRY_MODEL", "Diffuse texture is null. Applying Color::CLEAR to diffuse and specular.");
                continue;
            }

            if(diffuseTex != nullptr && meshMat->SetUniformValue<std::shared_ptr<Texture>>("uMaterial.diffuseTex", diffuseTex) == nullptr){
                meshMat->AddUniform<std::shared_ptr<Texture>>("uMaterial.diffuseTex", diffuseTex);
            }

            if(specularTex != nullptr && meshMat->SetUniformValue<std::shared_ptr<Texture>>("uMaterial.specularTex", specularTex) == nullptr){
                meshMat->AddUniform<std::shared_ptr<Texture>>("uMaterial.specularTex", specularTex);
            }

            if(normalTex != nullptr){
                if(meshMat->SetUniformValue<std::shared_ptr<Texture>>("uMaterial.normalTex", normalTex) == nullptr)
                    meshMat->AddUniform<std::shared_ptr<Texture>>("uMaterial.normalTex", normalTex);
                meshMat->SetUniform("uMaterial.hasNormalMap", true);
            }

            if(heightTex != nullptr && meshMat->SetUniformValue<std::shared_ptr<Texture>>("uMaterial.heightTex", heightTex) == nullptr){
                meshMat->AddUniform<std::shared_ptr<Texture>>("uMaterial.heightTex", heightTex);
            }
        }
    }
    
    json ImportGeometryModel::ToJson(){
        json j;
        j["type"] = "import_geometry_model";
        j["object_data"] = SceneObject::ToJson();
        j["geometry_data"] = _importGeometry->ToJson();
        j["material_data"] = _baseMaterial->ToJson();
        return j;
    }

    std::shared_ptr<ImportGeometryModel> ImportGeometryModel::FromJson(const json& data){
        return std::make_shared<ImportGeometryModel>(
            ImportGeometry::FromJson(data["geometry_data"]),
            Material::FromJson(data["material_data"])
        );
    }

    ImportModel::ImportModel(std::filesystem::path filePath, bool lightingMaterial){
        _filePath = filePath;
        _lightingMaterial = lightingMaterial;

        initialize();

        if(_calculateNormalsNeeded) 
            CalculateNormals();
    }

    void ImportModel::initialize(){
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(_filePath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            Print(PrintCode::ERROR, "MODEL", "Assimp Error: \n" + std::string(import.GetErrorString()));
            return;
        }

        processNode(scene->mRootNode, scene);
    }

    void ImportModel::processNode(aiNode *node, const aiScene *scene)
    {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
            _meshes.push_back(processMesh(mesh, scene));			
        }

        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    } 

    std::shared_ptr<Mesh> ImportModel::processMesh(aiMesh *mesh, const aiScene *scene){
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            //WorldPosition
            glm::vec3 position; 
            position.x = mesh->mVertices[i].x;
            position.y = mesh->mVertices[i].y;
            position.z = mesh->mVertices[i].z; 

            //Normal
            glm::vec3 normal;
            if(mesh->HasNormals()){
                normal.x = mesh->mNormals[i].x;
                normal.y = mesh->mNormals[i].y;
                normal.z = mesh->mNormals[i].z; 
            } else {
                if(!_calculateNormalsNeeded) 
                    _calculateNormalsNeeded = true;
                normal = glm::vec3(0.0f);
            }
            

            //Texture Coords
            glm::vec2 texCoords;
            if(mesh->HasTextureCoords(0)){
                texCoords.x = mesh->mTextureCoords[0][i].x;
                texCoords.y = mesh->mTextureCoords[0][i].y;
            } else {
                texCoords = glm::vec2(0.0f);
            }
            
            //Tangent
            glm::vec3 tangent;
            if(mesh->HasTangentsAndBitangents()){
                tangent.x = mesh->mTangents[i].x;
                tangent.y = mesh->mTangents[i].y;
                tangent.z = mesh->mTangents[i].z;
            } else {
                if(!_calculateNormalsNeeded) _calculateNormalsNeeded = true;
                tangent = glm::vec3(0.0f);
            }
            
            vertices.push_back(Vertex(position, normal, texCoords, tangent));
        }

        //Indices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }  

        //Textures
        aiMaterial* aiMaterial = scene->mMaterials[mesh->mMaterialIndex]; 

        std::shared_ptr<Texture> diffuseMap = loadMaterialTexture(aiMaterial, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        std::shared_ptr<Texture> specularMap = loadMaterialTexture(aiMaterial, aiTextureType_SPECULAR, TextureType::SPECULAR);
        std::shared_ptr<Texture> normalMap = loadMaterialTexture(aiMaterial, aiTextureType_NORMALS, TextureType::NORMAL);
        std::shared_ptr<Texture> heightMap = loadMaterialTexture(aiMaterial, aiTextureType_HEIGHT, TextureType::HEIGHT);

        std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>(vertices, indices);

        std::shared_ptr<Material> material;
        if(_lightingMaterial){
            if(diffuseMap){
                material = std::make_shared<LambertMaterial>(diffuseMap);
            } else {
                material = std::make_shared<LambertMaterial>(Color(1.0f));
            }
        } else {
            if(diffuseMap){
                material = std::make_shared<UnlitMaterial>(diffuseMap);
            } else {
                material = std::make_shared<UnlitMaterial>(Color(1.0f));
            }
        }
    
        return std::make_shared<Mesh>(geometry, material);
    }

    std::shared_ptr<Texture> ImportModel::loadMaterialTexture(aiMaterial *mat, aiTextureType aiType, TextureType type){
        for(unsigned int i = 0; i < mat->GetTextureCount(aiType); i++)
        {
            aiString str;
            mat->GetTexture(aiType, i, &str);
            std::string texPath = _filePath.parent_path().string() + "/" + str.C_Str();
            for(unsigned int j = 0; j < _loadedTextures.size(); j++)
            {
                if(_loadedTextures[j]->GetFilePath().string().data() == texPath)
                {
                    return _loadedTextures[j];
                }
            }
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(texPath, type);
            _loadedTextures.push_back(texture);
            return texture;
        }

        return nullptr;
    }

    std::filesystem::path ImportModel::GetFilePath(){
        return _filePath;
    }

    json ImportModel::ToJson(){
        json j;

        j["type"] = "import_model";
        j["object_data"] = SceneObject::ToJson();
        j["path"] = _filePath;
         
        return j;
    }

    std::shared_ptr<ImportModel> ImportModel::FromJson(const json& data){
        return std::make_shared<ImportModel>(
            data["path"]
        );
    }
    
}