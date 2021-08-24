#pragma once

#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif

#ifdef __UNIX__
#include <string.h>
#endif

#include <fstream>
#include <string>
#include <vector>

#include "vec3.hpp"

static bool load_obj_file_basic(const char* filePath,
                                std::vector<glm::vec3>& vertices,
                                std::vector<glm::vec2>& texturecoords,
                                std::vector<glm::vec3>& normals)
{
	FILE *f = fopen(filePath, "r");
	if(f == NULL){
		fprintf(stderr, "Failed to find obj file");
		return false;
	}

	std::vector<unsigned int> vertex_indices, texture_indices, normal_indices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_textureCords;
	std::vector<glm::vec3> temp_normals;

	for(;;){
		char lineHeader[128];
		int result = fscanf(f, "%s", lineHeader);

		if(result == EOF) break;

		if(strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(f, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if(strcmp(lineHeader, "vt") == 0){
			glm::vec2 textureCord;
			fscanf(f, "%f %f", &textureCord.x, &textureCord.y);
			temp_textureCords.push_back(textureCord);
		}
		else if(strcmp(lineHeader, "vn") == 0){
			glm::vec3 normal;
			fscanf(f, "%f %f %f", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if(strcmp(lineHeader, "f") == 0){
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
								 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
								 &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}

			vertex_indices.push_back(vertexIndex[0]);
			vertex_indices.push_back(vertexIndex[1]);
			vertex_indices.push_back(vertexIndex[2]);

			texture_indices.push_back(uvIndex[0]);
			texture_indices.push_back(uvIndex[1]);
			texture_indices.push_back(uvIndex[2]);

			normal_indices.push_back(normalIndex[0]);
			normal_indices.push_back(normalIndex[1]);
			normal_indices.push_back(normalIndex[2]);
		}		
	}

	for(unsigned int i = 0; i < vertex_indices.size(); i++){
		unsigned int vertexIndex = vertex_indices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		vertices.push_back(vertex);
	}

	for(unsigned int i = 0; i < texture_indices.size(); i++){
		unsigned int textureIndex = texture_indices[i];
		glm::vec2 textureCord = temp_textureCords[textureIndex - 1];
		texturecoords.push_back(textureCord);
	}

	for(unsigned int i = 0; i < normal_indices.size(); i++){
		unsigned int normalIndex = normal_indices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		normals.push_back(normal);
	}
	
	return true;
}
