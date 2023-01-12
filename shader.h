struct VertexShader {
  ID3D11VertexShader *shader;
  ID3D10Blob *blob;
};

struct PixelShader {
  ID3D11PixelShader *shader;
  ID3D10Blob *blob;
};

static void CreateVertexShader(VertexShader *shader, ID3D11Device *device,
                        LPCWSTR filename);
static void CreatePixelShader(PixelShader *shader, ID3D11Device *device,
                       LPCWSTR filename);

