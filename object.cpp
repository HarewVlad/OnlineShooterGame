void InitializeGraphicsObject(GraphicsObject *graphics_object,
                              ID3D11Device *device,
                              D3D11_INPUT_ELEMENT_DESC *vertex_element_desc,
                              UINT size, LPCWSTR vs, LPCWSTR ps) {
  CreateVertexShader(&graphics_object->vertex_shader, device, vs);
  CreatePixelShader(&graphics_object->pixel_shader, device, ps);

  graphics_object->input_layout = CreateInputLayout(
      device, vertex_element_desc, size, &graphics_object->vertex_shader);
}

void InitializeGraphicsObject(GraphicsObject *graphics_object,
                              ID3D11Device *device, LPCWSTR vs, LPCWSTR ps) {
  CreateVertexShader(&graphics_object->vertex_shader, device, vs);
  CreatePixelShader(&graphics_object->pixel_shader, device, ps);
}
