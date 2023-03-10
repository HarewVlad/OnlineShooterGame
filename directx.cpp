static void CreateRenderTarget(Directx *directx) {
  ID3D11Texture2D *back_buffer;
  directx->swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
  directx->device->CreateRenderTargetView(back_buffer, NULL,
                                          &directx->render_target_view);
  back_buffer->Release();
}

static void CreateDepthStencilView(Directx *directx, int width, int height) {
  D3D11_TEXTURE2D_DESC depth_stencil_desc = {};
  depth_stencil_desc.Width = width;
  depth_stencil_desc.Height = height;
  depth_stencil_desc.MipLevels = 1;
  depth_stencil_desc.ArraySize = 1;
  depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depth_stencil_desc.SampleDesc.Count = 1;
  depth_stencil_desc.SampleDesc.Quality = 0;
  depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
  depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depth_stencil_desc.CPUAccessFlags = 0;
  depth_stencil_desc.MiscFlags = 0;

  assert(directx->device->CreateTexture2D(&depth_stencil_desc, nullptr,
                                          &directx->depth_stencil) == S_OK);
  assert(directx->device->CreateDepthStencilView(
             directx->depth_stencil, nullptr, &directx->depth_stencil_view) ==
         S_OK);
}

static void InitializeDirectx(Directx *directx, HWND window, int width, int height) {
  DXGI_SWAP_CHAIN_DESC scd = {};
  scd.BufferCount = 2;
  scd.BufferDesc.Width = 0;
  scd.BufferDesc.Height = 0;
  scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  scd.BufferDesc.RefreshRate.Numerator = 60;
  scd.BufferDesc.RefreshRate.Denominator = 1;
  scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
  scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  scd.OutputWindow = window;
  scd.SampleDesc.Count = 1;
  scd.SampleDesc.Quality = 0;
  scd.Windowed = TRUE;
  scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  UINT create_device_flags = 0;
  D3D_FEATURE_LEVEL feature_level;
  const D3D_FEATURE_LEVEL feature_levels[2] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_0,
  };
  
  assert(D3D11CreateDeviceAndSwapChain(
             NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, create_device_flags,
             feature_levels, 2, D3D11_SDK_VERSION, &scd, &directx->swap_chain,
             &directx->device, &feature_level,
             &directx->device_context) == S_OK);

  // Sampler
  D3D11_SAMPLER_DESC sd = {};
  sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sd.MinLOD = 0;
  sd.MaxLOD = D3D11_FLOAT32_MAX;
  assert(directx->device->CreateSamplerState(&sd, &directx->sampler) == S_OK);

  // Viewport
  directx->viewport.Width = (FLOAT)width;
  directx->viewport.Height = (FLOAT)height;
  directx->viewport.MinDepth = 0.0f;
  directx->viewport.MaxDepth = 1.0f;
  directx->viewport.TopLeftX = 0;
  directx->viewport.TopLeftY = 0;

  // Depth stencil state
  D3D11_DEPTH_STENCIL_DESC dssd = {};
  dssd.DepthEnable = true;
  dssd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  dssd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
  dssd.StencilEnable = true;
  dssd.StencilReadMask = 0xFF;
  dssd.StencilWriteMask = 0xFF;

  dssd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  dssd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  dssd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  dssd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  dssd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  dssd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  dssd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  dssd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  assert(directx->device->CreateDepthStencilState(&dssd, &directx->depth_stencil_state) == S_OK);

  // Blend state
  D3D11_BLEND_DESC bsd = {};
  bsd.RenderTarget[0].BlendEnable = TRUE;
  bsd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
  bsd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
  bsd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
  bsd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
  bsd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
  bsd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  bsd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

  assert(directx->device->CreateBlendState(&bsd, &directx->blend_state) == S_OK);

  CreateRenderTarget(directx);
  CreateDepthStencilView(directx, width, height);
}

static ID3D11InputLayout *CreateInputLayout(ID3D11Device *device,
                       D3D11_INPUT_ELEMENT_DESC *vertex_element_desc, UINT size,
                       VertexShader *vertex_shader) {
  ID3D11InputLayout *result = NULL;
  assert(device->CreateInputLayout(
             vertex_element_desc, size,
             vertex_shader->blob->GetBufferPointer(),
             vertex_shader->blob->GetBufferSize(), &result) == S_OK);

  return result;
}

static void UpdateBuffer(ID3D11Buffer *buffer, const void *data, size_t size,
                  ID3D11DeviceContext *device_context) {
  D3D11_MAPPED_SUBRESOURCE resource;
  device_context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
  memcpy(resource.pData, data, size);
  device_context->Unmap(buffer, 0);
}

static ID3D11Buffer *CreateBuffer(ID3D11Device *device, D3D11_USAGE usage, UINT type, UINT cpu_flags, void *data, UINT size) {
  ID3D11Buffer *result = NULL;

  D3D11_BUFFER_DESC bd = {};
  bd.Usage = usage;
  bd.BindFlags = type;
  bd.ByteWidth = size;
  bd.CPUAccessFlags = cpu_flags;
  
  if (data != NULL) {
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = data;
    assert(device->CreateBuffer(&bd, &sd, &result) == S_OK);
  } else {
    assert(device->CreateBuffer(&bd, NULL, &result) == S_OK);
  }

  return result;
}

static ID3D11ShaderResourceView *CreateTexture(ID3D11Device *device, const char *filename) {
  ID3D11ShaderResourceView *result = nullptr;

  int width;
  int height;
  int bpp;
  unsigned char *pixels = stbi_load(filename, &width, &height, &bpp, 4);

  D3D11_TEXTURE2D_DESC td = {};
  td.Width = width;
  td.Height = height;
  td.MipLevels = 1;
  td.ArraySize = 1;
  td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  td.SampleDesc.Count = 1;
  td.Usage = D3D11_USAGE_DEFAULT;
  td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
  td.CPUAccessFlags = 0;

  ID3D11Texture2D *texture = nullptr;
  D3D11_SUBRESOURCE_DATA sd;
  sd.pSysMem = pixels;
  sd.SysMemPitch = td.Width * 4;
  sd.SysMemSlicePitch = 0;
  assert(device->CreateTexture2D(&td, &sd, &texture) == S_OK);

  D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
  srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  srvd.Texture2D.MipLevels = td.MipLevels;
  srvd.Texture2D.MostDetailedMip = 0;
  assert(device->CreateShaderResourceView(texture, &srvd, &result) == S_OK);

  texture->Release();

  return result;
}