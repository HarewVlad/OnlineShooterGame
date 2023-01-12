static void InitializeBullets(Bullets *bullets, ID3D11Device *device, int count) {
  // Data in this buffer will be updated once per frame
  bullets->instance_buffer =
      CreateBuffer(device, D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER,
                   D3D11_CPU_ACCESS_WRITE, bullets->mesh_instance_data,
                   sizeof(MeshInstanceData) * count);
}

static void ShootBullet(Bullets *bullets, CXMVECTOR direction, XMFLOAT2 position) {
  // Bullet has been shooted, store start information
  arrput(bullets->mesh_instance_data,
         (MeshInstanceData{position, s_BulletSize}));
  arrput(bullets->bullet_instance_data,
         (BulletInstanceData{direction, position}));
}

static void UpdateBullets(Bullets *bullets, float max_distance, float speed) {
  // Update positions of bullets based on initial shoot direction
  for (int i = 0; i < arrlen(bullets->mesh_instance_data); i++) {
    BulletInstanceData bullet_instance_data = bullets->bullet_instance_data[i];
    MeshInstanceData *mesh_instance_data = &bullets->mesh_instance_data[i];

    XMFLOAT2 previous_position = mesh_instance_data->position;
    XMFLOAT2 new_position = {
        previous_position.x +
            XMVectorGetX(bullet_instance_data.direction) * speed,
        previous_position.y +
            XMVectorGetY(bullet_instance_data.direction) * speed};

    XMFLOAT2 start_position = bullet_instance_data.start_position;
    if (abs(new_position.x - start_position.x) > max_distance ||
        abs(new_position.y - start_position.y) > max_distance) {
      RemoveBullet(bullets, i);
    } else {
      mesh_instance_data->position = new_position;
    }
  }
}

static void RemoveBullet(Bullets *bullets, int index) {
  arrdel(bullets->bullet_instance_data, index);
  arrdel(bullets->mesh_instance_data, index);
}