#include <d3d9.h>
#include <wrl\client.h>

#pragma comment (lib, "d3d9.lib")

using namespace Microsoft::WRL;

ComPtr<IDirect3D9> d3d;
ComPtr<IDirect3D9Ex> d3dEx;

ComPtr<IDirect3DDevice9> device;
ComPtr<IDirect3DDevice9Ex> deviceEx;

void KeyDown(UINT8 key) {
	if (GetAsyncKeyState(VK_ESCAPE))
		exit(0);
}

void KeyUp(UINT8 key) {

}

void Clear() {
	d3d.ReleaseAndGetAddressOf();
	d3dEx.ReleaseAndGetAddressOf();
	device.ReleaseAndGetAddressOf();
	deviceEx.ReleaseAndGetAddressOf();
}

void Update() {

}

void Render() {
	if (deviceEx) {
		deviceEx->BeginScene();
		deviceEx->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(100, 149, 237, 1), 1.0f, 0);
		deviceEx->EndScene();
		deviceEx->Present(NULL, NULL, NULL, NULL);
		return;
	}
	else if (device) {
		device->BeginScene();
		device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(100, 149, 237, 1), 1.0f, 0);
		device->EndScene();
		device->Present(NULL, NULL, NULL, NULL);
		return;
	}
	else {
		MessageBox(NULL, "Something went wrong.",
			"Error", MB_ICONERROR);
		exit(-1);
	}
}

void Init(_In_ HWND hWnd) {
	D3DPRESENT_PARAMETERS desc = {};
	desc.BackBufferCount = 2;
	desc.BackBufferFormat = D3DFMT_A8R8G8B8;
	desc.SwapEffect = D3DSWAPEFFECT_DISCARD;
	desc.hDeviceWindow = hWnd;
	desc.Windowed = TRUE;

	if (SUCCEEDED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3dEx))) {
		d3dEx->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &desc, NULL, &deviceEx);
		return;
	}
	else if (d3d = Direct3DCreate9(D3D_SDK_VERSION)) {
		d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, NULL,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &desc, &device);
		return;
	}
	else {
		MessageBox(NULL, "Your GPU doesn't support DirectX 9.",
			"Error", MB_ICONERROR);
		exit(-1);
	}
}

LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:
		Init(hWnd);
		return 0;

	case WM_KEYDOWN:
		KeyDown(static_cast<UINT8>(wParam));
		return 0;

	case WM_KEYUP:
		KeyUp(static_cast<UINT8>(wParam));
		return 0;

	case WM_PAINT:
		Update();
		Render();
		return 0;

	case WM_DESTROY:
		Clear();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";
	RegisterClassEx(&wc);
	HWND hWnd = CreateWindow(wc.lpszClassName, "Test DirectX 9 for CreateDevice \\ CreateDeviceEx",
		WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN) - 1024) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - 576) / 2,
		1024, 576, nullptr, nullptr, hInstance, 0);
	ShowWindow(hWnd, nCmdShow);
	MSG msg = {};
	while (msg.message != WM_QUIT)
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	return static_cast<char>(msg.wParam);
}
