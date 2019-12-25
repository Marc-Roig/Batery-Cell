#ifndef TESTS_H
#define TESTS_H

class WebSocketsClientMock {

private:
    String server_id;
    int server_port;
    String server_route;
    void (* websocketEvent)(WStype_t, uint8_t*, size_t);
public:
    WebSocketsClientMock() = default;
    void begin(String _server_id, int _server_port, String _server_route);
    void onEvent(void (*)(WStype_t, uint8_t*, size_t)); // Make first call as WStype_CONNECTED
    void setReconnectInterval(int);
    void sendTXT(String msg);
    void sendTXT(char* msg);
    void sendTXT(const char* msg);
    void loop();
};

class WiFiMultiMock {

private:
public:
    WiFiMultiMock() = default;
    void addAP(const char*, const char*){}
    int8_t run() {return WL_CONNECTED;}
};
#endif