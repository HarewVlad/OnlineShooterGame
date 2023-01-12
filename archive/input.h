struct DirectInput {
  LPDIRECTINPUT8 input;
  IDirectInputDevice8* keyboard;
  IDirectInputDevice8* mouse;
  DIMOUSESTATE mouse_state;
  BYTE keyboard_state[256];
};

static void InitializeInput(DirectInput *direct_input, HWND window, HINSTANCE instance);
static void GetInput(DirectInput *direct_input);
static bool IsKeyDown(DirectInput *direct_input, int key);