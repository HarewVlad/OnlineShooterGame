// TODO: Create namespaces may be like Player, Game, Bullet, ...

static const int s_Fps = 144;
static const int s_Width = GetSystemMetrics(SM_CXSCREEN);
static const int s_HalfWidth = s_Width / 2;
static const int s_Height = GetSystemMetrics(SM_CYSCREEN);
static const int s_HalfHeight = s_Height / 2;
static const int s_MapWidth = 2000;
static const int s_MapHeight = 2000;
static const XMFLOAT2 s_PlayerSpawnPosition = {s_MapWidth * 0.2f, s_MapHeight * 0.2f};
static const XMFLOAT2 s_EnemySpawnPosition = {s_MapWidth - s_MapWidth * 0.2f, s_MapHeight - s_MapHeight * 0.2f};
static const float s_PlayerSpeed = 2.0f;
static const float s_PlayerRunSpeed = 3.5f;
static const float s_PlayerStamina = 1.0f;
static const XMFLOAT2 s_PlayerSize = {15, 15};
static const XMFLOAT2 s_WeaponSize = {32, 32};
static const float s_WeaponDefaultAngle = XMConvertToRadians(-90);
static const float s_BulletMaxDistance = 1000; // For now every gun have the same shoot speed and distance
static const int s_MaxBullets = 256; // Max instance buffer size, we will be using it as circle buffer
static const float s_BulletSpeed = 24.0f;
static const float s_ShootFrequency = 0.1f; // Bullet per second // TODO: Different guns have different shot speed, add that
static const XMFLOAT2 s_BulletSize = {3, 3};
static const float s_PlayerHp = 100.0f;
static const XMFLOAT2 s_DummyPosition = {1000, 1000};
static const float s_DummyHp = 1000.0f;
static const float s_DummyRegenRate = 100.0f; // HP per second