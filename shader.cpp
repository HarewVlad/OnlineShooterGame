static ID3D10Blob *CompileShader(LPCWSTR filename, const char *target) {
  ID3D10Blob *result = NULL;
  ID3D10Blob *compilation_message = NULL;
  if (D3DCompileFromFile(filename, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE,
                         "main", target, D3DCOMPILE_DEBUG, 0, &result,
                         &compilation_message) != S_OK) {
    if (compilation_message != NULL)
      OutputDebugStringA((char *)compilation_message->GetBufferPointer());
  }

  return result;
}

static void CreateVertexShader(VertexShader *shader, ID3D11Device *device,
                        LPCWSTR filename) {
  shader->blob = CompileShader(filename, "vs_5_0");

  assert(device->CreateVertexShader(shader->blob->GetBufferPointer(),
                                    shader->blob->GetBufferSize(), NULL,
                                    &shader->shader) == S_OK);
}

static void CreatePixelShader(PixelShader *shader, ID3D11Device *device,
                       LPCWSTR filename) {
  shader->blob = CompileShader(filename, "ps_5_0");

  assert(device->CreatePixelShader(shader->blob->GetBufferPointer(),
                                   shader->blob->GetBufferSize(), NULL,
                                   &shader->shader) == S_OK);
}