struct BulletInstanceData {
  XMVECTOR direction;
  XMFLOAT2 start_position;
};

struct Bullets {
  MeshInstanceData *mesh_instance_data = NULL;
  
  // Not used to draw, but to update position and stuff
  BulletInstanceData *bullet_instance_data = NULL;

  ID3D11Buffer *instance_buffer;
};

static void InitializeBullets(Bullets *bullets, ID3D11Device *device, LPCWSTR ps, int count);
static void ShootBullet(Bullets *bullets, CXMVECTOR direction, XMFLOAT2 position);
static void UpdateBullets(Bullets *bullets, float max_distance, float speed);
static void RemoveBullet(Bullets *bullets, int index);