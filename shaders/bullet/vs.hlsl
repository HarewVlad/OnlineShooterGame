cbuffer ConstantBuffer : register(b0)
{
  float4x4 mvp;
};

struct VS_IN
{
  float2 pos : position;
  float2 tex : texcoord;
  float2 offset : texcoord1;
  float2 size : texcoord2;
};

struct VS_OUT
{
  float4 pos : sv_position;
  float2 tex : texcoord;
};

VS_OUT VS(VS_IN input)
{
  VS_OUT vout;
  
  // For instance elements
  input.pos *= input.size;
  input.pos += input.offset;

  vout.pos = mul(float4(input.pos, 0.0f, 1.0f), mvp);
  vout.tex = input.tex;

  return vout;
}