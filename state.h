enum States {
  State_None = 0,
  State_Menu = 1 << 0,
  State_Game = 1 << 1,
  State_Exit = 1 << 2
};

enum NetworkStates {
  NetworkState_None = 0,
  NetworkState_Host = 1 << 0,
  NetworkState_Client = 1 << 1,
};

// static bool CheckFlag(int flag);
// static void SetFlag(int flag);
// static void RemoveFlag(int flag);