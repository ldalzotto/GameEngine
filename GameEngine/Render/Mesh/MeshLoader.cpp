#include "MeshLoader.h"

#include <stdexcept>

extern "C"
{
#include "DataStructures/Specifications/String.h"
#include "Algorithm/String/StringAlgorithm.h"
}

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "MYLog/MYLog.h"

namespace _GameEngine::_Render
{
	struct LoadedData
	{
		std::vector<Vertex> Vertices;
		std::vector<uint16_t> Indices;
	};

	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<LoadedData>* meshes);

	void MeshLoader_readMeshFromFile(ReadMeshFromFileInfo* p_readMeshFromFileInfo)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(p_readMeshFromFileInfo->Path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			{
				Core_GenericArray l_errorMessage; Core_string_alloc(&l_errorMessage, 100);
				Core_string_append(&l_errorMessage, "Cannot load the model : ");
				Core_string_append(&l_errorMessage, (char*)p_readMeshFromFileInfo->Path.c_str());
				throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE_STRING(&l_errorMessage));
			}
		}

		std::vector<LoadedData> loadedData;
		ProcessNode(scene->mRootNode, scene, &loadedData);
		import.FreeScene();

		if (loadedData.size() > 0)
		{
			*p_readMeshFromFileInfo->Vertices = loadedData.at(0).Vertices;
			*p_readMeshFromFileInfo->Indices = loadedData.at(0).Indices;
		}

	};

	void ProcessMesh(aiMesh* mesh, const aiScene* scene, LoadedData* p_loadedData) {

		p_loadedData->Vertices.resize(mesh->mNumVertices);

		// Vertex data
		for (size_t i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			aiVector3D& verticeVector = mesh->mVertices[i];
			vertex.pos = { verticeVector.x, verticeVector.y, verticeVector.z };

			if (mesh->mTextureCoords) {
				aiVector3D& texCoordVector = mesh->mTextureCoords[0][i];
				vertex.TexCoord = { texCoordVector.x, texCoordVector.y };
			}
			else {
				vertex.TexCoord = { 0.0f };
			}

			p_loadedData->Vertices.at(i) = vertex;
		}

		// Indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				p_loadedData->Indices.push_back(face.mIndices[j]);
			}
		}

	}

	void ProcessNode(aiNode* node, const aiScene* scene, std::vector<LoadedData>* meshes) {

		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			LoadedData l_loadedData{};
			ProcessMesh(mesh, scene, &l_loadedData);
			meshes->push_back(l_loadedData);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene, meshes);
		}
	}



}