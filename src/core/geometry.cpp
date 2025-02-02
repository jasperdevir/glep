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

#include <GLEP/core/geometry.hpp>

namespace GLEP {

    Geometry::Geometry(){}

    Geometry::Geometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices){
        _vertices = vertices;
        _indices = indices;

        initialize();
    }

    Geometry::~Geometry(){
        glDeleteVertexArrays(1, &_VAO);
        glDeleteBuffers(1, &_VBO);
        glDeleteBuffers(1, &_EBO);
    }

    void Geometry::initialize(){
        glGenVertexArrays(1, &_VAO);
        glGenBuffers(1, &_VBO);
        glGenBuffers(1, &_EBO);

        glBindVertexArray(_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

        //WorldPosition
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        //Normal
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        //TexCoords
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
        //Tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

        _hasInit = true;

        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0); 
    }

    std::vector<Vertex> Geometry::GetVertices() { return _vertices; }
    std::vector<unsigned int> Geometry::GetIndices() { return _indices; }

    void Geometry::SetVertices(std::vector<Vertex> vertices){
        _vertices = vertices;
        bindVertices();
    }

    void Geometry::SetIndices(std::vector<unsigned int> indices){
        _indices = indices;
        bindIndices();
    }

    void Geometry::bindVertices(){
        glBindVertexArray(_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);
    }

    void Geometry::bindIndices(){
        glBindVertexArray(_VAO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);
    }

    void Geometry::Draw(){
        if(!_hasInit) return;
        glBindVertexArray(_VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(_indices.size()), GL_UNSIGNED_INT, 0);
    }

    void Geometry::CalculateNormals(){
        //NORMALS
        for (int i = 0; i < _indices.size(); i += 3) {
            unsigned int index0 = _indices[i];
            unsigned int index1 = _indices[i + 1];
            unsigned int index2 = _indices[i + 2];

            glm::vec3 pos0 = _vertices[index0].Position;
            glm::vec3 pos1 = _vertices[index1].Position;
            glm::vec3 pos2 = _vertices[index2].Position;

            glm::vec3 edge1 = pos1 - pos0;
            glm::vec3 edge2 = pos2 - pos0;

            glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

            _vertices[index0].Normal = faceNormal;
            _vertices[index1].Normal = faceNormal;
            _vertices[index2].Normal = faceNormal;

            glm::vec2 uv0 = _vertices[index0].TexCoord;
            glm::vec2 uv1 = _vertices[index1].TexCoord;
            glm::vec2 uv2 = _vertices[index2].TexCoord;

            glm::vec2 deltaUV1 = uv1 - uv0;
            glm::vec2 deltaUV2 = uv2 - uv0; 

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            glm::vec3 tangent = glm::vec3(
                f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
                f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
                f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
            );

            _vertices[index0].Tangent = tangent;
            _vertices[index1].Tangent = tangent;
            _vertices[index2].Tangent = tangent;
        }

        for (Vertex v : _vertices) {
            if (glm::length(v.Normal) > 0.0f) {
                v.Normal = glm::normalize(v.Normal);
            }
        }

        glBindVertexArray(_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);
    }

    json Geometry::ToJson(){
        json j; 
        j["type"] = "geometry";

        j["vertices"] = json::array();
        for(auto& v : _vertices){
            j["vertices"].push_back(v.ToJson());
        }

        j["indices"] = json::array();
        for(auto& i : _indices){
            j["indices"].push_back(i);
        }

        return j;
    }

    std::shared_ptr<Geometry> Geometry::FromJson(const json& data){
        std::shared_ptr<Geometry> result;

        if(data["type"] == "geometry"){
            std::vector<Vertex> vertices;
            for(int i = 0; i < data["vertices"].size(); i++){
                vertices.push_back(Vertex::FromJson(data["vertices"][i]));
            }

            std::vector<unsigned int> indices;
            for(int i = 0; i < data["indices"].size(); i++){
                indices.push_back(data["indices"][i]);
            }

            result = std::make_shared<Geometry>(vertices, indices);
        } else if(data["type"] == "cube_geometry"){
            result = CubeGeometry::FromJson(data);
        } else if(data["type"] == "plane_geometry"){
            result = PlaneGeometry::FromJson(data);
        } else if (data["type"] == "grid_geometry"){
            result = GridGeometry::FromJson(data);
        } else {
            Print(PrintCode::ERROR, "GEOMETRY", "Unknown Geometry type: " + data["type"]);
            return nullptr;
        }

        return result;
    }

    ImportGeometry::ImportGeometry(std::filesystem::path filePath){
        _filePath = filePath;

        initialize();
    }

    void ImportGeometry::initialize(){
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(_filePath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            Print(PrintCode::ERROR, "GEOMETRY", "Assimp Error: \n" + std::string(import.GetErrorString()));
            return;
        }

        processNode(scene->mRootNode, scene);

        if(_normalCalculationNeeded){
            for(auto& g : _geometry){
                g->CalculateNormals();
            }
        }

    }

    void ImportGeometry::processNode(aiNode *node, const aiScene *scene)
    {
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
            _geometry.push_back(processGeometry(mesh, scene));			
        }

        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    } 

    std::shared_ptr<Geometry> ImportGeometry::processGeometry(aiMesh *mesh, const aiScene *scene){
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
                if(!_normalCalculationNeeded) _normalCalculationNeeded = true;
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
                if(!_normalCalculationNeeded) _normalCalculationNeeded = true;
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

        return std::make_shared<Geometry>(vertices, indices);
    }

    std::vector<std::shared_ptr<Geometry>>& ImportGeometry::GetGeometry(){
        return _geometry;
    }

    std::filesystem::path ImportGeometry::GetFilePath(){
        return _filePath;
    }

    json ImportGeometry::ToJson(){
        json j;
        j["type"] = "import_geometry";
        j["path"] = _filePath;
        return j;
    }

    std::shared_ptr<ImportGeometry> ImportGeometry::FromJson(const json& data){
        return std::make_shared<ImportGeometry>(
            data["path"]
        );
    }

    CubeGeometry::CubeGeometry(float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments) {
        _width = width;
        _height = height;
        _depth = depth;
        _widthSegments = widthSegments;
        _heightSegments = heightSegments;
        _depthSegments = depthSegments;

        generate();
        initialize();
    }

    json CubeGeometry::ToJson(){
        json j;
        j["type"] = "cube_geometry";
        j["width"] = _width;
        j["height"] = _height;
        j["depth"] = _depth;
        j["width_segments"] = _widthSegments;
        j["height_segments"] = _heightSegments;
        j["depth_segments"] = _depthSegments;
        return j;
    }

    std::shared_ptr<CubeGeometry> CubeGeometry::FromJson(const json& data){
        return std::make_shared<CubeGeometry>(
            data["width"], 
            data["height"], 
            data["depth"], 
            data["width_segments"], 
            data["height_segments"], 
            data["depth_segments"]
        );
    }

    void CubeGeometry::generate(){
        _vertices.clear();
        _indices.clear();

        //Front
        std::vector<unsigned int> frontIndices = GeneratePlaneIndices(_widthSegments, _heightSegments, (int)_vertices.size());

        _indices.insert(_indices.end(), frontIndices.begin(), frontIndices.end());

        std::vector<Vertex> frontFace = GeneratePlaneVertices(_width, _height, _depth, _widthSegments, _heightSegments);
        for (Vertex vertex : frontFace) {
            vertex.Position.z += 0.5f * _depth; 
            vertex.Position.z -= _depth;
            vertex.Normal = glm::vec3(0.0f, 0.0f, -1.0f);
            vertex.TexCoord = glm::vec2(vertex.Position.x / _width + 0.5f, vertex.Position.y / _height + 0.5f);
            _vertices.push_back(vertex);
        }
        
        //Back
        std::vector<unsigned int> backIndices = GeneratePlaneIndices(_widthSegments, _heightSegments, (int)_vertices.size(), true);
        _indices.insert(_indices.end(), backIndices.begin(), backIndices.end());

        std::vector<Vertex> backFace = GeneratePlaneVertices(_width, _height, _depth, _widthSegments, _heightSegments);
        for (Vertex vertex : backFace) {
            vertex.Position.z -= 0.5f * _depth;
            vertex.Position.z -= _depth;
            vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
            vertex.TexCoord = glm::vec2(vertex.Position.x / _width + 0.5f, vertex.Position.y / _height + 0.5f);
            _vertices.push_back(vertex);
        }
        
        //Left
        std::vector<unsigned int> leftIndices = GeneratePlaneIndices(_depthSegments, _heightSegments, (int)_vertices.size());
        _indices.insert(_indices.end(), leftIndices.begin(), leftIndices.end());
        
        std::vector<Vertex> leftFace = GeneratePlaneVertices(_depth, _height, _width, _depthSegments, _heightSegments);
        for (Vertex vertex : leftFace) {
            float x = vertex.Position.x;
            vertex.Position.x = vertex.Position.z - 0.5f * _width;
            vertex.Position.x -= _width; 
            vertex.Position.z = x; 
            vertex.Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
            vertex.TexCoord = glm::vec2(vertex.Position.y / _height + 0.5f, vertex.Position.z / _depth + 0.5f);
            _vertices.push_back(vertex);
        }

        //Right
        std::vector<unsigned int> rightIndices = GeneratePlaneIndices(_depthSegments, _heightSegments,(int)_vertices.size(), true);
        _indices.insert(_indices.end(), rightIndices.begin(), rightIndices.end());
        
        std::vector<Vertex> rightFace = GeneratePlaneVertices(_depth, _height, _width, _depthSegments, _heightSegments);
        for (Vertex vertex : rightFace) {
            float x = vertex.Position.x;
            vertex.Position.x = vertex.Position.z + 0.5f * _width; 
            vertex.Position.x -= _width; 
            vertex.Position.z = x;
            vertex.Normal = glm::vec3(1.0f, 0.0f, 0.0f);
            vertex.TexCoord = glm::vec2(vertex.Position.y / _height + 0.5f, vertex.Position.z / _depth + 0.5f);
            _vertices.push_back(vertex);
        }
        
        //Top
        std::vector<unsigned int> topIndices = GeneratePlaneIndices(_widthSegments, _depthSegments, (int)_vertices.size());
        _indices.insert(_indices.end(), topIndices.begin(), topIndices.end());
        
        std::vector<Vertex> topFace = GeneratePlaneVertices(_width, _depth, _height, _widthSegments, _depthSegments);
        for (Vertex vertex : topFace) {
            float y = vertex.Position.y;
            vertex.Position.y = vertex.Position.z - 0.5f * _height; 
            vertex.Position.y -= _height;
            vertex.Position.z = y + 1.0f * _depth;
            vertex.Position.z -= _depth;
            vertex.Normal = glm::vec3(0.0f, -1.0f, 0.0f); 
            vertex.TexCoord = glm::vec2(vertex.Position.x / _width + 0.5f, vertex.Position.z / _depth + 0.5f);
            _vertices.push_back(vertex);
        }

        //Bottom
        std::vector<unsigned int> botIndices = GeneratePlaneIndices(_widthSegments, _depthSegments, (int)_vertices.size());
        _indices.insert(_indices.end(), botIndices.begin(), botIndices.end());

        std::vector<Vertex> bottomFace = GeneratePlaneVertices(_width, _depth, _height, _widthSegments, _depthSegments);
        for (Vertex vertex : bottomFace) {
            float y = vertex.Position.y;
            vertex.Position.y = vertex.Position.z + 0.5f * _height;
            vertex.Position.y -= _height;
            vertex.Position.z = -y + 1.0f * _depth;
            vertex.Position.z -= _depth;
            vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);  
            vertex.TexCoord = glm::vec2(vertex.Position.x / _width + 0.5f, vertex.Position.z / _depth + 0.5f);
            _vertices.push_back(vertex);
        }
        
    }

    float CubeGeometry::GetWidth() { return _width; }
    float CubeGeometry::GetHeight() { return _height; }
    float CubeGeometry::GetDepth() { return _depth; }
    int CubeGeometry::GetWidthSegments() { return _widthSegments; }
    int CubeGeometry::GetHeightSegments() { return _heightSegments; }
    int CubeGeometry::GetDepthSegments() { return _depthSegments; }

    void CubeGeometry::Regenerate(float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments){
        _width = width;
        _height = height;
        _depth = depth;
        _widthSegments = widthSegments;
        _heightSegments = heightSegments;
        _depthSegments = depthSegments;

        generate();
        bindVertices();
        bindIndices();
    }

    PlaneGeometry::PlaneGeometry(float width, float height, int widthSegments, int heightSegments) {
        _width = width;
        _height = height;
        _widthSegments = widthSegments;
        _heightSegments = heightSegments;

        generate();
        initialize();
        CalculateNormals();

    }

    void PlaneGeometry::generate(){
        _vertices = GeneratePlaneVertices(_width, _height, 0.0f, _widthSegments, _heightSegments);
        _indices = GeneratePlaneIndices(_widthSegments, _heightSegments, 0);
    }

    float PlaneGeometry::GetWidth() { return _width; }
    float PlaneGeometry::GetHeight() { return _height; }
    int PlaneGeometry::GetWidthSegments() { return _widthSegments; }
    int PlaneGeometry::GetHeightSegments() { return _heightSegments; }

    void PlaneGeometry::Regenerate(float width, float height, int widthSegments, int heightSegments){
        _width = width;
        _height = height;
        _widthSegments = widthSegments;
        _heightSegments = heightSegments;

        generate();
        CalculateNormals();
        bindIndices();
    }

    json PlaneGeometry::ToJson(){
        json j;
        j["type"] = "plane_geometry";
        j["width"] = _width;
        j["height"] = _height;
        j["width_segments"] = _widthSegments;
        j["height_segments"] = _heightSegments;
        return j;
    }

    std::shared_ptr<PlaneGeometry> PlaneGeometry::FromJson(const json& data){
        return std::make_shared<PlaneGeometry>(
            data["width"], 
            data["height"], 
            data["width_segments"], 
            data["height_segments"]
        );
    }

    std::vector<Vertex> Geometry::GeneratePlaneVertices(float width, float height, float depth, int widthSegments, int heightSegments){
        std::vector<Vertex> vertices;
        float stepX = (float)width / widthSegments;
        float stepY = (float)height / heightSegments;

        for(int y = 0; y <= heightSegments; ++y){
            for(int x = 0; x <= widthSegments; ++x){
                float posX = -0.5f * width + x * stepX;
                float posY = 0.5f * height - y * stepY;
                float posZ = depth;
                
                glm::vec2 texCoords = glm::vec2(posX/ width + 0.5f, posY / height + 0.5f);
                vertices.push_back(Vertex(glm::vec3(posX, posY, posZ), glm::vec3(0.0f), texCoords, glm::vec3(0.0f)));
            }
        }

        return vertices;
    }

    std::vector<unsigned int> Geometry::GeneratePlaneIndices(int widthSegments, int heightSegments, int offset, bool swap){
        std::vector<unsigned int> indices;
        for(int y = 0; y < heightSegments; ++y){
            for(int x = 0; x < widthSegments; ++x){
                int topLeft = y * (widthSegments + 1) + x + offset;
                int topRight = y * (widthSegments + 1) + (x + 1) + offset;
                int bottomLeft = (y + 1) * (widthSegments + 1) + x + offset;
                int bottomRight = (y + 1) * (widthSegments + 1) + (x + 1) + offset;

                int first = swap ? topRight : topLeft;
                int second = bottomLeft;
                int third = swap ? topLeft : topRight;

                int fourth = swap ? bottomLeft : bottomRight;
                int fifth = topRight;
                int sixth = swap ?  bottomRight : bottomLeft;

                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(third);

                indices.push_back(fourth);
                indices.push_back(fifth);
                indices.push_back(sixth);
        
            }
        }
        return indices;
    }

    GridGeometry::GridGeometry(float width, float height, int widthSegments, int heightSegments){
        _width = width;
        _height = height;
        _widthSegments = widthSegments;
        _heightSegments = heightSegments;

        generate();
        initialize();
    }

    void GridGeometry::generate(){
        for(int j = 0; j <= _heightSegments; ++j) {
            for(int i = 0; i <= _widthSegments; ++i) {
                float x = (float)i/(float)_widthSegments - 0.5f;
                float y = (float)j/(float)_heightSegments - 0.5f;
                float z = 0.0f;

                x *= _width;
                y *= _height;
                
                _vertices.push_back(Vertex(glm::vec3(x, y, z), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
            }
        }

        for(int j = 0; j < _heightSegments; ++j) {
            for(int i = 0; i < _widthSegments; ++i) {

                int row1 =  j    * (_widthSegments+1);
                int row2 = (j+1) * (_heightSegments+1);

                _indices.push_back(row1+i);
                _indices.push_back(row1+i+1);
                _indices.push_back(row1+i+1);
                _indices.push_back(row2+i+1);
                _indices.push_back(row2+i+1);
                _indices.push_back(row2+i);
                _indices.push_back(row2+i);
                _indices.push_back(row1+i);
            }
        }
    }

    float GridGeometry::GetWidth() { return _width; }
    float GridGeometry::GetHeight() { return _height; }
    int GridGeometry::GetWidthSegments() { return _widthSegments; }
    int GridGeometry::GetHeightSegments() { return _heightSegments; }

    void GridGeometry::Draw(){
        glBindVertexArray(_VAO);
        glDrawElements(GL_LINES, (int)_indices.size() * 4, GL_UNSIGNED_INT, NULL);
    }

    json GridGeometry::ToJson(){
        json j;
        j["type"] = "grid_geometry";
        j["width"] = _width;
        j["height"] = _height;
        j["width_segments"] = _widthSegments;
        j["height_segments"] = _heightSegments;
        return j;
    }

    std::shared_ptr<GridGeometry> GridGeometry::FromJson(const json& data){
        return std::make_shared<GridGeometry>(
            data["width"], 
            data["height"], 
            data["width_segments"], 
            data["height_segments"]
        );
    }
}