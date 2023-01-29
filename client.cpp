void Client::Start(const char *ip, const char *port) {
  tcp::resolver resolver(m_io);
  auto endpoints = resolver.resolve(ip, port);

  // Just to store fucking tcp::socket
  m_connection = new Connection{tcp::socket(m_io)};
  Connect(endpoints);

  // This will run our async operations
  m_thread = std::thread([this]() { m_io.run(); });
}

void Client::Stop() {
  if (m_connection != NULL) {
    asio::post(m_io, [this]() { m_connection->socket.close(); });
    m_thread.join();

    // Clean allocations
    delete m_connection;
    m_connection = NULL;
  }
}

void Client::Connect(const tcp::resolver::results_type &endpoints) {
  asio::async_connect(m_connection->socket, endpoints,
                      [this](std::error_code error, tcp::endpoint) {
                        if (!error) {
                          ProcessServerConnection();
                        } else {
                          m_connection->socket.close();
                        }
                      });
}

void Client::ProcessServerConnection() {
  // Send client player data to server
  asio::async_write(
      m_connection->socket,
      asio::buffer(&m_connection->client_data, sizeof(ConnectionData)),
      [this](std::error_code error, std::size_t length) {
        if (!error) {
          // Read host player data
          asio::async_read(
              m_connection->socket,
              asio::buffer(&m_connection->host_data, sizeof(ConnectionData)),
              [this](std::error_code error, size_t length) {
                if (!error) {
                  // Repeat
                  ProcessServerConnection();
                }
              });
        }
      });

  // Later can save bandwidth by implementing player position prediction and
  // sending correct position data in time intervals
  // asio::steady_timer t(io, asio::chrono::seconds(5));
  // t.async_wait(&print);
}