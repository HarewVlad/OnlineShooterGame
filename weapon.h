enum WeaponType { Weapon_AK47, Weapon_Anaconda, Weapon_AUG };
// enum WeaponDamage { 5, 3, 2.5f };

// TODO: HashMap<type, damage> weapon_info;
// struct WeaponInfo {
//   int type;
//   int damage;
// };

struct Weapon {
  XMFLOAT2 position;
  XMFLOAT2 size;
  int type;
  float angle;
};

void InitializeWeapon(Weapon *weapon, int type, XMFLOAT2 position, XMFLOAT2 size, float angle);
void WeaponRotate(Weapon *weapon, int y, int height, CXMVECTOR direction);