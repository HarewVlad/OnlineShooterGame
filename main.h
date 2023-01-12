#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <timeapi.h>
#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>
#include <random>
#include <assert.h>
#include <memory>

// Asio
#include <asio.hpp>

// TODO: Later add this to includes to do this - #include <stb/stb_image.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_DS_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_ds.h>

// TODO: Later add this to includes
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>

using namespace DirectX;
using asio::ip::tcp;

#include "utils.h"
#include "config.h"
#include "collision.h"
#include "camera.h"
#include "state.h"
#include "interface.h"
#include "constant_buffer.h"
#include "window.h"
#include "shader.h"
#include "directx.h"
#include "vertex.h"
#include "mesh.h"
#include "object.h"
#include "weapon.h"
#include "player.h"
#include "enemy.h"
#include "map.h"
#include "bullets.h"
#include "connection.h"
#include "client.h"
#include "server.h"
#include "dummy.h"
#include "game.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// TODO: Find a way to zoom camera (Viewport? Manipulation object sizes?)

// TODO: Guns (https://apokalips123.itch.io/2d-weapon-generator generator for random ranged weapons) and bullets (particle system)
// For now I'will use https://fxcreator.itch.io/pixel-weapon as pixel weapons
// TODO: Create image storage
// TODO: Store vertex and pixel shaders somewhere else, as they did not interact with anything else at all?
// TODO: Replace assert with my assert so i can remove them in release build
// TODO: Add paralax background on menu
// TODO: Make config menu
// TODO: Charge power shoot when spinning gun
// TODO: Add speed boost when pressing shift
// TODO: Add parallax background



// TODO(Bonus): Make labirinth instead of what i have now, texture walls, and make player able to jump through walls?
// TODO(Bonus): Add shop and economics system
// TODO(Bonus): Map generation via Wave Function Collapse
// TDOO(Bonus): Add explode effect to guns
// TODO(Bonus): Square rotation based on the mouse position
// TDOO(Bonus): Add movement with slide
// TODO(Bonus): Create bouncy objects, that you can bounce of to mb avoid bullets
// TODO(Bonus): Make cheat codes based on move pattern
// TODO(Bonus): Make chat with opponent
// TODO(Bonus): Make cheat that makes you produce a lot of shaking cubes that covers whole map and kills you opponent
// TODO(Bonus): Make hats (Glasses for cube, wizard hat)
// TODO(Bonus): Cheat code that adds you some insanely powerfull weapon
// TODO(Bonus): Add unkillable boss that will instantly kill player when met him
// TODO(Bonus): Add cool lightning
// TODO(Bonus): Buffer updates to 1 per frame
// TODO(Bonus): Start bullet position is barrel muzzle not center of weapon

// TODO(Low): When body is agains 2 bodies, it squizes through both of them, even if there is not enough space to go through

// TODO(Super low): Add blockchain to shop and stuff

// 22-27.11.2022
// DONE: Make Sampler2D and and load weapon image
// DONE: Rotate weapon toward mouse direction
// DONE: Shot bullets (Particles)
// DONE: Make bullets disappear at some distance
// DONE: Change object sizes constants to XMFLOAT2 then
// TODO(Low): Record video of moving in Walk repository (Cubes)

// 28.11.2022
// TODO: Add bullets type
// TODO: Add rubber bullet type
// TODO: Replace assert with my assert so i can remove them in release build
// DONE: Add training map (lmao)
// DONE: Add server and client in simple way
// DONE: Add shift to player
// DONE: Add ingame interface that shows when shifts are available
// DONE: Refactor window.h and window.cpp
// DONE: Pass parameters like size and so on, not make them default in structs or use them implicetely in methods

// 21.12.2022
// DONE: Refactor Window.h (Create window separate from the data?)
// DONE: Recieve player 2 position through the server
// DONE: Draw player 2

// 27-28.12.2022
// DONE: Store all shaders and input layouts in stb::map
// DONE: Small shader creation code modification
// DONE: Display player coordinates on enemy's machine (Add additional field to connection?)
// DONE: Remove repeated fuctions from InitliazeShaders function
// TODO: Pass weapon rotation, weapon type (Different texture)

// 04.01.2023
// DONE: 1. Training room (Maneken in center of the room to practice)
// 2. Match with different player

// 04-10.01.2023
// DONE: 1. Not to render game when in main menu, the game is not yet started at all
// DONE: 2. Training dummy with infinite hp to test your dps or something
// DONE: 3. Make hp bar on top of me and training dummy
// DONE: 4. Collision for training dummy 
// 5. Make training dummy punch you if you are too close
// 6. Make texture for training dummy
// *7. Make texture for player
// **8. Make ability to throw dummy, change position on force
// DONE: 9. Disable input on player when not rendering
// DONE: 10. Remove "Play" and "Connect" button and replace with "Host" and "Client" and while waiting for player play on default map
// 11. Animations for player and dummy