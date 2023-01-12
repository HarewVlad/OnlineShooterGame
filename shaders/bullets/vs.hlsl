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
  float4 color : texcoord0;
  float2 tex : texcoord1;
};

VS_OUT main(VS_IN input, float time : TIME)
{
  VS_OUT vout;
  
  // For instance elements
  input.pos *= input.size;
  input.pos += input.offset;

  vout.pos = mul(float4(input.pos, 0.0f, 1.0f), mvp);

  // Swirling effect
  {
    // float swirlRadius = 0.01;
    // float swirlAngle = 3.14;

    // Calculate swirl factor
    // float swirlFactor = atan2(vout.pos.y, vout.pos.x);
    // float r = length(vout.pos);
    // float swirlX = r * cos(swirlFactor + swirlAngle * (r / swirlRadius));
    // float swirlY = r * sin(swirlFactor + swirlAngle * (r / swirlRadius));

    // Modify position
    // vout.pos.xy = float2(swirlX, swirlY);
  }

  // Plasma effect (For color generation)
  {
    float timeScale = 0.5;
    float colorScale = 0.5;

    // Calculate plasma values
    float value1 = sin(vout.pos.x * 10.0 + time * timeScale);
    float value2 = sin((vout.pos.y + vout.pos.x) * 10.0 + time * timeScale);
    float value3 = sin(sqrt(vout.pos.x * vout.pos.x + vout.pos.y * vout.pos.y) * 10.0 + time * timeScale);

    // Calculate color (Return this as pos and will get interesting results =))
    //vout.color = float4(
        //(value1 + value2 + value3) * colorScale,
        //(value1 + value2 - value3) * colorScale,
        //(value1 - value2 + value3) * colorScale,
        //1.0
    //);
  }

  // Gravitational lensing
  {
    float lensRadius = 3;
    float lensStrength = 1500.0;
    float colorScale = 10;

    // Calculate lensing factor
    float r = length(vout.pos);
    float lensFactor = 1.0 / (1.0 + lensStrength * (r / lensRadius) * (r / lensRadius));

    // Modify position
    // vout.pos = vout.pos * lensFactor;
    // vout.color = float4(0.0, 0.0, 0.0, 1.0) * lensFactor * colorScale;
  }

  vout.tex = input.tex;

  return vout;
}