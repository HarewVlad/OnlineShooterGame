struct Animation {
  int m_frames;
  int m_index = 0;
  float m_time = 0.0f;
  XMFLOAT2 m_uv[4] = {}; // TODO: Probably bad for cache here (but it is easier to initialize). Check performance

  void Update(float dt);
  void Reset();
};