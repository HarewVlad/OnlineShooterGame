struct Game {
  HWND window;
  WindowData window_data;
  Directx directx;
  Mesh mesh;
  Map map;
  Camera camera;
  Player player;
  Weapon weapon;
  Dummy dummy;
  Bullets bullets;
  Server server;
  Client client;
  
  XMMATRIX projection;

  ID3D11Buffer *default_constant_buffer;
  HashMap<char *, ID3D11InputLayout *> *input_layouts = NULL;
  HashMap<char *, VertexShader> *vertex_shaders = NULL;
  HashMap<char *, PixelShader> *pixel_shaders = NULL;
  HashMap<int, ID3D11ShaderResourceView *> *textures = NULL;

  HashMap<int, int> *weapon_info = NULL;
};

static void InitializeGame(Game *game, HINSTANCE instance);
static void Run(Game *game);