struct Player {
  XMFLOAT2 position;
  XMFLOAT2 size;
  float stamina;
  float hp;
};

static void InitializePlayer(Player *player, XMFLOAT2 position, XMFLOAT2 size, float stamina, float hp);