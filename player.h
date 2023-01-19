struct Player {
  XMFLOAT2 position;
  XMFLOAT2 size;
  float stamina;
  float hp;
  int animation_type;
};

static void InitializePlayer(Player *player, XMFLOAT2 position, XMFLOAT2 size, float stamina, float hp, int animation_type);