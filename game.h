struct Game {
  HWND m_window;
  WindowData m_window_data;
  Directx m_directx;
  Map m_map;
  Camera m_camera;
  Player m_player;
  Weapon m_weapon;
  Dummy m_dummy;
  Bullets m_bullets;
  Server m_server;
  Client m_client;
  
  XMMATRIX m_projection;

  ID3D11Buffer *m_default_constant_buffer;
  HashMap<char *, ID3D11InputLayout *> *m_input_layouts = NULL;
  HashMap<char *, VertexShader> *m_vertex_shaders = NULL;
  HashMap<char *, PixelShader> *m_pixel_shaders = NULL;
  HashMap<int, ID3D11ShaderResourceView *> *m_textures = NULL;
  HashMap<char *, Mesh> *m_meshes = NULL;

  HashMap<int, int> *m_weapon_info = NULL;
  HashMap<int, Animation> *m_animations = NULL;

  int m_state = State_None;

  void Initialize(HINSTANCE instance);
  void Run();

private:
  void AddVsPsInputLayout(D3D11_INPUT_ELEMENT_DESC *input_element_desc,
                                 UINT count, LPCWSTR vs, LPCWSTR ps,
                                 const char *key);
  void InitializeShaders();
  void CreateAndAddTexture(int type, const char *filename);
  void InitializeTextures();
  void InitializeAnimations();
  void InitializeWeaponInfo();
  void InitializeMeshes();
  ConnectionData *GetConnectionData();
  void UpdateCollision(float dt);
  void Input(float dt);
  void UpdateDummy(float dt);
  void UpdateConnection();
  void UpdateAnimations(float dt);
  void Update(float dt);
  void RenderGame();
  void RenderMenuInterface();
  bool IsConnected();
  ImVec2 GetARelativeToB(XMFLOAT2 a, XMFLOAT2 b);
  void RenderGameInterface();
  void RenderBegin();
  void Render();
  void ChangePlayerAnimation(int animation_type);
};