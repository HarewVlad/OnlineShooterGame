static void AddVsPsInputLayout(Game *game, ID3D11Device *device,
                               D3D11_INPUT_ELEMENT_DESC *input_element_desc,
                               UINT count, LPCWSTR vs, LPCWSTR ps,
                               const char *key) {
  VertexShader vertex_shader;
  CreateVertexShader(&vertex_shader, game->directx.device, vs);

  PixelShader pixel_shader;
  CreatePixelShader(&pixel_shader, game->directx.device, ps);

  ID3D11InputLayout *input_layout = CreateInputLayout(
      game->directx.device, input_element_desc, count, &vertex_shader);

  shput(game->vertex_shaders, key, vertex_shader);
  shput(game->pixel_shaders, key, pixel_shader);
  shput(game->input_layouts, key, input_layout);
}

// And input layouts as they are bound to vertex shader
static void InitializeShaders(Game *game) {
  auto device = game->directx.device;

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

    AddVsPsInputLayout(game, device, input_element_desc,
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

    AddVsPsInputLayout(game, device, input_element_desc,
                       _countof(input_element_desc),
                       L"shaders\\player\\vs.hlsl",
                       L"shaders\\player\\ps.hlsl", "player");
  }

  // Weapon
  {
    PixelShader pixel_shader;
    CreatePixelShader(&pixel_shader, device, L"shaders\\weapon\\ps.hlsl");
    shput(game->pixel_shaders, "weapon", pixel_shader);
  }

  // Bullets
  {
    VertexShader vertex_shader;
    CreateVertexShader(&vertex_shader, device, L"shaders\\bullets\\vs.hlsl");

    PixelShader pixel_shader;
    CreatePixelShader(&pixel_shader, device, L"shaders\\bullets\\ps.hlsl");

    shput(game->vertex_shaders, "bullets", vertex_shader);
    shput(game->pixel_shaders, "bullets", pixel_shader);
  }
}

static void CreateAndAddTexture(Game *game, int type, const char *filename) {
  ID3D11ShaderResourceView *texture = CreateTexture(game->directx.device, filename);
  hmput(game->textures, type, texture);
}

static void InitializeTextures(Game *game) {
  // Weapons
  {
    CreateAndAddTexture(game, Weapon_AK47, "assets\\weapons\\AK-47.png");
    CreateAndAddTexture(game, Weapon_Anaconda, "assets\\weapons\\Anaconda.png");
    CreateAndAddTexture(game, Weapon_AUG, "assets\\weapons\\AUG.png");
  }

  // Biker
  {
    CreateAndAddTexture(game, Animation_Run, "assets\\biker\\run.png");
    CreateAndAddTexture(game, Animation_Death, "assets\\biker\\death.png");
    CreateAndAddTexture(game, Animation_Idle, "assets\\biker\\idle.png");
    CreateAndAddTexture(game, Animation_Hurt, "assets\\biker\\hurt.png");
  }
}

static void InitializeAnimations(Game *game) {
  // Biker
  {
    HashMapPut<int, Animation>(&game->animations, Animation_Run, Animation {0, 6, 0});
    HashMapPut<int, Animation>(&game->animations, Animation_Death, Animation {0, 6, 0});
    HashMapPut<int, Animation>(&game->animations, Animation_Idle, Animation {0, 4, 0});
    HashMapPut<int, Animation>(&game->animations, Animation_Hurt, Animation {0, 2, 0});
  }
}

static void InitializeWeaponInfo(Game *game) {
  HashMapPut<int, int>(&game->weapon_info, Weapon_AK47, 15);
  HashMapPut<int, int>(&game->weapon_info, Weapon_Anaconda, 10);
  HashMapPut<int, int>(&game->weapon_info, Weapon_AUG, 3);
}

static void InitializeMeshes(Game *game) {
  auto device = game->directx.device;

  Mesh mesh;

  // Player
  {
    InitializeSquare(&mesh, device);
    shput(game->meshes, "player", mesh);
  }

  // Weapon
  {
    InitializeSquare(&mesh, device);
    shput(game->meshes, "weapon", mesh);
  }

  // Map
  {
    InitializeSquare(&mesh, device);
    shput(game->meshes, "map", mesh);
  }

  // Bullet
  {
    InitializeSquare(&mesh, device);
    shput(game->meshes, "bullets", mesh);
  }
}

static void InitializeGame(Game *game, HINSTANCE instance) {
  game->window = CreateWindow("Walki", s_Width, s_Height);
  SetWindowData(game->window, &game->window_data);
  PresentWindow(game->window);

  InitializeImGuiWin32(game->window);

  InitializeDirectx(&game->directx, game->window, s_Width, s_Height);
  ImGui_ImplDX11_Init(game->directx.device, game->directx.device_context);

  game->default_constant_buffer = CreateBuffer(
      game->directx.device, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER,
      D3D11_CPU_ACCESS_WRITE, NULL, sizeof(ConstantBuffer));

  InitializeMeshes(game);
  InitializeShaders(game);
  InitializeTextures(game);
  InitializeAnimations(game);
  InitializeDefaultMap(&game->map, game->directx.device, s_MapWidth, s_MapHeight);
  InitializeCamera(&game->camera, s_PlayerSpawnPosition);
  InitializePlayer(&game->player, s_PlayerSpawnPosition, s_PlayerSize, s_PlayerStamina, s_PlayerHp, Animation_Idle);
  InitializeDummy(&game->dummy, s_DummyPosition, s_PlayerSize, s_DummyHp);

  InitializeWeaponInfo(game);
  XMFLOAT2 weapon_position = {game->player.position.x + game->player.size.x + game->player.size.x * 0.1f,
                              game->player.position.y};
  InitializeWeapon(&game->weapon, Weapon_AK47,
                  weapon_position, s_WeaponSize, s_WeaponDefaultAngle);
  InitializeBullets(&game->bullets, game->directx.device, s_MaxBullets);

  // game->projection = XMMatrixPerspectiveFovLH(
  // XMConvertToRadians(45), s_Width / (float)s_Height, 0.1f, 1024);
  game->projection = XMMatrixOrthographicLH(static_cast<float>(s_Width), static_cast<float>(s_Height), 1, 1024);
  // game->projection = XMMatrixOrthographicOffCenterLH(-s_Width, s_Width, -s_Height, s_Height, 1, 1000);
  // game->projection = XMMatrixPerspectiveLH(
  //   s_Width, s_Height, 0.1f, 1024);
}

ConnectionData *GetConnectionData(Game *game) {
  auto connection = GetConnection(&game->server);
  if (connection != NULL) {
    return &connection->host_data;
  } else {
    connection = game->client.connection;
    if (connection != NULL) {
      return &connection->client_data;
    }
  }

  return NULL;
}

static void UpdateCollision(Game *game, float dt) {
  auto player = &game->player;
  auto weapon = &game->weapon;
  auto map = &game->map;

  Body player_body;
  InitializeBody(&player_body, player->position, player->size);

  // Player collision with map objects
  size_t map_object_count = arrlen(map->mesh_instance_data);
  for (int i = 0; i < map_object_count; i++) {
    auto oid = map->mesh_instance_data[i];

    Body object_body;
    InitializeBody(&object_body, oid.position, oid.size);

    if (TestAABBAABB(&player_body, &object_body)) {
      XMFLOAT2 offset =
          GetIntersectionResolutionOffset(&player_body, &object_body);

      AddToVector(&player->position, offset);
      AddToVector(&weapon->position, offset);
      MoveCamera(&game->camera, offset.x, offset.y);
    }
  }

  // Bullets
  auto bullets = &game->bullets;
  auto dummy = &game->dummy;

  Body dummy_body;
  InitializeBody(&dummy_body, dummy->position, dummy->size);

  // Only process collisions on host machine
  auto connection = GetConnection(&game->server);
  const auto connection_data = &connection->client_data;

  Body enemy_body;
  if (connection != NULL) {
    InitializeBody(&enemy_body, connection_data->position, player->size);
  }

  for (int i = 0; i < arrlen(bullets->mesh_instance_data); i++) {
    auto bid = bullets->mesh_instance_data[i];

    Body bullet_body;
    InitializeBody(&bullet_body, bid.position, bid.size);

    // With client / host player
    if (connection != NULL) {
      if (TestAABBAABB(&bullet_body, &enemy_body)) {
        int damage = hmget(game->weapon_info, weapon->type);

        // So initial thought is that server makes changed, send them to client, and client apply them and after that cleans it
        // changed_data->hp = static_cast<float>(-damage);
      }
    } else { // With dummy
      if (TestAABBAABB(&bullet_body, &dummy_body)) {
        int damage = hmget(game->weapon_info, weapon->type);
        dummy->hp -= damage;
      }
    }

    // With map objects
    for (int j = 0; j < map_object_count; j++) {
      MeshInstanceData oid = map->mesh_instance_data[j];

      Body object_body;
      InitializeBody(&object_body, oid.position, oid.size);

      if (TestAABBAABB(&bullet_body, &object_body)) {
        RemoveBullet(bullets, i);
        break;
      }
    }
  }
}

static void Input(Game *game, float dt) {
  auto player = &game->player;
  auto weapon = &game->weapon;

  if (IsKeyDown(&game->window_data, VK_ESCAPE)) {
    s_State = State_Menu;
  }

  XMFLOAT2 player_velocity = {};
  if (IsKeyDown(&game->window_data, 'W')) {
    player_velocity.y = s_PlayerSpeed;
  }
  if (IsKeyDown(&game->window_data, 'S')) {
    player_velocity.y = -s_PlayerSpeed;
  }
  if (IsKeyDown(&game->window_data, 'A')) {
    player_velocity.x = -s_PlayerSpeed;
  }
  if (IsKeyDown(&game->window_data, 'D')) {
    player_velocity.x = s_PlayerSpeed;
  }

  /*
   * VK_L* & VK_R* - left and right Alt, Ctrl and Shift virtual keys.
   * Used only as parameters to GetAsyncKeyState() and GetKeyState().
   * No other API or message will distinguish left and right keys in this way.
  */
  if ((GetKeyState(VK_LSHIFT) & 0x8000) && player->stamina > 0) {
    player_velocity.x *= 2;
    player_velocity.y *= 2;
    player->stamina -= dt * 0.5f;
  } else if (player->stamina < s_PlayerStamina) {
    player->stamina += dt * 0.1f;
  }

  if (player_velocity.x != 0 || player_velocity.y != 0) {
    AddToVector(&player->position, player_velocity);
    AddToVector(&weapon->position, player_velocity);
    MoveCamera(&game->camera, player_velocity.x, player_velocity.y);
    player->animation_type = Animation_Run;
  } else {
    player->animation_type = Animation_Idle;
  }

  // Vector from weapon cetner to cursor
  POINT cursor_position = GetCursorPosition(game->window);

  // Solution to make rotation look more centered for surtain weapons?
  float additional_offset = 5; 

  XMFLOAT2 weapon_screen_center = {s_HalfWidth + player->size.x / 2.0f + weapon->size.x / 2.0f - additional_offset, (float)s_HalfHeight};
  XMVECTOR weapon_direction = XMVector2Normalize(XMVectorSet(weapon_screen_center.x - cursor_position.x, weapon_screen_center.y - cursor_position.y, 0, 0));

  // Rotate weapon toward some direction
  WeaponRotate(weapon, cursor_position.y, s_HalfHeight, weapon_direction);

  static float time = 0.0f;
  if (IsKeyDown(&game->window_data, MK_LBUTTON)) {
    if (time > s_ShootFrequency) {
      // Weapon default possition is toward -1 coordinate, so need to reverse x coordinate to shoot right
      float x = -XMVectorGetX(weapon_direction);

      ShootBullet(&game->bullets, XMVectorSetX(weapon_direction, x), weapon->position);
      time = 0.0f;
    }

    time += dt;
  }
}

static void UpdateDummy(Dummy *dummy, float dt) {
  // Regenerate dummy
  if (dummy->hp < s_DummyHp) {
    dummy->hp += s_DummyRegenRate * dt;
  }
}

static void UpdateConnection(Game *game) {
  auto player = &game->player;
  auto connection = GetConnection(&game->server);

  if (connection != NULL) {
    // Write host player data
    connection->host_data = {player->position, player->hp};
  } else {
    connection = game->client.connection;
    if (connection != NULL) {
      // Write client player data
      connection->client_data = {player->position, player->hp};
    }
  }
}

static void UpdateAnimations(Game *game, float dt) {
  auto player = &game->player;
  auto animation = &hmget(game->animations, player->animation_type);

  UpdateAnimation(animation, dt);
}

static void Update(Game *game, float dt) {
  UpdateBullets(&game->bullets, s_BulletMaxDistance, s_BulletSpeed);
  UpdateCollision(game, dt);
  UpdateDummy(&game->dummy, dt);
  UpdateConnection(game);
  UpdateAnimations(game, dt);
}

static void RenderGame(Game *game) {
  auto directx = &game->directx;
  auto device_context = directx->device_context;
  
  XMMATRIX view = GetCameraView(&game->camera);

  // Map
  {
    auto map = &game->map;

    auto vertex_shader = &shget(game->vertex_shaders, "map");
    auto pixel_shader = &shget(game->pixel_shaders, "map");
    auto input_layout = shget(game->input_layouts, "map");
    auto mesh = &shget(game->meshes, "map");

    device_context->IASetInputLayout(input_layout);
    device_context->VSSetShader(vertex_shader->shader,
                                         nullptr, 0);
    device_context->PSSetShader(pixel_shader->shader, nullptr,
                                         0);

    XMMATRIX mvp = view * game->projection;

    ConstantBuffer constant_buffer_data = { XMMatrixTranspose(mvp) };
    UpdateBuffer(game->default_constant_buffer, &constant_buffer_data, sizeof(ConstantBuffer),
                 device_context);

    device_context->VSSetConstantBuffers(0, 1, &game->default_constant_buffer);

    ID3D11Buffer *buffers[] = {mesh->vertex_buffer,
                               map->instance_buffer};
    UINT strides[] = {sizeof(Vertex), sizeof(MeshInstanceData)};
    UINT offsets[] = {0, 0};
    device_context->IASetVertexBuffers(0, 2, buffers, strides,
                                                offsets);
    device_context->IASetIndexBuffer(mesh->index_buffer,
                                              DXGI_FORMAT_R32_UINT, 0);

    UINT count = static_cast<UINT>(arrlen(map->mesh_instance_data));
    device_context->DrawIndexedInstanced(
        6, count, 0, 0, 0);
  }

  // Bullets
  auto bullets = &game->bullets;
  size_t bullet_count = arrlen(bullets->mesh_instance_data);
  if (bullet_count > 0)
  {
    auto vertex_shader = &shget(game->vertex_shaders, "bullets");
    auto pixel_shader = &shget(game->pixel_shaders, "bullets");
    auto mesh = &shget(game->meshes, "bullets");

    device_context->VSSetShader(vertex_shader->shader,
                                         nullptr, 0);
    device_context->PSSetShader(pixel_shader->shader, nullptr,
                                         0);
    // Apply changes to position
    UpdateBuffer(bullets->instance_buffer, bullets->mesh_instance_data, sizeof(MeshInstanceData) * bullet_count,
                 device_context);

    ID3D11Buffer *buffers[] = {mesh->vertex_buffer,
                               bullets->instance_buffer};
    UINT strides[] = {sizeof(Vertex), sizeof(MeshInstanceData)};
    UINT offsets[] = {0, 0};
    device_context->IASetVertexBuffers(0, 2, buffers, strides,
                                                offsets);

    device_context->DrawIndexedInstanced(
        6, static_cast<UINT>(bullet_count), 0, 0, 0);
  }

  // Player
  {
    auto player = &game->player;

    auto vertex_shader = &shget(game->vertex_shaders, "player");
    auto pixel_shader = &shget(game->pixel_shaders, "player");
    auto input_layout = shget(game->input_layouts, "player");
    auto mesh = &shget(game->meshes, "player");
    auto texture = hmget(game->textures, player->animation_type);
    auto animation = &hmget(game->animations, player->animation_type);

    // Update UVs for player
    Vertex vertices[4] = {
      Vertex{XMFLOAT2{-1, -1}, animation->uv[0]},
      Vertex{XMFLOAT2{-1, 1}, animation->uv[1]},
      Vertex{XMFLOAT2{1, 1}, animation->uv[2]},
      Vertex{XMFLOAT2{1, -1}, animation->uv[3]}
    };

    UpdateBuffer(mesh->vertex_buffer, &vertices, sizeof(vertices),
                 device_context);

    device_context->IASetInputLayout(input_layout);
    device_context->VSSetShader(vertex_shader->shader,
                                         nullptr, 0);
    device_context->PSSetShader(pixel_shader->shader,
                                         nullptr, 0);
    device_context->PSSetShaderResources(0, 1, &texture);

    XMMATRIX mvp = GetModelMatrix(player->position, player->size) * view * game->projection;

    ConstantBuffer constant_buffer_data = { XMMatrixTranspose(mvp) };
    UpdateBuffer(game->default_constant_buffer, &constant_buffer_data, sizeof(ConstantBuffer),
                 device_context);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    device_context->IASetVertexBuffers(
        0, 1, &mesh->vertex_buffer, &stride, &offset);
    device_context->IASetIndexBuffer(mesh->index_buffer,
                                              DXGI_FORMAT_R32_UINT, 0);

    device_context->DrawIndexed(6, 0, 0);

    // Client / Host depending on who see it
    auto connection = GetConnection(&game->server);
    XMFLOAT2 position = {};
    if (connection != NULL) {
      position = connection->client_data.position;
    } else {
      connection = game->client.connection;
      if (connection != NULL) {
        position = connection->host_data.position;
      }
    }

    if (connection != NULL) {
      mvp = GetModelMatrix(position, player->size) * view * game->projection;

      constant_buffer_data = { XMMatrixTranspose(mvp) };
      UpdateBuffer(game->default_constant_buffer, &constant_buffer_data, sizeof(ConstantBuffer),
                   device_context);

      device_context->DrawIndexed(6, 0, 0);
    } else { // For now we assume that if we don't have connection, than we are in a training mode
      auto dummy = &game->dummy;

      mvp = GetModelMatrix(dummy->position, player->size) * view * game->projection;

      constant_buffer_data = { XMMatrixTranspose(mvp) };
      UpdateBuffer(game->default_constant_buffer, &constant_buffer_data, sizeof(ConstantBuffer),
                   device_context);

      device_context->DrawIndexed(6, 0, 0);
    }

    // Weapon
    {
      auto weapon = &game->weapon;

      pixel_shader = &shget(game->pixel_shaders, "weapon");
      mesh = &shget(game->meshes, "weapon");
      texture = hmget(game->textures, weapon->type);

      device_context->PSSetShader(pixel_shader->shader,
                                           nullptr, 0);
      device_context->PSSetSamplers(0, 1, &directx->sampler);
      device_context->PSSetShaderResources(0, 1, &texture);

      mvp = GetModelMatrix(weapon->position, weapon->size, weapon->angle) * view * game->projection;

      constant_buffer_data = { XMMatrixTranspose(mvp) };
      UpdateBuffer(game->default_constant_buffer, &constant_buffer_data, sizeof(ConstantBuffer),
                   device_context);

      device_context->IASetVertexBuffers(
        0, 1, &mesh->vertex_buffer, &stride, &offset);

      device_context->DrawIndexed(6, 0, 0);
    }
  }
}

static void RenderMenuInterface(Game *game) {
  ImGuiIO &io = ImGui::GetIO();

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 30);
  ImGui::PushStyleColor(ImGuiCol_Border, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.0f));
  ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(0.0f, 0.0f, 1.0f));
  ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);

  const ImVec2 button_size = {200, 100};

  ImGui::SetCursorPos({io.DisplaySize.x / 2.0f - button_size.x * 0.5f, io.DisplaySize.y / 2.0f - button_size.y * 3});
  if (ImGui::Button("Play", button_size)) {
    s_State = State_Game;
  }
  
  ImGui::SetCursorPosX(io.DisplaySize.x / 2.0f - button_size.x * 0.5f);
  if (ImGui::Button("Host", button_size)) {
    s_State = State_Game;

    // Starting server and while playing on default map, waiting for connection
    StartServer(&game->server);
  }

  ImGui::SetCursorPosX(io.DisplaySize.x / 2.0f - button_size.x * 0.5f);
  if (ImGui::Button("Client", button_size)) {
    s_State = State_Game;

    // Connecting to host while playing on default map
    StartClient(&game->client, "127.0.0.1", "1234");
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

static bool IsConnected(Server *server, Client *client) {
  Connection *connection = GetConnection(server);
  if (connection != NULL) {
    return true;
  } else {
    if (client->connection != NULL) {
      return true;
    }
  }

  return false;
}

static ImVec2 GetARelativeToB(XMFLOAT2 a, XMFLOAT2 b) {
  return {a.x - b.x, 
          b.y - a.y};
}

static void RenderGameInterface(Game *game) {
  ImGuiIO &io = ImGui::GetIO();

  auto player = &game->player;
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0, 1.0f, 0, 1));
  ImGui::ProgressBar(player->stamina, ImVec2(100.0f, 0.0f));
  ImGui::PopStyleColor();

  ImGui::Text("Player hp -> %.3f", player->hp);

  auto weapon = &game->weapon;
  ImGui::Text("Weapon angle -> %.3f", weapon->angle);

  auto bullets = &game->bullets;
  ImGui::Text("Bullets count -> %d", arrlen(bullets->mesh_instance_data));

  auto dummy = &game->dummy;
  ImGui::Text("Dummy hp -> %.3f", dummy->hp);

  auto animation = &hmget(game->animations, player->animation_type);
  ImGui::Text("Animation index, time -> %d, %.3f", animation->index, animation->time);

  // If not multiplayer, render hp for dummy
  ImVec2 offset = {player->size.x * 2, player->size.y * 2};
  if (!IsConnected(&game->server, &game->client)) {
    ImVec2 dummy_position = GetARelativeToB(dummy->position, player->position);

    ImGui::SetCursorPos(io.DisplaySize / 2.0f + dummy_position - offset);
    DrawHpBar(dummy->hp, s_DummyHp);
  } else {
    // Render hp for Host / Client
    auto connection = GetConnection(&game->server);
    ConnectionData *connection_data = NULL;
    if (connection != NULL) {
      connection_data = &connection->client_data;
    } else {
      connection = game->client.connection;
      if (connection != NULL) {
        connection_data = &connection->host_data;
      }
    }

    // Not actually needed, but just in case
    assert(connection_data != NULL);

    ImVec2 relative_position = GetARelativeToB(connection_data->position, player->position);
    ImGui::SetCursorPos(io.DisplaySize / 2.0f + relative_position - offset);
    DrawHpBar(connection_data->hp, s_PlayerHp);
  }

  ImGui::SetCursorPos(io.DisplaySize / 2.0f - offset);
  DrawHpBar(player->hp, s_PlayerHp, ImVec4(1.0f, 0, 0, 1));
}

static void RenderBegin(Directx *directx) {
  ID3D11DeviceContext *device_context = directx->device_context;

  const float clear_color[] = {0.1, 0.1, 0.2, 1};
  device_context->ClearRenderTargetView(directx->render_target_view,
                                                 clear_color);
  device_context->ClearDepthStencilView(
      directx->depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
      1.0f, 0);
  device_context->RSSetViewports(1, &directx->viewport);
  device_context->OMSetRenderTargets(1, &directx->render_target_view,
                                              directx->depth_stencil_view);
  device_context->IASetPrimitiveTopology(
      D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Used to rendering opaque textures
  device_context->OMSetDepthStencilState(directx->depth_stencil_state,
                                                  0);
  const float blend_factor[] = {0, 0, 0, 0};
      device_context->OMSetBlendState(directx->blend_state,
      blend_factor, 0xffffffff);
}

static void Render(Game *game) {
  Directx *directx = &game->directx;

  RenderBegin(directx);
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

  switch (s_State) {
    case State_Menu:
      RenderMenuInterface(game);
      break;
    case State_Game:
      RenderGame(game);
      RenderGameInterface(game);
      break;
    default:
      assert(0);
      break;
  }
  
  ImGui::PopStyleVar(2);
  ImGui::End();

  ImGuiEnd();
  directx->swap_chain->Present(0, 0);
}

static void Run(Game *game) {
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

    if (IsWindowActive(&game->window_data)) {
      extra_time += time;

      while (extra_time >= frame_time) {
        if (s_State == State_Game) Input(game, dt);
        Update(game, dt);

        extra_time -= frame_time;
      }

      Render(game);
    } else {
      Sleep(5);
    }

    /////// END FRAME ///////

    old_time = new_time;
  }

  // Just in case of rage quit, close this shit
  StopServer(&game->server);
  StopClient(&game->client);
}