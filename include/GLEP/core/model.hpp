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

#ifndef MODEL_HPP
#define MODEL_HPP

#include <GLEP/core/geometry.hpp>
#include <GLEP/core/material.hpp>
#include <GLEP/core/mesh.hpp>
#include <GLEP/core/texture.hpp>

#include <filesystem>
#include <string>
#include <sstream>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GLEP{

    using json = nlohmann::ordered_json;

    class Model : public SceneObject{
        protected:
            std::vector<std::shared_ptr<Mesh>> _meshes;

            bool _calculateNormalsNeeded = false;

            void initialize();
            
        public:
            Model();
            Model(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);
            Model(std::shared_ptr<Mesh> mesh);
            Model(std::vector<std::shared_ptr<Mesh>> meshes);
            Model(std::shared_ptr<Model> model);


            /// @brief Recalculate each vertex of each geometry's normal vector based on its position.
            void CalculateNormals();


            /// @brief Get the meshes of this model.
            /// @return Meshes
            std::vector<std::shared_ptr<Mesh>> GetMeshes();


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson() override;

            /// @brief Deserialized data from JSON format.
            /// @param data Model data in JSON format
            /// @return Deserialize Model
            static std::shared_ptr<Model> FromJson(const json& data);
    };

    class ImportModelTexture {
        private:
            std::filesystem::path _filePath;
            std::vector<std::shared_ptr<TextureMap>> _textureMaps;

            std::vector<std::shared_ptr<Texture>> _loadedTextures;

            void initialize();
            void processNode(aiNode *node, const aiScene *scene);
            std::shared_ptr<TextureMap> processTextureMap(aiMesh *mesh, const aiScene *scene);
            std::shared_ptr<Texture> loadMaterialTexture(aiMaterial *mat, aiTextureType aiType, TextureType type);

        public:
            ImportModelTexture(std::filesystem::path filePath);

            /// @brief Get the model file path.
            /// @return Model file path
            std::filesystem::path GetFilePath();

            /// @brief Get all texture maps generated.
            /// @return Texture maps
            std::vector<std::shared_ptr<TextureMap>> GetTextureMaps();
    };

    class ImportGeometryModel : public Model{
        private:
            std::shared_ptr<ImportGeometry> _importGeometry;
            std::shared_ptr<Material> _baseMaterial;

            void initialize(bool copyUniforms);

        public:
            ImportGeometryModel(std::filesystem::path modelPath, std::shared_ptr<Material> baseMaterial, bool copyUniforms = true);
            ImportGeometryModel(std::shared_ptr<ImportGeometry> geometry, std::shared_ptr<Material> baseMaterial, bool copyUniforms = true);

            /// @brief Get the import geometry.
            /// @return Import geometry
            std::shared_ptr<ImportGeometry> GetImportGeometry();

            /// @brief Get the base material.
            /// @return Base material
            std::shared_ptr<Material> GetBaseMaterial();


            /// @brief Apply imported material texture maps.
            /// @param importTexture Import textures
            void ApplyImportTextures(std::shared_ptr<ImportModelTexture> importTexture);


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data ImportGeometryModel data in JSON format
            /// @return Deserialized ImportGeometryModel
            static std::shared_ptr<ImportGeometryModel> FromJson(const json& data);
    };

    class ImportModel : public Model{
        private:
            std::filesystem::path _filePath;
            bool _lightingMaterial;

            std::vector<std::shared_ptr<Texture>> _loadedTextures;

            void initialize();
            void processNode(aiNode *node, const aiScene *scene);
            std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene);
            std::shared_ptr<Texture> loadMaterialTexture(aiMaterial *mat, aiTextureType type, TextureType typeName);

        public:
            ImportModel(std::filesystem::path filePath, bool lightingMaterial = false);


            /// @brief Get the model file path.
            /// @return Model file path 
            std::filesystem::path GetFilePath();


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data ImportModel data in JSON format
            /// @return Deserialized ImportModel
            static std::shared_ptr<ImportModel> FromJson(const json& data);
    };

}

#endif //MODEL_HPP