struct Map {
  MeshInstanceData *m_mesh_instance_data = NULL;
  ID3D11Buffer *m_instance_buffer;

  void Initialize(Directx *directx, int width, int height);
};