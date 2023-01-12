struct Session {
  tcp::acceptor acceptor;
  Connection *connection = NULL;
};

struct Server {
  Session *session = NULL;
  asio::io_context io;
  std::thread thread;
};

static void StartServer(Server *server);
static void StopServer(Server *server);
static std::string GetEnemyAddress(Server *server);
static Connection *GetConnection(Server *server);