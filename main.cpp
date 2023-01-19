#include "include/imgui/imgui.cpp"
#include "include/imgui/imgui_demo.cpp"
#include "include/imgui/imgui_draw.cpp"
#include "include/imgui/imgui_tables.cpp"
#include "include/imgui/imgui_widgets.cpp"
#include "include/imgui/imgui_impl_win32.cpp"
#include "include/imgui/imgui_impl_dx11.cpp"

#include "main.h"
#include "utils.cpp"
#include "collision.cpp"
#include "camera.cpp"
#include "state.cpp"
#include "interface.cpp"
#include "window.cpp"
#include "shader.cpp"
#include "directx.cpp"
#include "mesh.cpp"
#include "object.cpp"
#include "weapon.cpp"
#include "enemy.cpp"
#include "player.cpp"
#include "map.cpp"
#include "bullets.cpp"
#include "client.cpp"
#include "server.cpp"
#include "dummy.cpp"
#include "animation.cpp"
#include "game.cpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd) {
  SetRandomSeed();

  Game game;
  InitializeGame(&game, hInstance);
  s_State = State_Menu;
  Run(&game);

  return 0;
}