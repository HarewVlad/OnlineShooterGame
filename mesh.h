struct MeshInstanceData {
  XMFLOAT2 position;
  XMFLOAT2 size;
};

struct Mesh {
  ID3D11Buffer *m_index_buffer;
  ID3D11Buffer *m_vertex_buffer;

  void InitializeAsSquare(Directx *directx);
};