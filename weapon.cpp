void Weapon::Initialize(int type, XMFLOAT2 position, XMFLOAT2 size, float angle) {
  m_type = type;
  m_position = position;
  m_size = size;
  m_angle = angle;
}

void Weapon::Rotate(int y, int height, CXMVECTOR direction) {
  XMVECTOR axis = XMVectorSet(1, 0, 0, 0);
  XMVECTOR result = XMVector2AngleBetweenNormalsEst(direction, axis);
  float angle = XMVectorGetX(result);

  // TODO: Find a way without y and height coordinates
  if (y < height) {
    m_angle = -angle;
  } else {
    m_angle = angle;
  }
}