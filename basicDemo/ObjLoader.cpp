
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
	vector<Geometry *> geometrys;
	vector<int> vertexIndices, uvIndices, normalIndices;
	vector< glm::vec3 > temp_vertex ,bitangentProm, tangentProm, bitangentCont,normalProm;
	vector< glm::vec2 > temp_uvs;
	vector< glm::vec3 > temp_normals;
	vector<float> aux;
	int max = 0;
	int contVert, contEnd;
	int num = 0;
	glm::vec3 pos1, pos2, pos3, tangent, bitangent, edge1, edge2, nl1,nl2,nl3;
	glm::vec2 uv1, uv2, uv3, deltaUV1, deltaUV2;
	int i,j,w;
	bool a = false;
	file.open(path, std::ios::in);
	while (!file.eof()) {
		file >> token;
		if (token == "o") {
			if (a) {
				geo->setSizeVertex(max);
				max = 0;
				geometrys.push_back(geo);
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
				bitangentCont.push_back(glm::vec3(0.0f));
				tangentProm.push_back(glm::vec3(0.0f));
				bitangentProm.push_back(glm::vec3(0.0f));
				normalProm.push_back(glm::vec3(0.0f));
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
	geometrys.push_back(geo);


	num = geometrys.size();

	contVert = 0;
	contEnd = 0;
	for (j = 0; j < num; j ++) {
		geo = geometrys[j];
		max = geo->getSizeVertex();
		contEnd += max;
		for (i = contVert; i < contEnd; i += 3) {
			pos1 = temp_vertex[vertexIndices[i]];
			pos2 = temp_vertex[vertexIndices[i + 1]];
			pos3 = temp_vertex[vertexIndices[i + 2]];

			uv1 = temp_uvs[uvIndices[i]];
			uv2 = temp_uvs[uvIndices[i + 1]];
			uv3 = temp_uvs[uvIndices[i + 2]];

			nl1 = temp_normals[normalIndices[i]];
			nl2 = temp_normals[normalIndices[i + 1]];
			nl3 = temp_normals[normalIndices[i + 2]];

			geo->setFace(pos1);
			geo->setFace(pos2);
			geo->setFace(pos3);

			geo->setUV(uv1);
			geo->setUV(uv2);
			geo->setUV(uv3);
			

			/*
			*/
			geo->setNormal(nl1);
			geo->setNormal(nl2);
			geo->setNormal(nl3);
			



			edge1 = pos2 - pos1;
			edge2 = pos3 - pos1;
			deltaUV1 = uv2 - uv1;
			deltaUV2 = uv3 - uv1;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);


			tangent = f * (edge1 * deltaUV2.y - edge2 * deltaUV1.y);

			bitangent = f * (edge2 * deltaUV1.x - edge1 * deltaUV2.x);

			geo->setTangent(tangent);
			geo->setTangent(tangent);
			geo->setTangent(tangent);
			geo->setBitangent(bitangent);
			geo->setBitangent(bitangent);
			geo->setBitangent(bitangent);


			/*
			normalProm[vertexIndices[i]] = nl1;
			normalProm[vertexIndices[i+1]] = nl2;
			normalProm[vertexIndices[i+2]] = nl3;


			tangentProm[vertexIndices[i]] = tangentProm[vertexIndices[i]]+tangent;
			tangentProm[vertexIndices[i+1]] = tangentProm[vertexIndices[i + 1]]+ tangent;
			tangentProm[vertexIndices[i+2]] = tangentProm[vertexIndices[i + 2]]+tangent;

			bitangentProm[vertexIndices[i]] =bitangentProm[vertexIndices[i]] + bitangent;
			bitangentProm[vertexIndices[i+1]] = bitangentProm[vertexIndices[i+1]]+bitangent;
			bitangentProm[vertexIndices[i+2]] = bitangentProm[vertexIndices[i+2]]+bitangent;

			bitangentCont[vertexIndices[i]] += glm::vec3(1.0f);
			bitangentCont[vertexIndices[i + 1]] += glm::vec3(1.0f);
			bitangentCont[vertexIndices[i + 2]] += glm::vec3(1.0f);
			*/
			
		}
		contVert += max;
		/*
		*/
		mesh->setGeometry(geo);
	}
	/*
	contVert = 0;
	contEnd = 0;
	for (j = 0; j < num; j++) {
		geo = geometrys[j];
		max= geo->getSizeVertex();
		contEnd += max;
		for (i = contVert; i < contEnd; i ++) {
			tangent = tangentProm[vertexIndices[i]] / bitangentCont[vertexIndices[i]];
			geo->setTangent(tangent);
			bitangent = bitangentProm[vertexIndices[i]] / bitangentCont[vertexIndices[i]];
			geo->setBitangent(bitangent);
			normal= normalProm[vertexIndices[i]] / bitangentCont[vertexIndices[i]];
			geo->setNormal(normal);
		}
		contVert += max;
		mesh->setGeometry(geo);
	}
	*/
	
	

	temp_normals.clear();
	temp_uvs.clear();
	temp_vertex.clear();
	vertexIndices.clear();
	normalIndices.clear();
	uvIndices.clear();
	tangentProm.clear();
	bitangentProm.clear();
	normalProm.clear();
	bitangentCont.clear();


	mesh->setKD(1);
	mesh->setKS(1);
	return mesh;
}