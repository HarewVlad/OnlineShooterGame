struct Body {
  XMFLOAT2 min;
  XMFLOAT2 max;
};

static void InitializeBody(Body *body, XMFLOAT2 position, XMFLOAT2 size);
static bool TestAABBAABB(Body *a, Body *b);
static XMFLOAT2 GetIntersectionResolutionOffset(Body *a, Body *b);