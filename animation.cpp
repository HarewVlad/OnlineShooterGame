void Animation::Update(float dt) {
  if (m_time > 1.0f / m_frames) {
    m_index = (m_index + 1) % m_frames;
    m_time = 0.0f;
  } else {
    m_time += dt;
  }

  float size = 1.0f / m_frames;
  float start = m_index * size;
  float end = start + size;

  m_uv[0] = {start, 1};
  m_uv[1] = {start, 0};
  m_uv[2] = {end, 0};
  m_uv[3] = {end, 1};
}

void Animation::Reset() {
  m_time = 0.0f;
  m_index = 0;
}