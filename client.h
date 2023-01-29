struct Client {
  Connection *m_connection = NULL;
  asio::io_context m_io;
  std::thread m_thread;

  void Start(const char *ip, const char *port);
  void Stop();

private:
  void Connect(const tcp::resolver::results_type &endpoints);
  void ProcessServerConnection();
};