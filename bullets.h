struct BulletInstanceData {
  XMVECTOR direction;
  XMFLOAT2 start_position;
};

struct Bullets {
  MeshInstanceData *m_mesh_instance_data = NULL;
  BulletInstanceData *m_bullet_instance_data = NULL;
  ID3D11Buffer *m_instance_buffer;

  void Initialize(Directx *directx, int count);
  void Shoot(CXMVECTOR direction, XMFLOAT2 position, XMFLOAT2 size);
  void Update(float distance, float speed);
  void Remove(int index);
};