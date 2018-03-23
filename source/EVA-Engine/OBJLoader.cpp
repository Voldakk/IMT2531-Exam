#include "OBJLoader.hpp"

#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>

#include <glm/glm.hpp>

#include "Mesh.hpp"


std::vector<std::shared_ptr<Mesh>> OBJLoader::Load(const char * path)
{
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Mesh>> empty;

	std::vector<unsigned int> finVertIdx, finUvIdx, finNormIdx;

	std::vector<glm::vec3> tmpVert;
	std::vector<glm::vec2> tmpUv;
	std::vector<glm::vec3> tmpNorms;

	FILE *file = fopen(path, "r");

	if (file == nullptr)
	{
		printf("OBJLoader::Load - Unable to open file: %s", path);
		return empty;
	}

	char symb[100];
	char buffer[1000];
	std::string name;

	while (true)
	{
		const auto res = fscanf(file, "%s", symb);

		if (res == EOF) 
		{
			break;
		}

		if (strcmp(symb, "o") == 0)
		{
			// Save the current mesh
			if(!tmpVert.empty())
			{
				meshes.push_back(CreateMesh(name, tmpVert, tmpUv, tmpNorms, finVertIdx, finUvIdx, finNormIdx));

				finVertIdx.clear();
				finUvIdx.clear();
				finNormIdx.clear();
			}

			// Read the name of the new mesh
			fscanf(file, "%s", &buffer);
			name = buffer;
			std::cout << "OBJLoader::Load - Loading object: " << name << std::endl;
		}
		else if (strcmp(symb, "v") == 0) 
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tmpVert.push_back(vertex);
		}
		else if (strcmp(symb, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tmpUv.push_back(uv);
		}
		else if (strcmp(symb, "vn") == 0) 
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmpNorms.push_back(normal);
		}
		else if (strcmp(symb, "f") == 0) 
		{
			unsigned int vertIdx[3], uvIdx[3], nnIdx[3];

			const auto suc = fscanf(file, 
				"%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertIdx[0], &uvIdx[0], &nnIdx[0],
				&vertIdx[1], &uvIdx[1], &nnIdx[1],
				&vertIdx[2], &uvIdx[2], &nnIdx[2]);

			if (suc != 9) 
			{
				printf("Wrong format for face specification.");
				fclose(file);
				return empty;
			}
			finVertIdx.push_back(vertIdx[0]);
			finVertIdx.push_back(vertIdx[1]);
			finVertIdx.push_back(vertIdx[2]);

			finUvIdx.push_back(uvIdx[0]);
			finUvIdx.push_back(uvIdx[1]);
			finUvIdx.push_back(uvIdx[2]);

			finNormIdx.push_back(nnIdx[0]);
			finNormIdx.push_back(nnIdx[1]);
			finNormIdx.push_back(nnIdx[2]);

		}
		else 
		{
			//discard
			fgets(buffer, 1000, file);
		}
	}

	fclose(file);

	// Save the current mesh
	if (!tmpVert.empty())
		meshes.push_back(CreateMesh(name, tmpVert, tmpUv, tmpNorms, finVertIdx, finUvIdx, finNormIdx));

	return meshes;
}

std::shared_ptr<Mesh> OBJLoader::CreateMesh(const std::string& name, 
	std::vector<glm::vec3> tmpVert, std::vector<glm::vec2> tmpUv, std::vector<glm::vec3> tmpNorms, 
	std::vector<unsigned int> finVertIdx, std::vector<unsigned int> finUvIdx, std::vector<unsigned int> finNormIdx)
{

	std::vector<Vertex> vertices;

	for (unsigned int i = 0; i < finVertIdx.size(); i++)
	{
		const auto vertIdx = finVertIdx[i];
		const auto uvIdx = finUvIdx[i];
		const auto nnIdx = finNormIdx[i];

		const auto vertex = tmpVert[vertIdx - 1];
		const auto uv = tmpUv[uvIdx - 1];
		const auto normals = tmpNorms[nnIdx - 1];

		vertices.push_back({ vertex, normals, uv });
	}

	return std::make_shared<Mesh>(vertices, name);
}