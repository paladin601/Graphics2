
#include "ObjLoader.h"

extern Texture *textureIDS;

ObjLoader::ObjLoader()
{
}


ObjLoader::~ObjLoader()
{
}

Mesh * ObjLoader::load(string path)
{
	Mesh *mesh = new Mesh();
	Geometry *geo = NULL;
	ifstream file;
	glm::vec3 vertex, normal;
	glm::vec2 uv;
	string token, v, v1, v2;
	string n1, n2, n3, rest;
	vector<int> vertexIndices, uvIndices, normalIndices;
	vector< glm::vec3 > temp_vertex;
	vector< glm::vec2 > temp_uvs;
	vector< glm::vec3 > temp_normals;
	vector<float> aux;
	int max = 0;
	int num = 0;
	glm::vec3 pos1, pos2, pos3, tangent, bitangent, edge1, edge2;
	glm::vec2 uv1, uv2, uv3, deltaUV1, deltaUV2;
	int i;
	bool a = false;
	file.open(path, std::ios::in);
	while (!file.eof()) {
		file >> token;
		if (token == "o") {
			if (a) {
				geo->setSizeVertex(max);
				for (i = 0; i < max; i++) {
					geo->setFace(temp_vertex[vertexIndices[i]]);
					geo->setNormal(temp_normals[normalIndices[i]]);
					geo->setUV(temp_uvs[uvIndices[i]]);

					switch (num)
					{
					case 0:
						pos1 = temp_vertex[vertexIndices[i]];
						uv1 = temp_uvs[uvIndices[i]];
						num++;
						break;
					case 1:
						pos1 = temp_vertex[vertexIndices[i]];
						uv1 = temp_uvs[uvIndices[i]];
						num++;
						break;
					case 2:
						pos1 = temp_vertex[vertexIndices[i]];
						uv1 = temp_uvs[uvIndices[i]];
						num = 0;

						edge1 = pos2 - pos1;
						edge2 = pos3 - pos1;
						deltaUV1 = uv2 - uv1;
						deltaUV2 = uv3 - uv1;

						float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

						tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
						tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
						tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
						tangent = glm::normalize(tangent);

						bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
						bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
						bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
						bitangent = glm::normalize(bitangent);


						break;
					}
				}
				max = 0;
				vertexIndices.clear();
				normalIndices.clear();
				uvIndices.clear();
				mesh->setGeometry(geo);
			}
			a = true;
			geo = new Geometry();
			file >> v;
			geo->setName(v);
		}
		else {
			if (token == "v") {
				file >> v;
				file >> v1;
				file >> v2;
				vertex = glm::vec3(strtof(v.c_str(), 0), strtof(v1.c_str(), 0), strtof(v2.c_str(), 0));
				temp_vertex.push_back(vertex);
			}
			else {
				if (token == "vt") {
					file >> v;
					file >> v1;
					uv = glm::vec2(strtof(v.c_str(), 0), strtof(v1.c_str(), 0));
					temp_uvs.push_back(uv);
				}
				else {
					if (token == "vn") {
						file >> v;
						file >> v1;
						file >> v2;
						normal = glm::vec3(strtof(v.c_str(), 0), strtof(v1.c_str(), 0), strtof(v2.c_str(), 0));
						temp_normals.push_back(normal);
					}
					else {
						if (token == "f") {
							for (i = 0; i < 3; i++) {
								file >> v;
								n1 = v.substr(0, v.find("/"));
								vertexIndices.push_back(atoi(n1.c_str()) - 1);

								rest = v.substr(v.find("/") + 1, (v.length() - 1));
								n2 = rest.substr(0, rest.find("/"));
								uvIndices.push_back(atoi(n2.c_str()) - 1);

								n3 = rest.substr(rest.find("/") + 1, (rest.length() - 1));
								normalIndices.push_back(atoi(n3.c_str()) - 1);
							}
							max += 3;
						}
					}
				}
			}
		}

	}

	file.close();

	geo->setSizeVertex(max);
	for (i = 0; i < max; i++) {
		geo->setFace(temp_vertex[vertexIndices[i]]);
		geo->setNormal(temp_normals[normalIndices[i]]);
		geo->setUV(temp_uvs[uvIndices[i]]);
	}
	mesh->setGeometry(geo);
	mesh->setKD(1);
	mesh->setKS(1);
	return mesh;
}