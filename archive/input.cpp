static void InitializeInput(DirectInput *direct_input, HWND window, HINSTANCE instance) {
  HRESULT result = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8,
        (void**)&direct_input->input,
        NULL);
  assert(result == S_OK);

  result = direct_input->input->CreateDevice(GUID_SysKeyboard, &direct_input->keyboard, NULL);
  assert(result == S_OK);

  result = direct_input->input->CreateDevice(GUID_SysMouse, &direct_input->mouse, NULL);
  assert(result == S_OK);

  result = direct_input->keyboard->SetDataFormat(&c_dfDIKeyboard);
  result = direct_input->keyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

  result = direct_input->mouse->SetDataFormat(&c_dfDIMouse);
  result = direct_input->mouse->SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
}

static void GetInput(DirectInput *direct_input) {
  direct_input->keyboard->Acquire();
  direct_input->mouse->Acquire();

  direct_input->keyboard->GetDeviceState(256, (LPVOID)&direct_input->keyboard_state);
  direct_input->mouse->GetDeviceState(sizeof(DIMOUSESTATE), &direct_input->mouse_state);
}

static bool IsKeyDown(DirectInput *direct_input, int key) {
  return direct_input->keyboard_state[key] & 0x80;
}