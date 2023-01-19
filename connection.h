struct ConnectionData {
  XMFLOAT2 position;
  float hp;
};

struct Connection {
  tcp::socket socket;
  ConnectionData host_data;
  ConnectionData client_data;
};