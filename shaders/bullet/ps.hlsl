struct VS_OUT
{
  float4 pos : sv_position;
  float2 tex : texcoord;
};

float4 PS(VS_OUT input) : SV_Target
{
  return float4(1, 1, 1, 1);
}
