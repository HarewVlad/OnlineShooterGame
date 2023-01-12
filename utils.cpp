static int GetMilliseconds() {
  static int base = timeGetTime();

  return timeGetTime() - base;
}

static void SetRandomSeed() {
  std::random_device rd;
  s_Random.seed(rd());
}

static void AddToVector(XMFLOAT2 *vector, float x, float y) {
  vector->x += x;
  vector->y += y;
}

static void SetVector(XMFLOAT2 *vector, float x, float y) {
  vector->x = x;
  vector->y = y;
}

static void SetVector(XMINT2 *vector, int x, int y) {
  vector->x = x;
  vector->y = y;
}

static void AddToVector(XMFLOAT3 *vector, float x, float y, float z) {
  vector->x += x;
  vector->y += y;
  vector->z += z;
}

static void VectorMinus(XMFLOAT2 *a, XMFLOAT2 *b) {
  a->x -= b->x;
  a->y -= b->y;
}

static void AddToVector(XMFLOAT2 *a, XMFLOAT2 b) {
  a->x += b.x;
  a->y += b.y;
}

static XMMATRIX GetModelMatrix(XMFLOAT2 position, XMFLOAT2 size) {
  return XMMatrixScaling(size.x, size.y, 1) *
         XMMatrixTranslation(position.x, position.y, 0);
}

static XMMATRIX GetModelMatrix(XMFLOAT2 position, XMFLOAT2 size, float z) {
  return XMMatrixRotationZ(z) *
         XMMatrixScaling(size.x, size.y, 1) *
         XMMatrixTranslation(position.x, position.y, 0);
}