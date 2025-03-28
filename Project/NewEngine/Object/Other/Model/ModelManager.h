#pragma once
#include "Model.h"
#include "ObjModel.h"
#include "FbxModel.h"
#include "Singleton.h"
#include <unordered_map>

// 前方宣言
template<typename T> class Singleton;

// モデルを管理するクラス
class ModelManager : public Singleton<ModelManager>
{
private:
	std::unordered_map<std::string, std::unique_ptr<Model>> mModelMap; // モデルのマップ
	std::mutex mMutex;	// 排他制御
	std::string mDirectoryPath;

#pragma region new

public:
	void LoadModel(const std::string fileName, const bool isSmoothing = false);
	void LoadModel(const std::wstring fileName, const bool isSmoothing = false);

	Model* GetModel2(const std::string& tag);
	std::unordered_map<std::string, std::unique_ptr<Model>>* GetModelMap();

#pragma endregion

public: // ロード関連

	// objモデルのロード
	static Model* LoadObjModel(const std::string fileName, const std::string tag, const bool isSmoothing = false);

	// fbxモデルのロード
	static Model* LoadFbxModel(const std::string fileName, const std::string tag);

	// マテリアルのロード
	static void LoadMtlFile(std::string filePath, Model* model);

public: // 破棄関連

	// モデルの破棄
	static void DestroyModel(const std::string tag);

public:	// モデルの取得関連
	static Model* GetModel(std::string tag);

public: // モデルのマップの取得関連
	static std::unordered_map<std::string, std::unique_ptr<Model>>* GetModelMap2();

public:
	friend Singleton<ModelManager>;
	ModelManager();
};
