#pragma once
#include "Singleton.h"

// 前方宣言
class HierarchyWindow;
class InspectorWindow;
class AssetsWindow;
class ViewWindow;
template<typename T> class Singleton;

class MainWindow : public Singleton<MainWindow>
{
private:
	std::unique_ptr<HierarchyWindow> mHierarchyWindow;
	std::unique_ptr<InspectorWindow> mInspectorWindow;
	std::unique_ptr<AssetsWindow> mAssetsWindow;
	std::unique_ptr<ViewWindow> mViewWindow;

private:
	std::string mCurrentObjName;
	bool mIsActive;

public:
	void SaveSceneData();

public:
	void DrawDebugGui();

public:
	// セッター
	void SetCurrentObjName(const std::string currentObjName);

public:
	// ゲッター
	std::string GetCurrentObjName();

private:
	friend Singleton<MainWindow>;
	MainWindow();
};
