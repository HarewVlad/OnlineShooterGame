void Bullets::Initialize(Directx *directx, int count) {
  m_instance_buffer =
      directx->CreateBuffer(D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER,
                   D3D11_CPU_ACCESS_WRITE, m_mesh_instance_data,
                   sizeof(MeshInstanceData) * count);
}

void Bullets::Shoot(CXMVECTOR direction, XMFLOAT2 position, XMFLOAT2 size) {
  // Bullet has been shooted, store initial information
  arrput(m_mesh_instance_data,
         (MeshInstanceData{position, size}));
  arrput(m_bullet_instance_data,
         (BulletInstanceData{direction, position}));
}

void Bullets::Update(float distance, float speed) {
  // Update positions of bullets based on initial shoot direction
  for (int i = 0; i < arrlen(m_mesh_instance_data); i++) {
    BulletInstanceData bullet_instance_data = m_bullet_instance_data[i];
    MeshInstanceData *mesh_instance_data = &m_mesh_instance_data[i];

    XMFLOAT2 previous_position = mesh_instance_data->position;
    XMFLOAT2 new_position = {
        previous_position.x +
            XMVectorGetX(bullet_instance_data.direction) * speed,
        previous_position.y +
            XMVectorGetY(bullet_instance_data.direction) * speed};

    XMFLOAT2 start_position = bullet_instance_data.start_position;
    if (abs(new_position.x - start_position.x) > distance ||
        abs(new_position.y - start_position.y) > distance) {
      Remove(i);
    } else {
      mesh_instance_data->position = new_position;
    }
  }
}

void Bullets::Remove(int index) {
  arrdel(m_bullet_instance_data, index);
  arrdel(m_mesh_instance_data, index);
}