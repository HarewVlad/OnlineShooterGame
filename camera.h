struct Camera {
  XMFLOAT3 position;
  XMFLOAT3 target;
};

static void InitializeCamera(Camera *camera, XMFLOAT2 position);
static void MoveCamera(Camera *camera, float x, float y, float z = 0);
// TODO: Check performance here
static XMMATRIX GetCameraView(Camera *camera);