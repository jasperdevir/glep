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

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <GLEP/core/utility/file.hpp>
#include <GLEP/core/utility/print.hpp>
#include <GLEP/core/utility/math.hpp>

#include <vector>
#include <string>
#include <sstream>

#include <nlohmann/json.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GLEP{

    using json = nlohmann::ordered_json;

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
        glm::vec3 Tangent;

        Vertex(){};

        Vertex(glm::vec3 position){
            Position = position;
        }

        Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord, glm::vec3 tangent){
            Position = position;
            Normal = normal;
            TexCoord = texCoord;
            Tangent = tangent;
        };

        /// @brief Serialize data to JSON format.
        /// @return Serialized data
        json ToJson(){
            json j;
            j["position"] = Math::ToJson(Position);
            j["normal"] = Math::ToJson(Normal);
            j["tex_coord"] = Math::ToJson(TexCoord);
            j["tangent"] = Math::ToJson(Tangent);
            return j;
        }

        /// @brief Deserialize data from JSON format.
        /// @param data Vertex data in JSON format
        /// @return Deserialized Vertex
        static Vertex FromJson(const json& data){
            return Vertex(
                Math::Vec3FromJson(data["position"]),
                Math::Vec3FromJson(data["normal"]),
                Math::Vec2FromJson(data["tex_coord"]),
                Math::Vec3FromJson(data["tangent"])
            );
        }
    };

    class Geometry{
        protected:
            unsigned int _VAO;
            unsigned int _VBO;
            unsigned int _EBO;

            bool _hasInit = false;

            std::vector<Vertex> _vertices;
            std::vector<unsigned int> _indices;

            void initialize();
            void bindVertices();
            void bindIndices();
            
        public:
            /// @brief Generate vertices of a procedual plane generated at the origin.
            /// @param width Total width of vertices
            /// @param height Total height of vertices
            /// @param depth Total depth of vertices
            /// @param widthSegments Amount of quad segments along the plane's width
            /// @param heightSegments Amount of quad segments along the plane's height
            /// @return Generated vertices
            static std::vector<Vertex> GeneratePlaneVertices(float width, float height, float depth, int widthSegments, int heightSegments);

            /// @brief Generate indices for a procedual plane.
            /// @param widthSegments Number of width segments in plane vertices
            /// @param heightSegments Number of width segments in plane vertices
            /// @param offset Added offset to each index
            /// @param swap Swap the winding order of each triangle
            /// @return Generated indices
            static std::vector<unsigned int> GeneratePlaneIndices(int widthSegments, int heightSegments, int offset, bool swap = false);

            Geometry();
            Geometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
            ~Geometry();


            /// @brief Get vertex data
            /// @return Vertex data
            std::vector<Vertex> GetVertices();

            /// @brief Get index data
            /// @return Index data
            std::vector<unsigned int> GetIndices();


            /// @brief Set vertex data, rebinding it.
            /// @param vertices Vertex data to set
            void SetVertices(std::vector<Vertex> vertices);

            /// @brief Set index data, rebinding it.
            /// @param indices Index data to set
            void SetIndices(std::vector<unsigned int> indices);


            /// @brief If data has been initialized, bind the vertex array, and draw the elements as triangles based on its indices.
            virtual void Draw();

            /// @brief Calculate normals for each vertex based on their position.
            void CalculateNormals();

            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            virtual json ToJson();

            /// @brief Deserialized data from JSON format.
            /// @param data Geometry data in JSON format
            /// @return Deserialized data
            static std::shared_ptr<Geometry> FromJson(const json& data);
    };

    class ImportGeometry{
        private: 
            std::filesystem::path _filePath;
            std::vector<std::shared_ptr<Geometry>> _geometry;

            bool _normalCalculationNeeded = false;

            void initialize();
            void processNode(aiNode *node, const aiScene *scene);
            std::shared_ptr<Geometry> processGeometry(aiMesh *mesh, const aiScene *scene);

        public:
            ImportGeometry(std::filesystem::path filePath);

            /// @brief Get the generated geometry data.
            /// @return Geometry data
            std::vector<std::shared_ptr<Geometry>>& GetGeometry();

            /// @brief Get the model file path.
            /// @return Model file path
            std::filesystem::path GetFilePath();


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson();

            /// @brief Deserialize data from JSON format.
            /// @param data ImportGeometry data in JSON format
            /// @return Deserialized ImportGeometry
            static std::shared_ptr<ImportGeometry> FromJson(const json& data);

    };

    class CubeGeometry : public Geometry{
        private:
            float _width;
            float _height;
            float _depth;
            int _widthSegments;
            int _heightSegments;
            int _depthSegments;

            void generate();

        public:
            CubeGeometry(float width, float height, float depth, int widthSegments = 1, int heightSegments = 1, int depthSegments = 1);


            /// @brief Get generated width.
            /// @return Width
            float GetWidth();

            /// @brief Get generated height.
            /// @return Height
            float GetHeight();

            /// @brief Get generated depth.
            /// @return Depth
            float GetDepth();

            /// @brief Get generated width segments.
            /// @return Width segments
            int GetWidthSegments();

            /// @brief Get generated height segments.
            /// @return Height segments
            int GetHeightSegments();

            /// @brief Get generated depth segments.
            /// @return Depth segments
            int GetDepthSegments();


            /// @brief Regenerate geometry.
            /// @param width Total width of the generated cube
            /// @param height Total height of the generated cube
            /// @param depth Total depth of the generated cube
            /// @param widthSegments Amount of quad segments generated along the cube's width
            /// @param heightSegments Amount of quad segments generated along the cube's height
            /// @param depthSegments Amount of quad segments generated along the cube's depth
            void Regenerate(float width, float height, float depth, int widthSegments = 1, int heightSegments = 1, int depthSegments = 1);


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data CubeGeometry data in JSON format
            /// @return Deserialized CubeGeometry
            static std::shared_ptr<CubeGeometry> FromJson(const json& data);
    };

    class PlaneGeometry : public Geometry{
        private:
            float _width;
            float _height;
            int _widthSegments;
            int _heightSegments;

            void generate();

        public:
            PlaneGeometry(float width, float height, int widthSegments = 1, int heightSegments = 1);

            /// @brief Get generated width
            /// @return Width
            float GetWidth();

            /// @brief Get generated height
            /// @return Height
            float GetHeight();

            /// @brief Get generated width segments
            /// @return Width segments
            int GetWidthSegments();

            /// @brief Get generated height segments
            /// @return Height segments
            int GetHeightSegments();


            /// @brief Regenerate geometry.
            /// @param width Total width of the generated plane
            /// @param height Total height of the generated plane
            /// @param widthSegments Amount of quad segments generated along the plane's width
            /// @param heightSegments Amount of quad segments generated along the plane's height
            void Regenerate(float width, float height, int widthSegments = 1, int heightSegments = 1);


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data PlaneGeometry data in JSON format
            /// @return Deserialized PlaneGeometry
            static std::shared_ptr<PlaneGeometry> FromJson(const json& data);
    };

    class GridGeometry : public Geometry{
        private:
            float _width;
            float _height;
            int _widthSegments;
            int _heightSegments;

            void generate();
        public:
            GridGeometry(float width, float height, int widthSegments, int heightSegments);

            /// @brief Get the generated width
            /// @return Width
            float GetWidth();

            /// @brief Get the generated height
            /// @return Height
            float GetHeight();

            /// @brief Get the generated width segments
            /// @return Width segments
            int GetWidthSegments();

            /// @brief Get the generated height segments
            /// @return Height segments
            int GetHeightSegments();


            /// @brief Regenerate geometry.
            /// @param width Total width of the generated grid
            /// @param height Total height of the generated grid
            /// @param widthSegments Amount of quad segments generated along the grid's width
            /// @param heightSegments Amount of quad segments generated along the grid's height
            void Regenerate(float width, float height, int widthSegments = 1, int heightSegments = 1);


            /// @brief If data has been initialized, bind the vertex array, and draw the elements as lines based on its indices.
            void Draw() override;


            /// @brief Serialize data to JSON format.
            /// @return Serialize data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data GridGeometry data in JSON format
            /// @return Deserialized GridGeometry
            static std::shared_ptr<GridGeometry> FromJson(const json& data);
    };

    class LineGeometry : public Geometry {
        public:
            LineGeometry(glm::vec3 startPoint, glm::vec3 endPoint);

            /// @brief Get the start point of the line.
            /// @return Start point
            glm::vec3 GetStartPoint();

            /// @brief Get the end point of the line.
            /// @return End point
            glm::vec3 GetEndPoint();

            /// @brief Set the start point of the line.
            /// @param startPoint Start point to set
            void SetStartPoint(glm::vec3 startPoint);

            /// @brief Set the end point of the line.
            /// @param endPoint End point to set
            void SetEndPoint(glm::vec3 endPoint);

            /// @brief Regnerate geometry.
            /// @param startPoint Start point to set
            /// @param endPoint End point to set
            void Regenerate(glm::vec3 startPoint, glm::vec3 endPoint);

            /// @brief If data has been initialized, bind the vertex array, and draw a line from the start point to the end point.
            void Draw() override;

            /// @brief Serialize data to JSON format.
            /// @return Serailized data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data LineGeometry data in JSON format
            /// @return Deserialized LineGeometry
            static std::shared_ptr<LineGeometry> FromJson(const json& data);
    };

    class IcosphereGeometry : public Geometry {
        private:
            float _radius;
            unsigned int _subdivision;

            void generate();

        public:
            IcosphereGeometry(float radius, unsigned int subdivision);

            /// @brief Get the generated radius.
            /// @return Radius
            float GetRadius();

            /// @brief Get the generated subdivision level.
            /// @return Subdivision level
            unsigned int GetSubdivision();


            /// @brief Regenerate geometry.
            /// @param radius Radius of the sphere
            /// @param subdivision Subdivision level
            void Regenerate(float radius, unsigned int subdivision);


            /// @brief Serialize data to JSON format.
            /// @return Serailized data 
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data IcosphereGeometry data in JSON format
            /// @return Deserialized IcosphereGeometry
            static std::shared_ptr<IcosphereGeometry> FromJson(const json& data);
    };

}

#endif //GEOMETRY_HPP