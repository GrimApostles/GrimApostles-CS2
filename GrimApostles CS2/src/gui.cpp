#include "gui.h"
#include "sdk.h"

namespace gui {
	ID3D11Device* g_pd3dDevice = NULL;
	ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	IDXGISwapChain* g_pSwapChain = NULL;
	UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
	ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
	bool exitRequested = false;
	WNDCLASSEXW wc = {};
	HWND hwnd = NULL;
}

namespace maps {
	//You can change radar size based on your resolution. Set at 1080p right now for 1920x1080
	float radarSize = 1080;
	float vertigoZBound = 11700;
	float nukeZBound = -495;
	std::unordered_map<std::string, ID3D11ShaderResourceView*> mapTextures;
	std::unordered_map<std::string, mapData> mapBounds;
}

namespace icons {
	int id;
	float scale = 0.4f;
	std::unordered_map<int, ID3D11ShaderResourceView*> iconTextures;
	std::unordered_map<int, int> iconWidths;
	std::unordered_map<int, int> iconHeights;
}




//Init
void gui::initializeGUI() {
	// Create application window
	wc = { sizeof(wc), CS_VREDRAW | CS_HREDRAW, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"GrimApostles", nullptr };
	::RegisterClassExW(&wc);
	gui::hwnd = ::CreateWindowExW(WS_EX_APPWINDOW, wc.lpszClassName, L"GrimApostles CS2", WS_POPUP, 0, 0, 0, 0, nullptr, nullptr, wc.hInstance, nullptr);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClassW(wc.lpszClassName, wc.hInstance);
		return;
	}
	// Show the window
	::ShowWindow(hwnd, SW_MAXIMIZE);
	::UpdateWindow(hwnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

}

//All of our rendering
void gui::Render() {
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.f);
	CGame game;
	loadTextures();
	loadMapBounds();
	// Main loop
	bool done = false;
	while (!done)
	{
		// Poll and handle messages (inputs, window resize, etc.)
		// See the WndProc() function below for our to dispatch events to the Win32 backend.
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT || exitRequested) {
				done = true;
			}

		}
		if (done)
			break;

		// Handle window resize (we don't resize directly in the WM_SIZE handler)
		if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
			g_ResizeWidth = g_ResizeHeight = 0;
			CreateRenderTarget();
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//Where the magic happens
		gui::ConnectButton();
		gui::ExitButton();
		//Read in all info
		if (DMADevice::bConnected) {
			game.update();
			//Render
			gameLoop(game);
		}

		ImGui::Render();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(0, 0); // Present without vsync
	}


	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClassW(wc.lpszClassName, wc.hInstance);
	DMADevice::ShowKeyPress();
	return;
}
//Here is where we initialize our DMA and connect to process
void gui::ConnectButton() {
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 185, 35));
	ImGui::StyleColorsDark();
	ImGui::Begin("Connect", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	if (ImGui::Button(DMADevice::bConnected ? "Disconnect" : "Connect", { 150, 20 })) {
		if (!DMADevice::bConnected) {
			DMADevice::Connect();
			DMADevice::AttachToProcessId();
			DMADevice::moduleBase = DMADevice::getModuleBase(MODULE);
			std::cout << "\nProcess Information..." << std::endl;
			std::cout << "[DMA]:          CLIENT: " << PROCESS << std::endl;
			std::cout << "[DMA]:             PID: " << std::dec << DMADevice::dwAttachedProcessId << std::endl;
			std::cout << "[DMA]: CLIENT.DLL BASE: 0x" << std::hex << DMADevice::moduleBase << std::endl;
		}
		else {
			std::cout << "[DEBUG OUTPUT]: " << std::endl;
			//closing our vmm & scatter handle
			DMADevice::Disconnect();
		}
	}
	ImGui::End();
}
//Exit button
void gui::ExitButton() {
	//Setting up exit button 
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 185, 80));
	ImGui::StyleColorsDark();
	ImGui::Begin("EXIT", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	if (ImGui::Button("Exit", { 150 , 20 })) {
		exitRequested = true;
		DMADevice::Disconnect();
		std::cout << "Exiting Program... " << std::endl;

	}
	ImGui::End();
}




//Handles resizing, minimizing, exiting window
LRESULT WINAPI gui::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_GETMINMAXINFO:
		MINMAXINFO* minMaxInfo = (MINMAXINFO*)lParam;
		minMaxInfo->ptMinTrackSize.x = 960;  // Set minimum width
		minMaxInfo->ptMinTrackSize.y = 540;  // Set minimum height
		return 0;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);

}