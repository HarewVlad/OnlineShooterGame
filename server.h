struct Session {
  tcp::acceptor m_acceptor;
  Connection *m_connection = NULL;

public:
  void ProcessClientConnection();
};

struct Server {
  Session *m_session = NULL;
  asio::io_context m_io;
  std::thread m_thread;

  void Start();
  void Stop();
  // std::string GetEnemyAddress();
  Connection *GetConnection();

private:
  void Listen();

};