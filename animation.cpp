static void InitializeAnimation(Animation *animation, int index, int frames, float time) {
  animation->index = index;
  animation->frames = frames;
  animation->time = time;
}

static void UpdateAnimation(Animation *animation, float dt) {
  if (animation->time > 1.0f / animation->frames) {
    animation->index = (animation->index + 1) % animation->frames;
    animation->time = 0.0f;
  } else {
    animation->time += dt;
  }

  float size = 1.0f / animation->frames;
  float start = animation->index * size;
  float end = start + size;

  animation->uv[0] = {start, 1};
  animation->uv[1] = {start, 0};
  animation->uv[2] = {end, 0};
  animation->uv[3] = {end, 1};
}