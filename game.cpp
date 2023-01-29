void Game::AddVsPsInputLayout(D3D11_INPUT_ELEMENT_DESC *input_element_desc,
                               UINT count, LPCWSTR vs, LPCWSTR ps,
                               const char *key) {
  VertexShader vertex_shader = m_directx.CreateVertexShader(vs);
  PixelShader pixel_shader = m_directx.CreatePixelShader(ps);

  ID3D11InputLayout *input_layout = m_directx.CreateInputLayout(input_element_desc, count, &vertex_shader);

  shput(m_vertex_shaders, key, vertex_shader);
  shput(m_pixel_shaders, key, pixel_shader);
  shput(m_input_layouts, key, input_layout);
}

// And input layouts as they are bound to vertex shader
void Game::InitializeShaders() {
  // Map
  {
    D3D11_INPUT_ELEMENT_DESC input_element_desc[] = {
      {"position", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
       D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"texcoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
       D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"texcoord", 1, DXGI_FORMAT_R32G32_FLOAT, 1,
       D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
      {"texcoord", 2, DXGI_FORMAT_R32G32_FLOAT, 1,
       D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1},
    };

    AddVsPsInputLayout(input_element_desc,
                       _countof(input_element_desc), 
                       L"shaders\\map\\vs.hlsl",
                       L"shaders\\map\\ps.hlsl", "map");
  }

  // Player
  {
    D3D11_INPUT_ELEMENT_DESC input_element_desc[] = {
      {"position", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
       D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"texcoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
       D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    AddVsPsInputLayout(input_element_desc,
                       _countof(input_element_desc),
                       L"shaders\\player\\vs.hlsl",
                       L"shaders\\player\\ps.hlsl", "player");
  }

  // Weapon
  {
    PixelShader pixel_shader = m_directx.CreatePixelShader(L"shaders\\weapon\\ps.hlsl");
    shput(m_pixel_shaders, "weapon", pixel_shader);
  }

  // Bullets
  {
    VertexShader vertex_shader = m_directx.CreateVertexShader(L"shaders\\bullets\\vs.hlsl");
    PixelShader pixel_shader = m_directx.CreatePixelShader(L"shaders\\bullets\\ps.hlsl");

    shput(m_vertex_shaders, "bullets", vertex_shader);
    shput(m_pixel_shaders, "bullets", pixel_shader);
  }
}

void Game::CreateAndAddTexture(int type, const char *filename) {
  ID3D11ShaderResourceView *texture = m_directx.CreateTexture(filename);
  hmput(m_textures, type, texture);
}

void Game::InitializeTextures() {
  // Weapons
  {
    CreateAndAddTexture(Weapon_AK47, "assets\\weapons\\AK-47.png");
    CreateAndAddTexture(Weapon_Anaconda, "assets\\weapons\\Anaconda.png");
    CreateAndAddTexture(Weapon_AUG, "assets\\weapons\\AUG.png");
  }

  // Biker
  {
    CreateAndAddTexture(Animation_Run, "assets\\biker\\run.png");
    CreateAndAddTexture(Animation_Death, "assets\\biker\\death.png");
    CreateAndAddTexture(Animation_Idle, "assets\\biker\\idle.png");
    CreateAndAddTexture(Animation_Hurt, "assets\\biker\\hurt.png");
    CreateAndAddTexture(Animation_Dash, "assets\\biker\\dash.png");
    CreateAndAddTexture(Animation_Idle2, "assets\\biker\\idle2.png");
    CreateAndAddTexture(Animation_Sitdown, "assets\\biker\\sitdown.png");
  }
}

void Game::InitializeAnimations() {
  // Biker
  {
    HashMapPut<int, Animation>(&m_animations, Animation_Run, Animation {6});
    HashMapPut<int, Animation>(&m_animations, Animation_Death, Animation {6});
    HashMapPut<int, Animation>(&m_animations, Animation_Idle, Animation {4});
    HashMapPut<int, Animation>(&m_animations, Animation_Hurt, Animation {2});
    HashMapPut<int, Animation>(&m_animations, Animation_Dash, Animation {6});
    HashMapPut<int, Animation>(&m_animations, Animation_Idle2, Animation {6});
    HashMapPut<int, Animation>(&m_animations, Animation_Sitdown, Animation {4});
  }
}

void Game::InitializeWeaponInfo() {
  HashMapPut<int, int>(&m_weapon_info, Weapon_AK47, 15);
  HashMapPut<int, int>(&m_weapon_info, Weapon_Anaconda, 10);
  HashMapPut<int, int>(&m_weapon_info, Weapon_AUG, 3);
}

void Game::InitializeMeshes() {
  Mesh mesh;

  // Player
  {
    mesh.InitializeAsSquare(&m_directx);
    shput(m_meshes, "player", mesh);
  }

  // Weapon
  {
    mesh.InitializeAsSquare(&m_directx);
    shput(m_meshes, "weapon", mesh);
  }

  // Map
  {
    mesh.InitializeAsSquare(&m_directx);
    shput(m_meshes, "map", mesh);
  }

  // Bullet
  {
    mesh.InitializeAsSquare(&m_directx);
    shput(m_meshes, "bullets", mesh);
  }
}

void Game::Initialize(HINSTANCE instance) {
  m_window = CreateWindow("Walki", s_Width, s_Height);
  SetWindowData(m_window, &m_window_data);
  PresentWindow(m_window);

  InitializeImGuiWin32(m_window);

  m_directx.Initialize(m_window, s_Width, s_Height);
  ImGui_ImplDX11_Init(m_directx.m_device, m_directx.m_device_context);

  m_default_constant_buffer = m_directx.CreateBuffer(
      D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER,
      D3D11_CPU_ACCESS_WRITE, NULL, sizeof(ConstantBuffer));

  InitializeMeshes();
  InitializeShaders();
  InitializeTextures();
  InitializeAnimations();
  InitializeWeaponInfo();

  m_map.Initialize(&m_directx, s_MapWidth, s_MapHeight);
  m_camera.Initialize(s_PlayerSpawnPosition);
  m_player.Initialize(s_PlayerSpawnPosition, s_PlayerSize, s_PlayerStamina, s_PlayerHp, Animation_Idle);
  m_dummy.Initialize(s_DummyPosition, s_PlayerSize, s_DummyHp);
  
  XMFLOAT2 weapon_position = {m_player.m_position.x + m_player.m_size.x + m_player.m_size.x * 0.1f,
                              m_player.m_position.y};
  m_weapon.Initialize(Weapon_AK47, weapon_position, s_WeaponSize, s_WeaponDefaultAngle);
  m_bullets.Initialize(&m_directx, s_MaxBullets);

  // game->projection = XMMatrixPerspectiveFovLH(
  // XMConvertToRadians(45), s_Width / (float)s_Height, 0.1f, 1024);
  m_projection = XMMatrixOrthographicLH(static_cast<float>(s_Width), static_cast<float>(s_Height), 1, 1024);
  // game->projection = XMMatrixOrthographicOffCenterLH(-s_Width, s_Width, -s_Height, s_Height, 1, 1000);
  // game->projection = XMMatrixPerspectiveLH(
  //   s_Width, s_Height, 0.1f, 1024);
  m_state = State_Menu;
}

// TODO: Rethink
ConnectionData *Game::GetConnectionData() {
  auto connection = m_server.GetConnection();
  if (connection != NULL) {
    return &connection->host_data;
  } else {
    connection = m_client.m_connection;
    if (connection != NULL) {
      return &connection->client_data;
    }
  }

  return NULL;
}

void Game::UpdateCollision(float dt) {
  Body player_body;
  player_body.Initialize(m_player.m_position, m_player.m_size);

  // Player collision with map objects
  size_t map_object_count = arrlen(m_map.m_mesh_instance_data);
  for (int i = 0; i < map_object_count; i++) {
    auto oid = m_map.m_mesh_instance_data[i];

    Body object_body;
    object_body.Initialize(oid.position, oid.size);

    if (TestAABBAABB(&player_body, &object_body)) {
      XMFLOAT2 offset =
          GetIntersectionResolutionOffset(&player_body, &object_body);

      AddToVector(&m_player.m_position, offset);
      AddToVector(&m_weapon.m_position, offset);
      m_camera.Move(offset.x, offset.y);
    }
  }

  // Bullets
  Body dummy_body;
  dummy_body.Initialize(m_dummy.m_position, m_dummy.m_size);

  // Only process collisions on host machine
  auto connection = m_server.GetConnection();
  const auto connection_data = &connection->client_data;

  Body enemy_body;
  if (connection != NULL) {
    enemy_body.Initialize(connection_data->position, m_player.m_size);
  }

  for (int i = 0; i < arrlen(m_bullets.m_mesh_instance_data); i++) {
    auto bid = m_bullets.m_mesh_instance_data[i];

    Body bullet_body;
    bullet_body.Initialize(bid.position, bid.size);

    // With client / host player
    if (connection != NULL) {
      if (TestAABBAABB(&bullet_body, &enemy_body)) {
        int damage = hmget(m_weapon_info, m_weapon.m_type);

        // TODO: Send new hp to client
      }
    } else { // With dummy
      if (TestAABBAABB(&bullet_body, &dummy_body)) {
        int damage = hmget(m_weapon_info, m_weapon.m_type);
        if (m_dummy.m_hp - damage > 0) m_dummy.m_hp -= damage;
      }
    }

    // With map objects
    for (int j = 0; j < map_object_count; j++) {
      MeshInstanceData oid = m_map.m_mesh_instance_data[j];

      Body object_body;
      object_body.Initialize(oid.position, oid.size);

      if (TestAABBAABB(&bullet_body, &object_body)) {
        m_bullets.Remove(i);
        break;
      }
    }
  }
}

void Game::ChangePlayerAnimation(int animation_type) {
  if (m_player.m_animation_type != animation_type) {
    auto animation = &hmget(m_animations, m_player.m_animation_type);  
    animation->Reset();
  }

  m_player.m_animation_type = animation_type;
}

void Game::Input(float dt) {
  if (IsKeyDown(&m_window_data, VK_ESCAPE)) {
    m_state = State_Menu;
  }

  XMFLOAT2 player_velocity = {};
  if (IsKeyDown(&m_window_data, 'W')) {
    player_velocity.y = s_PlayerSpeed;
  }
  if (IsKeyDown(&m_window_data, 'S')) {
    player_velocity.y = -s_PlayerSpeed;
  }
  if (IsKeyDown(&m_window_data, 'A')) {
    player_velocity.x = -s_PlayerSpeed;
  }
  if (IsKeyDown(&m_window_data, 'D')) {
    player_velocity.x = s_PlayerSpeed;
  }

  /*
   * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
   * Used only as parameters to GetAsyncKeyState() and GetKeyState().
   * No other API or message will distinguish left and right keys in this way.
  */
  bool is_dash = false;
  if ((GetKeyState(VK_LSHIFT) & 0x8000) && m_player.m_stamina > 0) {
    player_velocity.x *= 2;
    player_velocity.y *= 2;
    m_player.m_stamina -= dt * 0.5f;
    is_dash = true;
  } else if (m_player.m_stamina < s_PlayerStamina) {
    m_player.m_stamina += dt * 0.1f;
  }

  bool is_ctrl_down = false;
  if (GetKeyState(VK_LCONTROL) & 0x8000) {
    is_ctrl_down = true;
  }

  if (!IsNull(player_velocity)) {
    m_player.m_direction = {RoundToClosest1(player_velocity.x), RoundToClosest1(player_velocity.y)};
    AddToVector(&m_player.m_position, player_velocity);
    AddToVector(&m_weapon.m_position, player_velocity);
    m_camera.Move(player_velocity.x, player_velocity.y);

    if (is_dash) {
      ChangePlayerAnimation(Animation_Dash);
    } else {
      ChangePlayerAnimation(Animation_Run);
    }
  } else {
    if (is_ctrl_down) {
      ChangePlayerAnimation(Animation_Sitdown);
    } else {
      ChangePlayerAnimation(Animation_Idle);
    }
  }

  POINT cursor_position = GetCursorPosition(m_window);

  // Solution to make rotation look more centered for surtain weapons?
  float additional_offset = 5; 

  XMFLOAT2 weapon_screen_center = {s_HalfWidth + m_player.m_size.x / 2.0f + m_weapon.m_size.x / 2.0f - additional_offset, (float)s_HalfHeight};
  XMVECTOR weapon_direction = XMVector2Normalize(XMVectorSet(weapon_screen_center.x - cursor_position.x, weapon_screen_center.y - cursor_position.y, 0, 0));
  m_weapon.Rotate(cursor_position.y, s_HalfHeight, weapon_direction);

  static float time = 0.0f;
  if (IsKeyDown(&m_window_data, MK_LBUTTON)) {
    if (time > s_ShootFrequency) {
      // Weapon default possition is toward -1 coordinate, so need to reverse x coordinate to shoot right
      float x = -XMVectorGetX(weapon_direction);

      m_bullets.Shoot(XMVectorSetX(weapon_direction, x), m_weapon.m_position, s_BulletSize);
      time = 0.0f;
    }

    time += dt;
  }
}

void Game::UpdateDummy(float dt) {
  if (m_dummy.m_hp < s_DummyHp) {
    m_dummy.m_hp += s_DummyRegenRate * dt;
  }
}

void Game::UpdateConnection() {
  auto connection = m_server.GetConnection();
  if (connection != NULL) {
    // Write host player data
    connection->host_data = {m_player.m_position, m_player.m_hp};
  } else {
    connection = m_client.m_connection;
    if (connection != NULL) {
      // Write client player data
      connection->client_data = {m_player.m_position, m_player.m_hp};
    }
  }
}

void Game::UpdateAnimations(float dt) {
  auto animation = &hmget(m_animations, m_player.m_animation_type);

  animation->Update(dt);
}

void Game::Update(float dt) {
  m_bullets.Update(s_BulletMaxDistance, s_BulletSpeed);
  UpdateCollision(dt);
  UpdateDummy(dt);
  UpdateConnection();
  UpdateAnimations(dt);
}

void Game::RenderGame() {
  auto device_context = m_directx.m_device_context;
  
  XMMATRIX view = m_camera.GetView();

  // Map
  {
    auto vertex_shader = &shget(m_vertex_shaders, "map");
    auto pixel_shader = &shget(m_pixel_shaders, "map");
    auto input_layout = shget(m_input_layouts, "map");
    auto mesh = &shget(m_meshes, "map");

    device_context->IASetInputLayout(input_layout);
    device_context->VSSetShader(vertex_shader->shader,
                                         nullptr, 0);
    device_context->PSSetShader(pixel_shader->shader, nullptr,
                                         0);

    XMMATRIX mvp = view * m_projection;

    ConstantBuffer constant_buffer_data = { XMMatrixTranspose(mvp) };
    m_directx.UpdateBuffer(m_default_constant_buffer, &constant_buffer_data, sizeof(ConstantBuffer));

    device_context->VSSetConstantBuffers(0, 1, &m_default_constant_buffer);

    ID3D11Buffer *buffers[] = {mesh->m_vertex_buffer,
                               m_map.m_instance_buffer};
    UINT strides[] = {sizeof(Vertex), sizeof(MeshInstanceData)};
    UINT offsets[] = {0, 0};
    device_context->IASetVertexBuffers(0, 2, buffers, strides,
                                                offsets);
    device_context->IASetIndexBuffer(mesh->m_index_buffer,
                                              DXGI_FORMAT_R32_UINT, 0);

    UINT count = static_cast<UINT>(arrlen(m_map.m_mesh_instance_data));
    device_context->DrawIndexedInstanced(
        6, count, 0, 0, 0);
  }

  // Bullets
  size_t bullet_count = arrlen(m_bullets.m_mesh_instance_data);
  if (bullet_count > 0)
  {
    auto vertex_shader = &shget(m_vertex_shaders, "bullets");
    auto pixel_shader = &shget(m_pixel_shaders, "bullets");
    auto mesh = &shget(m_meshes, "bullets");

    device_context->VSSetShader(vertex_shader->shader,
                                         nullptr, 0);
    device_context->PSSetShader(pixel_shader->shader, nullptr,
                                         0);
    // Apply changes to position
    m_directx.UpdateBuffer(m_bullets.m_instance_buffer, m_bullets.m_mesh_instance_data, sizeof(MeshInstanceData) * bullet_count);

    ID3D11Buffer *buffers[] = {mesh->m_vertex_buffer,
                               m_bullets.m_instance_buffer};
    UINT strides[] = {sizeof(Vertex), sizeof(MeshInstanceData)};
    UINT offsets[] = {0, 0};
    device_context->IASetVertexBuffers(0, 2, buffers, strides,
                                                offsets);

    device_context->DrawIndexedInstanced(
        6, static_cast<UINT>(bullet_count), 0, 0, 0);
  }

  // Player
  {
    auto vertex_shader = &shget(m_vertex_shaders, "player");
    auto pixel_shader = &shget(m_pixel_shaders, "player");
    auto input_layout = shget(m_input_layouts, "player");
    auto mesh = &shget(m_meshes, "player");
    auto texture = hmget(m_textures, m_player.m_animation_type);
    auto animation = &hmget(m_animations, m_player.m_animation_type);

    // Update UVs animation
    Vertex vertices[4] = {
      Vertex{XMFLOAT2{-1, -1}, animation->m_uv[0]},
      Vertex{XMFLOAT2{-1, 1}, animation->m_uv[1]},
      Vertex{XMFLOAT2{1, 1}, animation->m_uv[2]},
      Vertex{XMFLOAT2{1, -1}, animation->m_uv[3]}
    };

    // Flip texture if needed
    if (m_player.m_direction.x < 1) {
      vertices[0].uv.x = 1 - vertices[0].uv.x;
      vertices[1].uv.x = 1 - vertices[1].uv.x;
      vertices[2].uv.x = 1 - vertices[2].uv.x;
      vertices[3].uv.x = 1 - vertices[3].uv.x;
    }

    m_directx.UpdateBuffer(mesh->m_vertex_buffer, &vertices, sizeof(vertices));

    device_context->IASetInputLayout(input_layout);
    device_context->VSSetShader(vertex_shader->shader,
                                         nullptr, 0);
    device_context->PSSetShader(pixel_shader->shader,
                                         nullptr, 0);
    device_context->PSSetShaderResources(0, 1, &texture);

    XMMATRIX mvp = GetModelMatrix(m_player.m_position, m_player.m_size) * view * m_projection;

    ConstantBuffer constant_buffer_data = { XMMatrixTranspose(mvp) };
    m_directx.UpdateBuffer(m_default_constant_buffer, &constant_buffer_data, sizeof(ConstantBuffer));

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    device_context->IASetVertexBuffers(
        0, 1, &mesh->m_vertex_buffer, &stride, &offset);
    device_context->IASetIndexBuffer(mesh->m_index_buffer,
                                              DXGI_FORMAT_R32_UINT, 0);

    device_context->DrawIndexed(6, 0, 0);

    // Client / Host depending on who see it
    auto connection = m_server.GetConnection();
    XMFLOAT2 position = {};
    if (connection != NULL) {
      position = connection->client_data.position;
    } else {
      connection = m_client.m_connection;
      if (connection != NULL) {
        position = connection->host_data.position;
      }
    }

    if (connection != NULL) {
      mvp = GetModelMatrix(position, m_player.m_size) * view * m_projection;

      constant_buffer_data = { XMMatrixTranspose(mvp) };
      m_directx.UpdateBuffer(m_default_constant_buffer, &constant_buffer_data, sizeof(ConstantBuffer));

      device_context->DrawIndexed(6, 0, 0);
    } else { // For now we assume that if we don't have connection, than we are in a training mode
      mvp = GetModelMatrix(m_dummy.m_position, m_player.m_size) * view * m_projection;

      constant_buffer_data = { XMMatrixTranspose(mvp) };
      m_directx.UpdateBuffer(m_default_constant_buffer, &constant_buffer_data, sizeof(ConstantBuffer));

      device_context->DrawIndexed(6, 0, 0);
    }

    // Weapon
    {
      pixel_shader = &shget(m_pixel_shaders, "weapon");
      mesh = &shget(m_meshes, "weapon");
      texture = hmget(m_textures, m_weapon.m_type);

      device_context->PSSetShader(pixel_shader->shader,
                                           nullptr, 0);
      device_context->PSSetSamplers(0, 1, &m_directx.m_sampler);
      device_context->PSSetShaderResources(0, 1, &texture);

      mvp = GetModelMatrix(m_weapon.m_position, m_weapon.m_size, m_weapon.m_angle) * view * m_projection;

      constant_buffer_data = { XMMatrixTranspose(mvp) };
      m_directx.UpdateBuffer(m_default_constant_buffer, &constant_buffer_data, sizeof(ConstantBuffer));

      device_context->IASetVertexBuffers(
        0, 1, &mesh->m_vertex_buffer, &stride, &offset);

      device_context->DrawIndexed(6, 0, 0);
    }
  }
}

void Game::RenderMenuInterface() {
  ImGuiIO &io = ImGui::GetIO();

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 30);
  ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.0f));
  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.0f, 0.0f, 1.0f));
  ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);

  const ImVec2 button_size = {200, 100};

  ImGui::SetCursorPos({io.DisplaySize.x / 2.0f - button_size.x * 0.5f, io.DisplaySize.y / 2.0f - button_size.y * 3});
  if (ImGui::Button("Play", button_size)) {
    m_state = State_Game;
  }
  
  ImGui::SetCursorPosX(io.DisplaySize.x / 2.0f - button_size.x * 0.5f);
  if (ImGui::Button("Host", button_size)) {
    m_state = State_Game;

    m_server.Start();
  }

  ImGui::SetCursorPosX(io.DisplaySize.x / 2.0f - button_size.x * 0.5f);
  if (ImGui::Button("Client", button_size)) {
    m_state = State_Game;

    m_client.Start("127.0.0.1", "1234");
  }
  ImGui::SetCursorPosX(io.DisplaySize.x / 2.0f - button_size.x * 0.5f);
  ImGui::Button("About", button_size);
  ImGui::SetCursorPosX(io.DisplaySize.x / 2.0f - button_size.x * 0.5f);
  if (ImGui::Button("Exit", button_size)) {
    PostQuitMessage(0);
  }

  ImGui::PopTextWrapPos();
  ImGui::PopStyleColor(2);
  ImGui::PopStyleVar();
}

bool Game::IsConnected() {
  auto connection = m_server.GetConnection();
  if (connection != NULL) {
    return true;
  } else {
    if (m_client.m_connection != NULL) {
      return true;
    }
  }

  return false;
}

// TODO: Move to utils?
ImVec2 Game::GetARelativeToB(XMFLOAT2 a, XMFLOAT2 b) {
  return {a.x - b.x, 
          b.y - a.y};
}

void Game::RenderGameInterface() {
  ImGuiIO &io = ImGui::GetIO();

  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0, 1.0f, 0, 1));
  ImGui::ProgressBar(m_player.m_stamina, ImVec2(100.0f, 0.0f));
  ImGui::PopStyleColor();

  ImGui::Text("Player hp -> %.3f", m_player.m_hp);
  ImGui::Text("Weapon angle -> %.3f", m_weapon.m_angle);
  ImGui::Text("Bullets count -> %d", arrlen(m_bullets.m_mesh_instance_data));
  ImGui::Text("Dummy hp -> %.3f", m_dummy.m_hp);

  auto animation = &hmget(m_animations, m_player.m_animation_type);
  ImGui::Text("Animation index, time -> %d, %.3f", animation->m_index, animation->m_time);

  // If not multiplayer, render hp for dummy
  ImVec2 offset = {m_player.m_size.x * 2, m_player.m_size.y * 2};
  if (!IsConnected()) {
    ImVec2 dummy_position = GetARelativeToB(m_dummy.m_position, m_player.m_position);

    ImGui::SetCursorPos(io.DisplaySize / 2.0f + dummy_position - offset);
    DrawHpBar(m_dummy.m_hp, s_DummyHp);
  } else {
    // Render hp for Host / Client
    auto connection = m_server.GetConnection();
    ConnectionData *connection_data = NULL;
    if (connection != NULL) {
      connection_data = &connection->client_data;
    } else {
      connection = m_client.m_connection;
      if (connection != NULL) {
        connection_data = &connection->host_data;
      }
    }

    // Not actually needed, but just in case
    assert(connection_data != NULL);

    ImVec2 relative_position = GetARelativeToB(connection_data->position, m_player.m_position);
    ImGui::SetCursorPos(io.DisplaySize / 2.0f + relative_position - offset);
    DrawHpBar(connection_data->hp, s_PlayerHp);
  }

  ImGui::SetCursorPos(io.DisplaySize / 2.0f - offset);
  DrawHpBar(m_player.m_hp, s_PlayerHp, ImVec4(1.0f, 0, 0, 1));
}

void Game::RenderBegin() {
  auto device_context = m_directx.m_device_context;

  const float clear_color[] = {0.1, 0.1, 0.2, 1};
  device_context->ClearRenderTargetView(m_directx.m_render_target_view,
                                                 clear_color);
  device_context->ClearDepthStencilView(
      m_directx.m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
      1.0f, 0);
  device_context->RSSetViewports(1, &m_directx.m_viewport);
  device_context->OMSetRenderTargets(1, &m_directx.m_render_target_view,
                                              m_directx.m_depth_stencil_view);
  device_context->IASetPrimitiveTopology(
      D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Used to rendering opaque textures
  device_context->OMSetDepthStencilState(m_directx.m_depth_stencil_state,
                                                  0);
  const float blend_factor[] = {0, 0, 0, 0};
      device_context->OMSetBlendState(m_directx.m_blend_state,
      blend_factor, 0xffffffff);
}

void Game::Render() {
  RenderBegin();
  ImGuiBegin();

  ImGuiIO &io = ImGui::GetIO();

  ImGui::SetNextWindowPos(ImVec2(0, 0), 0, ImVec2(0, 0));
  ImGui::SetNextWindowSize(io.DisplaySize);
  ImGui::SetNextWindowBgAlpha(0);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::Begin("Interface", NULL,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                   ImGuiWindowFlags_NoScrollbar);
  ImGui::Text("FPS: %.3f", io.Framerate);

  switch (m_state) {
    case State_Menu:
      RenderMenuInterface();
      break;
    case State_Game:
      RenderGame();
      RenderGameInterface();
      break;
    default:
      assert(0);
      break;
  }
  
  ImGui::PopStyleVar(2);
  ImGui::End();

  ImGuiEnd();
  m_directx.m_swap_chain->Present(0, 0);
}

void Game::Run() {
  int old_time = GetMilliseconds();
  int extra_time = 0;
  int frame_time = 1000 / s_Fps;
  float dt = frame_time / 1000.0f;

  MSG msg = {};
  while (msg.message != WM_QUIT) {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    int new_time = GetMilliseconds();
    int time = new_time - old_time;

    /////// FRAME ///////

    if (IsWindowActive(&m_window_data)) {
      extra_time += time;

      while (extra_time >= frame_time) {
        if (m_state == State_Game) Input(dt);
        Update(dt);

        extra_time -= frame_time;
      }

      Render();
    } else {
      Sleep(5);
    }

    /////// END FRAME ///////

    old_time = new_time;
  }

  // Just in case of rage quit, close this shit
  m_server.Stop();
  m_client.Stop();
}