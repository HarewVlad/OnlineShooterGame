struct VS_OUT
{
  float4 pos : sv_position;
  float2 tex : texcoord;
};

float4 main(VS_OUT input) : sv_target
{
  return float4(0, 1, 0, 1);
}
