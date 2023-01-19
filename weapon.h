struct Weapon {
  XMFLOAT2 position;
  XMFLOAT2 size;
  int type;
  float angle;
};

void InitializeWeapon(Weapon *weapon, int type, XMFLOAT2 position, XMFLOAT2 size, float angle);
void WeaponRotate(Weapon *weapon, int y, int height, CXMVECTOR direction);