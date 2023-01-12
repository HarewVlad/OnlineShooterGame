struct Directx {
  ID3D11Device *device;
  ID3D11DeviceContext *device_context;
  IDXGISwapChain *swap_chain;
  ID3D11RenderTargetView *render_target_view;
  ID3D11Texture2D *depth_stencil;
  ID3D11DepthStencilView *depth_stencil_view;
  ID3D11SamplerState *sampler;
  D3D11_VIEWPORT viewport;
  ID3D11DepthStencilState *depth_stencil_state;
  ID3D11BlendState *blend_state;
};

static void InitializeDirectx(Directx *directx, HWND window, int width, int height);
static ID3D11InputLayout *
CreateInputLayout(ID3D11Device *device,
                  D3D11_INPUT_ELEMENT_DESC *vertex_element_desc, UINT size,
                  VertexShader *vertex_shader);
static ID3D11Buffer *CreateBuffer(ID3D11Device *device, D3D11_USAGE usage, UINT type, UINT cpu_flags, void *data, UINT size);
static void UpdateBuffer(ID3D11Buffer *buffer, const void *data, size_t size,
                  ID3D11DeviceContext *device_context);
static ID3D11ShaderResourceView *CreateTexture(ID3D11Device *device, const char *filename);