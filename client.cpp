static void ProceedServerConnection(Connection *connection) {
  // Send client player data to server
  asio::async_write(
      connection->socket,
      asio::buffer(&connection->client_data, sizeof(ConnectionData)),
      [connection](std::error_code error, std::size_t length) {
        if (!error) {
          // Read host player data
          asio::async_read(
              connection->socket,
              asio::buffer(&connection->host_data, sizeof(ConnectionData)),
              [connection](std::error_code error, size_t length) {
                if (!error) {
                  // Repeat
                  ProceedServerConnection(connection);
                }
              });
        }
      });

  // Later can save bandwidth by implementing player position prediction and
  // sending correct position data in time intervals
  // asio::steady_timer t(io, asio::chrono::seconds(5));
  // t.async_wait(&print);
}

static void ClientConnect(Connection *connection,
                          const tcp::resolver::results_type &endpoints) {
  asio::async_connect(connection->socket, endpoints,
                      [connection](std::error_code error, tcp::endpoint) {
                        if (!error) {
                          ProceedServerConnection(connection);
                        } else {
                          connection->socket.close();
                        }
                      });
}

static void StartClient(Client *client, const char *ip, const char *port) {
  tcp::resolver resolver(client->io);
  auto endpoints = resolver.resolve(ip, port);

  // Just to store fucking tcp::socket
  client->connection = new Connection{tcp::socket(client->io)};
  ClientConnect(client->connection, endpoints);

  // This will run our async operations
  client->thread = std::thread([client]() { client->io.run(); });
}

static void StopClient(Client *client) {
  Connection *connection = client->connection;
  if (connection != NULL) {
    asio::post(client->io, [connection]() { connection->socket.close(); });
    client->thread.join();

    // Clean allocations
    delete connection;
    client->connection = NULL;
  }
}