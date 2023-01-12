template <typename T, typename V>
struct HashMap {
  T key;
  V value;
};

static std::mt19937 s_Random;

// TODO: For some reason my assert macro is not working with strange issues, fix later
// #if _DEBUG
// #define Assert(Expression) if(!(Expression)) {__debugbreak();}
// #else
// #define Assert(Expression)
// #endif

static void SetRandomSeed();
static int GetMilliseconds();
static void AddToVector(XMFLOAT2 *vector, float x, float y);
static void AddToVector(XMFLOAT2 *a, XMFLOAT2 b);
static void AddToVector(XMFLOAT3 *vector, float x, float y, float z);
static void SetVector(XMFLOAT2 *vector, float x, float y);
static void SetVector(XMINT2 *vector, int x, int y);
static void VectorMinus(XMFLOAT2 *a, XMFLOAT2 *b);
static XMMATRIX GetModelMatrix(XMFLOAT2 position, XMFLOAT2 size);
static XMMATRIX GetModelMatrix(XMFLOAT2 position, XMFLOAT2 size, float z);