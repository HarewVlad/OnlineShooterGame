struct GraphicsObject {
  VertexShader vertex_shader;
  PixelShader pixel_shader;
  ID3D11InputLayout *input_layout;
};

void InitializeGraphicsObject(GraphicsObject *graphics_object,
                              ID3D11Device *device,
                              D3D11_INPUT_ELEMENT_DESC *input_element_desc,
                              UINT size, LPCWSTR vs, LPCWSTR ps);