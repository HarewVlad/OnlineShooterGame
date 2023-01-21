struct Animation {
  int m_index;
  int m_frames;
  float m_time;
  XMFLOAT2 m_uv[4]; // TODO: Probably bad for cache here (but it is easier to initialize). Check performance

  void Initialize(int index, int frames, float time);
  void Update(float dt);
};