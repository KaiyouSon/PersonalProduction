#include "AssetsManager.h"
namespace fs = std::filesystem;
using namespace DirectX;

AssetsManager* gAssetsManager = nullptr;

AssetsManager::AssetsManager() :
	mMaterialManager(std::make_unique<MaterialManager>())
{
}

void AssetsManager::LoadAssets(const std::string& sceneName)
{
	mTextureManager.mTextureMapArrays[(uint32_t)TextureType::Default].clear();

	// テクスチャのロード
	LoadTextures(AppTextureDirectory + sceneName);

	// マテリアルのロード
	LoadMaterials(EngineMaterialDirectory);	// 一回エンジンのロード
	mMaterialManager->LoadMaterial(EngineMaterialDirectory + "BasicSpriteMaterial.json");
}

void AssetsManager::LoadMaterials(const std::string folderPath)
{
	fs::path fsFolderPath = folderPath;
	if (!fs::is_directory(fsFolderPath))
	{
		return;
	}

	for (const auto& entry : fs::directory_iterator(fsFolderPath))
	{
		if (fs::is_directory(entry))
		{
			LoadMaterials(entry.path().string());
		}
		else
		{
			fs::path path = entry.path();
			LoadMaterial(path.string());
		}
	}
}

void AssetsManager::LoadTextures(const std::string& folderPath)
{
	fs::path fsFolderPath = folderPath;
	if (!fs::is_directory(fsFolderPath))
	{
		return;
	}

	for (const auto& entry : fs::directory_iterator(fsFolderPath))
	{
		if (fs::is_directory(entry))
		{
			LoadTextures(entry.path().string());
		}
		else
		{
			fs::path path = entry.path();
			LoadTexture(path.string());
		}
	}
}

void AssetsManager::LoadModels(const std::string& folderPath)
{
	fs::path fsFolderPath = folderPath;
	if (!fs::is_directory(fsFolderPath))
	{
		return;
	}

	for (const auto& entry : fs::directory_iterator(fsFolderPath))
	{
		if (fs::is_directory(entry))
		{
			LoadModels(entry.path().string());
		}
		else
		{
			fs::path path = entry.path();
			LoadModel(path.string());
		}
	}
}

Texture* AssetsManager::GetTexture(const std::string& tag)
{
	Texture* tex = dynamic_cast<Texture*>(mTextureManager.mTextureMapArrays[(uint32_t)TextureType::Default][tag].get());
	return tex;
}

Material* AssetsManager::GetMaterial(const std::string& tag)
{
	return mMaterialManager->GetMaterial(tag);
}

std::unordered_map<std::string, std::unique_ptr<ITexture>>* AssetsManager::GetTextureMap(const TextureType texType)
{
	return &mTextureManager.mTextureMapArrays[(uint32_t)texType];
}

void AssetsManager::LoadMaterial(const std::string& path)
{
	mMaterialManager->LoadMaterial(path);
}

void AssetsManager::LoadTexture(const std::string& path)
{
	mTextureManager.LoadTexture(path);
}

void AssetsManager::LoadModel(const std::string& path)
{
	path;
	//mModelManager.LoadModel(path);
}

void LoadTexture(const std::string& path)
{
	SceneManager::GetInstance()->mCurrentScene->GetAssetsManager()->LoadTexture(path);
}
