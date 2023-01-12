struct ClientData {
  XMFLOAT2 position;
};

struct HostData {
  XMFLOAT2 position;
};

struct Connection {
  tcp::socket socket;
  HostData host_data;
  ClientData client_data;
};