struct Client {
  Connection *connection = NULL;
  asio::io_context io;
  std::thread thread;
};

static void StartClient(Client *client, const char *ip, const char *port);
static void StopClient(Client *client);