extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
                                                             UINT msg,
                                                             WPARAM wParam,
                                                             LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    return true;

  WindowData *window_data = (WindowData *)GetWindowLongPtr(hWnd, GWLP_USERDATA);

  switch (msg) {
  case WM_SIZE: {
    // We never resize, just gigachad fullscreen from the beginning
  }
    return 0;
  case WM_SYSCOMMAND:
    if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
      return 0;
    break;
  case WM_SETFOCUS:
    window_data->is_active = true;
    break;
  case WM_KILLFOCUS:
    window_data->is_active = false;
    break;
  case WM_KEYDOWN:
    window_data->keys[wParam & 0xFF] = true;
    break;
  case WM_KEYUP:
    window_data->keys[wParam & 0xFF] = false;
    break;
  case WM_LBUTTONDOWN:
    window_data->keys[MK_LBUTTON] = true;
    break;
  case WM_LBUTTONUP:
    window_data->keys[MK_LBUTTON] = false;
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

static HWND CreateWindow(const char *name, int width, int height) {
  WNDCLASSEX wc = {sizeof(WNDCLASSEX),
                   CS_CLASSDC,
                   WndProc,
                   0L,
                   0L,
                   GetModuleHandle(NULL),
                   NULL,
                   NULL,
                   NULL,
                   NULL,
                   name,
                   NULL};
  RegisterClassEx(&wc);

  HWND result =
      CreateWindowA(wc.lpszClassName, name, WS_POPUP, 0, 0,
                    width, height, NULL, NULL, wc.hInstance, NULL);

  return result;
}

static void PresentWindow(HWND window) {
  ShowWindow(window, SW_SHOWDEFAULT);
  UpdateWindow(window);
}

static void SetWindowData(HWND window, void *data) {
  SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)data);
}

static POINT GetCursorPosition(HWND window) {
  POINT result;
  GetCursorPos(&result);
  ScreenToClient(window, &result);
  return result;
}

static bool IsWindowActive(WindowData *window_data) {
  return window_data->is_active;
}

static bool IsKeyDown(WindowData *window_data, char key) {
  return window_data->keys[key];
}