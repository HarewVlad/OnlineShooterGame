struct Map {
  MeshInstanceData *mesh_instance_data = NULL;
  ID3D11Buffer *instance_buffer;
};

static void InitializeDefaultMap(Map *map, ID3D11Device *device, int width, int height);