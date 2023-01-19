static void InitializePlayer(Player *player, XMFLOAT2 position, XMFLOAT2 size, float stamina, float hp, int animation_type) {
  player->position = position;
  player->size = size;
  player->stamina = stamina;
  player->hp = hp;
  player->animation_type = animation_type;
}