struct Directx {
  ID3D11Device *m_device;
  ID3D11DeviceContext *m_device_context;
  IDXGISwapChain *m_swap_chain;
  ID3D11RenderTargetView *m_render_target_view;
  ID3D11Texture2D *m_depth_stencil;
  ID3D11DepthStencilView *m_depth_stencil_view;
  ID3D11SamplerState *m_sampler;
  D3D11_VIEWPORT m_viewport;
  ID3D11DepthStencilState *m_depth_stencil_state;
  ID3D11BlendState *m_blend_state;

  void Initialize(HWND window, int width, int height);
  ID3D11InputLayout *
  CreateInputLayout(D3D11_INPUT_ELEMENT_DESC *vertex_element_desc, UINT size,
                  VertexShader *vertex_shader);
  ID3D11Buffer *CreateBuffer(D3D11_USAGE usage, UINT type, UINT cpu_flags, void *data, UINT size);
  void UpdateBuffer(ID3D11Buffer *buffer, const void *data, size_t size);
  ID3D11ShaderResourceView *CreateTexture(const char *filename);
  VertexShader CreateVertexShader(LPCWSTR filename);
  PixelShader CreatePixelShader(LPCWSTR filename);

private:
  void CreateRenderTarget();
  void CreateDepthStencilView(int width, int height);
  ID3D10Blob *CompileShader(LPCWSTR filename, const char *target);
};