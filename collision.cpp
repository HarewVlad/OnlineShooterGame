static void InitializeBody(Body *body, XMFLOAT2 position, XMFLOAT2 size) {
  body->min = {position.x - size.x, position.y - size.y};
  body->max = {position.x + size.x, position.y + size.y};
}

static bool TestAABBAABB(Body *a, Body *b) {
  // Exit with no intersection if separated along an axis
  if (a->max.x < b->min.x || a->min.x > b->max.x)
    return false;
  if (a->max.y < b->min.y || a->min.y > b->max.y)
    return false;

  // Overlapping on all axes means AABBs are intersecting
  return true;
}

// static DirectX::XMFLOAT2 GetIntersectionResolutionOffset(Body *a, Body *b) {
//   //        ^ y
//   //        |
//   //        e1
//   //     -------- max
//   //    |        |
//   // e0 |        | e2 --> x
//   //    |        |
//   // min --------
//   //        e3

//   const XMFLOAT2 sizes[] = {{a->max.x - b->min.x, 0},
//                             {0, b->max.y - a->min.y},
//                             {b->max.x - a->min.x, 0},
//                             {0, a->max.y - b->min.y}};

//   const float correction = 0.01f;
//   const XMFLOAT2 offsets[] = {{b->min.x - a->max.x - correction, 0},
//                               {0, b->max.y - a->min.y + correction},
//                               {b->max.x - a->min.x + correction, 0},
//                               {0, b->min.y - a->max.y - correction}};

//   XMFLOAT2 result = {};
//   XMFLOAT2 min_size = {FLT_MAX / 2, FLT_MAX / 2};
//   for (int i = 0; i < _countof(sizes); i++) {
//     if (min_size.x + min_size.y > sizes[i].x + sizes[i].y) {
//       min_size = sizes[i];
//       result = offsets[i];
//     }
//   }

//   return result;
// }

static DirectX::XMFLOAT2 GetIntersectionResolutionOffset(Body *a, Body *b) {
  // Calculate the overlap in both the x and y axes
  float x_overlap = std::min(a->max.x, b->max.x) - std::max(a->min.x, b->min.x);
  float y_overlap = std::min(a->max.y, b->max.y) - std::max(a->min.y, b->min.y);

  // If the x overlap is smaller, the collision is happening in the x axis
  if (x_overlap < y_overlap) {
    return XMFLOAT2 { (a->min.x < b->min.x) ? -x_overlap : x_overlap, 0 };
  }
  else { // On y axes
    return XMFLOAT2{ 0, (a->min.y < b->min.y) ? -y_overlap : y_overlap };
  }
}