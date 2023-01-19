struct MeshInstanceData {
  XMFLOAT2 position;
  XMFLOAT2 size;
};

struct Mesh {
  ID3D11Buffer *index_buffer;
  ID3D11Buffer *vertex_buffer;
};

static void InitializeSquare(Mesh *mesh, ID3D11Device *device);