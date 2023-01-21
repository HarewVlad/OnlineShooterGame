struct Body {
  XMFLOAT2 m_min;
  XMFLOAT2 m_max;

  void Initialize(XMFLOAT2 position, XMFLOAT2 size);
};

static bool TestAABBAABB(Body *a, Body *b);
static XMFLOAT2 GetIntersectionResolutionOffset(Body *a, Body *b);