static void InitializeDefaultMap(Map *map, ID3D11Device *device, int width,
                                 int height) {
  // Make a map like something like this:
  // - - - - -- - - -
  // -               -
  // -   ----  ----  -
  // -   -        -  -
  // -   -        -  -
  // -   -        -  -
  // -   ----  ----  -
  // -               -
  // -               -
  // - -- - - - - -- -

  XMFLOAT2 block_size = {40, 40};
  int offset = 50;

  XMINT2 increment = {static_cast<int>(block_size.x) + offset,
                      static_cast<int>(block_size.y) + offset};

  XMFLOAT2 last_block_position = {
      static_cast<float>(width / (increment.x - 1) * increment.x),
      static_cast<float>(height - increment.y)};

  // Top, bottom
  for (int x = 0; x < width; x += increment.x) {
    arrput(map->mesh_instance_data,
           (MeshInstanceData{XMFLOAT2{static_cast<float>(x), 0}, block_size}));
    arrput(map->mesh_instance_data,
           (MeshInstanceData{
               XMFLOAT2{static_cast<float>(x), last_block_position.y},
               block_size}));
  }

  for (int y = 0; y < height; y += increment.y) {
    arrput(map->mesh_instance_data,
           (MeshInstanceData{XMFLOAT2{0, static_cast<float>(y)}, block_size}));
    arrput(map->mesh_instance_data,
           (MeshInstanceData{
               XMFLOAT2{last_block_position.x, static_cast<float>(y)},
               block_size}));
  }

  map->instance_buffer =
      CreateBuffer(device, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0,
                   map->mesh_instance_data,
                   sizeof(MeshInstanceData) *
                       static_cast<UINT>(arrlen(map->mesh_instance_data)));
}