struct WindowData {
  bool keys[0x100] = {};
  bool is_active;
};

#undef CreateWindow

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static HWND CreateWindow(const char *name, int width, int height);
static void SetWindowData(HWND window, void *data);
static void PresentWindow(HWND window);
static POINT GetCursorPosition(HWND window);
static bool IsKeyDown(WindowData *window_data, char key);
static bool IsWindowActive(WindowData *window_data);