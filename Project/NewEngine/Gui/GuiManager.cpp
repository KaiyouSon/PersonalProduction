#include "GuiManager.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "TextureManager.h"
#include "Util.h"
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

using namespace Gui;

void Gui::Init()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(RenderWindow::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(
		renderBase->GetDevice(),
		sNumFramesInFlight,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap(),
		DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
}

void Gui::PreDraw()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::NewFrame();
}

void Gui::PostDraw()
{
	ImGui::Render();
	// SRVヒープセット
	RenderBase* renderBase = RenderBase::GetInstance();
	auto srvDescHeap = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &srvDescHeap);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), renderBase->GetCommandList());
}

void Gui::Destroy()
{
	// 破棄
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool Gui::BeginWindow(const char* name, const Vec2& size, bool* isOpen)
{
	if (size.x != -1 && size.y != -1)
	{
		ImGui::SetNextWindowSize({ size.x,size.y });

		ImGuiWindowFlags windowFlags =
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoResize;
		return ImGui::Begin(name, isOpen, windowFlags);
	}

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar;
	return ImGui::Begin(name, isOpen, windowFlags);
}

void Gui::BeginFullWindow(const char* name)
{
	// 繧ｦ繧｣繝ｳ繝峨え縺ｮ險ｭ螳・
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoTitleBar |                // タイトルバーを非表示にする
		ImGuiWindowFlags_NoResize |                 // ウィンドウのリサイズを禁止する
		ImGuiWindowFlags_NoMove |                   // ウィンドウの移動を禁止する
		ImGuiWindowFlags_MenuBar |                  // メニューバーを表示する
		ImGuiWindowFlags_NoBringToFrontOnFocus |    // フォーカス時にウィンドウを前面に持ってこない
		ImGuiWindowFlags_NoCollapse;                // ウィンドウの折り畳みを禁止する

	static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
	{
		windowFlags |= ImGuiWindowFlags_NoBackground;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(name, nullptr, windowFlags);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags);
	}
}

void Gui::EndFullWindow()
{
	ImGui::End();
	ImGui::PopStyleVar();
}

void Gui::OpenPopModal(const char* tag)
{
	ImGui::OpenPopup(tag);
}

void Gui::ClosePopModal()
{
	ImGui::CloseCurrentPopup();
}

bool Gui::BeginPopModal(const char* tag)
{
	return ImGui::BeginPopupModal(tag);
}

void Gui::EndPopModal()
{
	ImGui::EndPopup();
}

bool Gui::DrawCollapsingHeader(const char* name)
{
	return ImGui::CollapsingHeader(name);
}

bool Gui::BeginMenuBar()
{
	return ImGui::BeginMenuBar();
}

void Gui::EndMenuBar()
{
	ImGui::EndMenuBar();
}

bool Gui::BeginMenu(const char* name)
{
	return ImGui::BeginMenu(name);
}

void Gui::EndMenu()
{
	ImGui::EndMenu();
}

bool Gui::MenuItem(const char* name)
{
	return ImGui::MenuItem(name);
}

void Gui::EndWindow()
{
	ImGui::End();
}

void Gui::DrawDemoWindow(bool& flag)
{
	ImGui::ShowDemoWindow(&flag);
}

bool Gui::DrawButton(const char* label, const Vec2& size)
{
	return ImGui::Button(label, { size.x,size.y });
}

void Gui::DrawTab()
{
	ImGui::SameLine();
}

void Gui::DrawColumns(uint32_t space, const bool& isBorder)
{
	ImGui::Columns(space, 0, isBorder);
}

void Gui::NextColumn()
{
	ImGui::NextColumn();
}

void Gui::DrawLine()
{
	ImGui::Separator();
}

void Gui::DrawString(const char* fmt, ...)
{
	ImGui::Text(fmt);
}

void Gui::DrawCheckBox(const char* label, bool* flag)
{
	ImGui::Checkbox(label, flag);
}

bool Gui::DrawRadioButton(const char* label, uint32_t* current, const uint32_t index, const bool isTab)
{
	bool flag = ImGui::RadioButton(label, (int*)current, (int)index);
	if (isTab == true)
	{
		ImGui::SameLine();
	}

	return flag;
}

void Gui::DrawSlider1(const char* label, float& v, const float& moveSpeed)
{
	ImGui::DragFloat(label, &v, moveSpeed);
}

void Gui::DrawSlider2(const char* label, Vec2& v, const float& moveSpeed)
{
	float temp[2] = { v.x,v.y };
	ImGui::DragFloat2(label, temp, moveSpeed);
	v.x = temp[0]; v.y = temp[1];
}

void Gui::DrawSlider3(const char* label, Vec3& v, const float& moveSpeed)
{
	float temp[3] = { v.x,v.y,v.z };
	ImGui::DragFloat3(label, temp, moveSpeed);
	v.x = temp[0];	v.y = temp[1];	v.z = temp[2];
}

void Gui::DrawColorEdit(const char* label, Color& color)
{
	float temp[4] = { color.r / 255,color.g / 255,color.b / 255,color.a / 255 };
	ImGui::ColorEdit4(label, temp);
	color.r = temp[0] * 255; color.g = temp[1] * 255; color.b = temp[2] * 255; color.a = temp[3] * 255;
}

bool Gui::DrawInputInt(const char* label, int32_t& v)
{
	bool flag = ImGui::InputInt(label, &v);
	return flag;
}

bool Gui::DrawInputText(const char* label, std::string& str)
{
	char* cstr = const_cast<char*>(str.c_str());
	bool result = ImGui::InputText(label, cstr, 30);
	str = cstr;

	return result;
	//return ImGui::InputText(label, const_cast<char*>(str.c_str()), 30);
}

void Gui::DrawImage(ITexture* texture, const Vec2& size)
{
	if (texture == nullptr) return;

	if (texture->GetTextureType() != TextureType::Render)
	{
		ImTextureID gpuHandle = (ImTextureID)texture->GetBufferResource()->srvHandle.gpu.ptr;
		ImVec2 textureSize = { size.x,size.y };
		ImGui::Image(gpuHandle, textureSize);
	}
	else
	{
		RenderTexture* renderTex = dynamic_cast<RenderTexture*>(texture);
		for (uint32_t i = 0; i < renderTex->GetBufferResources()->size(); i++)
		{
			ImTextureID gpuHandle = (ImTextureID)renderTex->GetBufferResources()->at(i).srvHandle.gpu.ptr;
			ImVec2 textureSize = { size.x,size.y };
			ImGui::Image(gpuHandle, textureSize);
		}
	}
}

void Gui::DrawImage(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle, const Vec2& size)
{
	ImTextureID handle = (ImTextureID)gpuHandle.ptr;
	ImVec2 textureSize = { size.x,size.y };
	ImGui::Image(handle, textureSize);
}

bool Gui::DrawImageButton(ITexture* texture, const Vec2& size)
{
	if (texture == nullptr) return false;

	ImTextureID gpuHandle = (ImTextureID)texture->GetBufferResource()->srvHandle.gpu.ptr;
	ImVec2 buttonSize = { size.x,size.y };
	return ImGui::ImageButton(gpuHandle, buttonSize);
}

