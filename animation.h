struct Animation {
  int index;
  int frames;
  float time;
  XMFLOAT2 uv[4]; // TODO: Probably bad for cache here (but it is easier to initialize). Check performance
};

static void InitializeAnimation(Animation *animation, int index, int frames, float time);
static void UpdateAnimation(Animation *animation, float dt);