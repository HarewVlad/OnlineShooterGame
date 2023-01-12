static void InitializeCamera(Camera *camera, XMFLOAT2 position) {
  camera->position = {position.x, position.y, -1};
  camera->target = {position.x, position.y, 1};
}

static void MoveCamera(Camera *camera, float x, float y, float z) {
  AddToVector(&camera->position, x, y, z);
  AddToVector(&camera->target, x, y, z);
}

static XMMATRIX GetCameraView(Camera *camera) {
  return XMMatrixLookAtLH(XMLoadFloat3(&camera->position), XMLoadFloat3(&camera->target),
      XMVECTOR{0, 1, 0}); 
}