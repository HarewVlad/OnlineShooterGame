void Camera::Initialize(XMFLOAT2 position) {
  m_position = {position.x, position.y, -1};
  m_target = {position.x, position.y, 1};
}

void Camera::Move(float x, float y, float z) {
  AddToVector(&m_position, x, y, z);
  AddToVector(&m_target, x, y, z);
}

XMMATRIX Camera::GetView() {
  return XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_target),
      XMVECTOR{0, 1, 0}); 
}