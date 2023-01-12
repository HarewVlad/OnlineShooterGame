void InitializeWeapon(Weapon *weapon, int type, XMFLOAT2 position, XMFLOAT2 size, float angle) {
  weapon->type = type;
  weapon->position = position;
  weapon->size = size;
  weapon->angle = angle;
}

void WeaponRotate(Weapon *weapon, int y, int height, CXMVECTOR direction) {
  XMVECTOR axis = XMVectorSet(1, 0, 0, 0);
  XMVECTOR result = XMVector2AngleBetweenNormalsEst(direction, axis);
  float angle = XMVectorGetX(result);

  // TODO: Find a way without y and height coordinates
  if (y < height) {
    weapon->angle = -angle;
  } else {
    weapon->angle = angle;
  }
}