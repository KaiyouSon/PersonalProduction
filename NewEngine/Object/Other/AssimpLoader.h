#pragma once
//#include <assimp/mesh.h>

class AssimpLoader
{
private:
	//void ParseMesh(aiMesh* mesh);	// メッシュの解析

public:
	void Init();

	void LoadFbxModel();

};

