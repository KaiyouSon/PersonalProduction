#pragma once
#include "Texture.h"
#include "DepthTexture.h"
#include "VolumeTexture.h"
#include "TextureAnimation.h"
#include "RenderTexture.h"
#include "Singleton.h"
#include "NewEngineEnum.h"
#include <DirectXTex.h>
#include <mutex>
#include <unordered_map>

template<typename T> class Singleton;

class AssetsManager;

// テクスチャーを管理するクラス
class TextureManager : public Singleton<TextureManager>
{
private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextureMap;
	std::unordered_map<std::string, std::unique_ptr<Texture>> mMaterialTextureMap;
	std::unordered_map<std::string, std::unique_ptr<DepthTexture>> mDepthTextureMap;
	std::unordered_map<std::string, std::unique_ptr<VolumeTexture>> mVolumeTextureMap;
	std::unordered_map<std::string, std::unique_ptr<RenderTexture>> mRenderTextureMap;

#pragma region new
private:
	std::array<std::unordered_map<std::string, std::unique_ptr<ITexture>>, (uint32_t)TextureType::Count> mTextureMapArrays;
	std::mutex mMutex;	// 排他制御

public:
	void LoadTexture(const std::string& path);
	ITexture* LoadMaterialTexture(const std::string& path);
	ITexture* LoadMaterialTexture(const std::wstring& wpath);
	std::unique_ptr<RenderTexture> CreateRenderTexture(const Vec2 size = Vec2(1920, 1080));

private:
	void LoadTextureFromPNG(const std::string filePath, DirectX::ScratchImage& scratchImg, DirectX::TexMetadata& metadata);
	void LoadTextureFromDDS(const std::string filePath, DirectX::ScratchImage& scratchImg, DirectX::TexMetadata& metadata);
	void LoadTextureFromPNG(const std::wstring wfilePath, DirectX::ScratchImage& scratchImg, DirectX::TexMetadata& metadata);
	void LoadTextureFromDDS(const std::wstring wfilePath, DirectX::ScratchImage& scratchImg, DirectX::TexMetadata& metadata);
#pragma endregion

public:	// 生成関連

	// テクスチャーをロード
	static void LoadTexture(const std::string filePath, const std::string tag);

	// mtlファイルのテクスチャーをロード
	static Texture* LoadMaterialTexture(const std::string filePath, const std::string tag);

	// 1x1の色テクスチャーを作成
	static void CreateColorTexture(const Color color, const std::string tag);

	// 深度テクスチャの作成
	static void CreateDepthTexture(DepthBuffer* depthBuffer, const std::string tag);

	// ボリュームテクスチャのロード
	static void LoadVolumeTexture(const std::string filePath, const std::string tag);

	// ボリュームテクスチャの作成
	static void CreateVolumeTexture(const std::vector<Texture*>& texs, const std::string tag);

	// レンダーテクスチャの作成
	static void CreateRenderTexture(const Vec2 size, const std::string tag);
	static void CreateRenderTexture(const RenderTextureSetting setting, const std::string tag);

public: // 破棄関連

	// テクスチャの破棄
	static void DestroyTexture(const std::string tag);

	// マテリアルのテクスチャの破棄
	static void DestroyMaterialTexture(const std::string tag);

	// 深度テクスチャの破棄
	static void DestroyDepthTexture(const std::string tag);

	// ボリュームテクスチャの破棄
	static void DestroyVolumeTexture(const std::string tag);

	// レンダーテクスチャの破棄
	static void DestroyRenderTexture(const std::string tag);

public: // テクスチャの取得関連

	// テクスチャーを取得
	static Texture* GetTexture(const std::string tag);

	// 深度テクスチャーを取得
	static DepthTexture* GetDepthTexture(const std::string tag);

	// ボリュームテクスチャを取得
	static VolumeTexture* GetVolumeTexture(const std::string tag);

	// レンダーテクスチャを取得
	static RenderTexture* GetRenderTexture(const std::string tag);

public:	// マップの取得関連

	// テクスチャーマップを取得
	static std::unordered_map<std::string, std::unique_ptr<Texture>>* GetTextureMap();

	// マテリアルテクスチャーマップを取得
	static std::unordered_map<std::string, std::unique_ptr<Texture>>* GetMaterialTextureMap();

	// 深度テクスチャーマップを取得
	static std::unordered_map<std::string, std::unique_ptr<DepthTexture>>* GetDepthTextureMap();

	// ボリュームテクスチャーマップを取得
	static std::unordered_map<std::string, std::unique_ptr<VolumeTexture>>* GetVolumeTextureMap();

	// レンダーテクスチャーマップを取得
	static std::unordered_map<std::string, std::unique_ptr<RenderTexture>>* GetRenderTextureMap();

public:
	// コマンド実行
	static void ExcuteComandList();


private:
	friend AssetsManager;

private:
	// 繧ｷ繝ｳ繧ｰ繝ｫ繝医Φ
	friend Singleton<TextureManager>;
	TextureManager();
};
