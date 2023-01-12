static void InitializePlayer(Player *player, XMFLOAT2 position, XMFLOAT2 size, float stamina, float hp) {
  player->position = position;
  player->size = size;
  player->stamina = stamina;
  player->hp = hp;
}