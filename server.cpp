void Session::ProcessClientConnection() {
  // Read client player data
  asio::async_read(
      m_connection->socket,
      asio::buffer(&m_connection->client_data, sizeof(ConnectionData)),
      [this](std::error_code error, size_t length) {
        if (!error) {
          // Send our player data
          asio::async_write(
              m_connection->socket,
              asio::buffer(&m_connection->host_data, sizeof(ConnectionData)),
              [this](std::error_code error, std::size_t length) {
                if (!error) {
                  // Send modified player data
                  ProcessClientConnection();
                }
              });
        }
      });

  // Later can save bandwidth by implementing player position prediction and
  // sending correct position data in time intervals
  // asio::steady_timer t(io, asio::chrono::seconds(5));
  // t.async_wait(&print);
}

void Server::Listen() {
  m_session->m_acceptor.async_accept(
      [this](std::error_code error, tcp::socket socket) {
        if (!m_session->m_acceptor.is_open()) {
          return;
        }

        if (!error) {
          m_session->m_connection = new Connection{std::move(socket)};
          m_session->ProcessClientConnection();
        }

        Listen();
      });
}

void Server::Start() {
  tcp::endpoint endpoint(tcp::v4(), 1234);

  // Shit to store tcp::acceptor
  m_session = new Session{tcp::acceptor(m_io, endpoint), NULL};
  Listen();

  // As we have at list 1 async operation all the time, run the loop
  m_thread = std::thread([this]() { m_io.run(); });
}

void Server::Stop() {
  m_io.stop();
  if (m_thread.joinable()) {
    m_thread.join();
  }

  // To be able to launch server again
  m_io.reset();

  // Clean allocations
  if (m_session != NULL) {
    if (m_session->m_connection != NULL) {
      delete m_session->m_connection;
    }
    delete m_session;
    m_session = NULL;
  }
}

// static std::string GetEnemyAddress(Server *server) {
//   std::string result;

//   Session *session = server->session;
//   if (session != NULL) {
//     Connection *connection = session->connection;
//     if (connection != NULL) {
//       result = connection->socket.remote_endpoint().address().to_string();
//     }
//   }

//   return result;
// }

Connection *Server::GetConnection() {
  if (m_session != NULL) {
    return m_session->m_connection;
  }

  return NULL;
}