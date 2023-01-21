struct Weapon {
  XMFLOAT2 m_position;
  XMFLOAT2 m_size;
  int m_type;
  float m_angle;

  void Initialize(int type, XMFLOAT2 position, XMFLOAT2 size, float angle);
  void Rotate(int y, int height, CXMVECTOR direction);
};