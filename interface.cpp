static void InitializeImGuiWin32(HWND window) {
  ImGui::CreateContext();
  ImGui_ImplWin32_Init(window);
}

static void ImGuiBegin() {
  ImGui_ImplDX11_NewFrame();
  ImGui_ImplWin32_NewFrame();
  ImGui::NewFrame();
}

static void ImGuiEnd() {
  ImGui::Render();
  ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

static void DrawHpBar(float hp, float max_hp, ImVec4 color) {
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, color);
  ImGui::ProgressBar(hp / max_hp, ImVec2(100.0f, 0.0f));
  ImGui::PopStyleColor();
}