struct Camera {
  XMFLOAT3 m_position;
  XMFLOAT3 m_target;

  void Initialize(XMFLOAT2 position);
  void Move(float x, float y, float z = 0);
  // TODO: Check performance here
  XMMATRIX GetView();
};