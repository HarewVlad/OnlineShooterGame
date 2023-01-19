static void ProceedClientConnection(Connection *connection) {
  // Read client player data
  asio::async_read(
      connection->socket,
      asio::buffer(&connection->client_data, sizeof(ConnectionData)),
      [connection](std::error_code error, size_t length) {
        if (!error) {
          // Send our player data
          asio::async_write(
              connection->socket,
              asio::buffer(&connection->host_data, sizeof(ConnectionData)),
              [connection](std::error_code error, std::size_t length) {
                if (!error) {
                  // Send modified player data
                  ProceedClientConnection(connection);
                }
              });
        }
      });

  // Later can save bandwidth by implementing player position prediction and
  // sending correct position data in time intervals
  // asio::steady_timer t(io, asio::chrono::seconds(5));
  // t.async_wait(&print);
}

static void SessionListen(Session *session) {
  session->acceptor.async_accept(
      [session](std::error_code error, tcp::socket socket) {
        if (!session->acceptor.is_open()) {
          return;
        }

        if (!error) {
          session->connection = new Connection{std::move(socket)};
          ProceedClientConnection(session->connection);
        }

        SessionListen(session);
      });
}

static void StartServer(Server *server) {
  tcp::endpoint endpoint(tcp::v4(), 1234);

  // Shit to store tcp::acceptor
  server->session = new Session{tcp::acceptor(server->io, endpoint), NULL};
  SessionListen(server->session);

  // As we have at list 1 async operation all the time, run the loop
  server->thread = std::thread([server]() { server->io.run(); });
}

static void StopServer(Server *server) {
  server->io.stop();
  if (server->thread.joinable()) {
    server->thread.join();
  }

  // To be able to launch server again
  server->io.reset();

  // Clean allocations
  Session *session = server->session;
  if (session != NULL) {
    Connection *connection = session->connection;
    if (connection != NULL) {
      delete connection;
    }
    delete session;
    server->session = NULL;
  }
}

static std::string GetEnemyAddress(Server *server) {
  std::string result;

  Session *session = server->session;
  if (session != NULL) {
    Connection *connection = session->connection;
    if (connection != NULL) {
      result = connection->socket.remote_endpoint().address().to_string();
    }
  }

  return result;
}

static Connection *GetConnection(Server *server) {
  Session *session = server->session;
  if (session != NULL) {
    return session->connection;
  }

  return NULL;
}