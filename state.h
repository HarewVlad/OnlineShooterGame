enum States {
  State_None = 0,
  State_Menu = 1 << 0,
  State_Game = 1 << 1,
  State_Exit = 1 << 2
};

enum Flags {
  Flag_None = 0,
  Flag_Host = 1 << 0,
  Flag_Client = 1 << 1,
};

static int s_State = State_None;
static int s_Flags = Flag_None;

static bool CheckFlag(int flag);
static void SetFlag(int flag);
static void RemoveFlag(int flag);