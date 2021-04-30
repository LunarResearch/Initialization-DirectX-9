#include <d3d9.h>
#include <wrl\client.h>

#pragma comment (lib, "d3d9")

using namespace Microsoft::WRL;

ComPtr<IDirect3D9> d3d;
ComPtr<IDirect3D9Ex> d3dEx;

ComPtr<IDirect3DDevice9> device;
ComPtr<IDirect3DDevice9Ex> deviceEx;

void KeyDown(UINT8 key)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		exit(0);
}

void KeyUp(UINT8 key)
{

}

void Clear()
{
	d3d.ReleaseAndGetAddressOf();
	d3dEx.ReleaseAndGetAddressOf();
	device.ReleaseAndGetAddressOf();
	deviceEx.ReleaseAndGetAddressOf();
}

void Update()
{

}

void Render()
{
	if (deviceEx) {
		deviceEx->BeginScene();
		deviceEx->Clear(NULL, nullptr, D3DCLEAR_TARGET, D3DCOLOR_RGBA(100, 149, 237, 1), 1.0f, NULL);
		deviceEx->EndScene();
		deviceEx->Present(nullptr, nullptr, nullptr, nullptr);
		return;
	}
	else if (device) {
		device->BeginScene();
		device->Clear(NULL, nullptr, D3DCLEAR_TARGET, D3DCOLOR_RGBA(100, 149, 237, 1), 1.0f, NULL);
		device->EndScene();
		device->Present(nullptr, nullptr, nullptr, nullptr);
		return;
	}
	else {
		MessageBox(nullptr, TEXT("Something went wrong."), TEXT("Error"), MB_ICONERROR);
		exit(-1);
	}
}

void Init(_In_ HWND hWnd)
{
	D3DPRESENT_PARAMETERS desc{};
	desc.BackBufferCount = 2;
	desc.BackBufferFormat = D3DFMT_A8R8G8B8;
	desc.SwapEffect = D3DSWAPEFFECT_DISCARD;
	desc.hDeviceWindow = hWnd;
	desc.Windowed = TRUE;

	if (SUCCEEDED(Direct3DCreate9Ex(D3D_SDK_VERSION, &d3dEx))) {
		d3dEx->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, nullptr, D3DCREATE_HARDWARE_VERTEXPROCESSING, &desc, nullptr, &deviceEx);
		return;
	}
	else if (d3d = Direct3DCreate9(D3D_SDK_VERSION)) {
		d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, nullptr, D3DCREATE_HARDWARE_VERTEXPROCESSING, &desc, &device);
		return;
	}
	else {
		MessageBox(nullptr, TEXT("Your GPU doesn't support DirectX 9."), TEXT("Error"), MB_ICONERROR);
		exit(-1);
	}
}

LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (message)
	{
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

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PTCHAR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASS wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = TEXT("WindowClass");
	RegisterClass(&wc);
	
	auto hWnd = CreateWindow(wc.lpszClassName, TEXT("Test DirectX 9 for CreateDeviceEx \\ CreateDevice"),
		WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN) - 1024) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - 576) / 2,
		1024, 576, nullptr, nullptr, hInstance, NULL);
	
	ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	MSG msg{};
	while (msg.message != WM_QUIT)
		if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	return (int)msg.wParam;
}
