struct Network {
  Client m_client;
  Server m_server;

  void StartAsHost();
  void StartAsClient()
};