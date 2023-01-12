// Just leave it here, i don't care
struct MeshInstanceData {
  XMFLOAT2 position;
  XMFLOAT2 size;
};

struct Mesh {
  ID3D11Buffer *index_buffer;
  ID3D11Buffer *vertex_buffer;
};

static void CreateSquare(Mesh *mesh, ID3D11Device *device);