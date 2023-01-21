void Player::Initialize(XMFLOAT2 position, XMFLOAT2 size, float stamina, float hp, int animation_type) {
  m_position = position;
  m_size = size;
  m_stamina = stamina;
  m_hp = hp;
  m_animation_type = animation_type;
}