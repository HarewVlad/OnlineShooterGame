static void CreateSquare(Mesh *mesh, ID3D11Device *device) {
  Vertex vertices[4] = {
    Vertex{DirectX::XMFLOAT2{-1, -1}, DirectX::XMFLOAT2{0, 1}},
    Vertex{DirectX::XMFLOAT2{-1, 1}, DirectX::XMFLOAT2{0, 0}},
    Vertex{DirectX::XMFLOAT2{1, 1}, DirectX::XMFLOAT2{1, 0}},
    Vertex{DirectX::XMFLOAT2{1, -1}, DirectX::XMFLOAT2{1, 1}}
  };
  int indices[6] = {0, 1, 2, 0, 2, 3};

  // We don't update this buffers, thats why it is D3D11_USAGE_IMMUTABLE
  mesh->index_buffer = CreateBuffer(device, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, &indices[0], sizeof(int) * 6);
  mesh->vertex_buffer = CreateBuffer(device, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, &vertices[0], sizeof(Vertex) * 4);
}