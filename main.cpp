#include "includes.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;




//globals

static bool menu = true; // Due to automatic
static bool menu2 = true; //Cael is the one and only.
char ExecBuf[99999] = "print('Hello World!')";
char Dumppath[99999] = "C://Desudo//is//a//Paste";



void LuaExecStyle() {
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowMinSize = ImVec2(440, 265);
	style->WindowRounding = 0.f;
	style->FrameRounding = 0.f;
};


void ResourceStyle() {
	ImGuiStyle* style = &ImGui::GetStyle();
	style->WindowMinSize = ImVec2(440,265);
	style->WindowRounding = 0.f;
	style->FrameRounding = 0.f;
}



void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 14);
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (menu) {
		LuaExecStyle();
		ImGui::Begin("Lua Executor",nullptr,ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		ImGui::InputTextMultiline("##Skid", ExecBuf, IM_ARRAYSIZE(ExecBuf), ImVec2(425, 210));
		if (ImGui::Button("Execute")) {
			//Triggers funcs lmfao
		};

		ImGui::End();
	}
	if (menu2) {
		ResourceStyle();
		ImGui::Begin("Resources",nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
		if (ImGui::BeginChild("##UglyResourceTabe", ImVec2(425, 210))) {
			ImGui::Text("Press Save to dump available client files.");

		

	


			ImGui::EndChild();
		};
		if (ImGui::Button("Save")) {

		};
		ImGui::SameLine();
		ImGui::InputText("##DUMPPATH", Dumppath, IM_ARRAYSIZE(Dumppath));

		ImGui::End();
	};





	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		//CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		MainThread();
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
