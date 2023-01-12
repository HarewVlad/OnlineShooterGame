struct PS_INPUT
{
  float4 pos : sv_position;
  float4 color : texcoord0;
  float2 tex : texcoord1;
};

float4 main(PS_INPUT input) : sv_target
{
  return float4(1, 1, 1, 1);
}