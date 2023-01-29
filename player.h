struct Player {
  XMFLOAT2 m_position;
  XMFLOAT2 m_size;
  XMFLOAT2 m_direction = {1, 0};
  XMFLOAT2 m_velocity = {};
  float m_stamina;
  float m_hp;
  int m_animation_type;

  void Initialize(XMFLOAT2 position, XMFLOAT2 size, float stamina, float hp, int animation_type);
};