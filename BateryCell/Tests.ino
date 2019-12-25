#include "main.h"

// -- WEBSOCKET MOCK -- //
void WebSocketsClientMock::begin(String _server_id, int _server_port, String _server_route) {
    server_id = _server_id;
    server_port = _server_port;
    server_route = _server_route;
}


void WebSocketsClientMock::onEvent(void (*_websocketEvent)(WStype_t, uint8_t *, size_t)) {
    websocketEvent = _websocketEvent;

    // Leave connection open
    websocketEvent(WStype_CONNECTED, nullptr, 0);
}


void WebSocketsClientMock::setReconnectInterval(int) {}


void WebSocketsClientMock::sendTXT(char *msg) {
    Serial.print("[WS Mock] Sending websocket message: ");
    Serial.println(msg);
}


void WebSocketsClientMock::sendTXT(const char *msg) {
    Serial.print("[WS Mock] Sending websocket message: ");
    Serial.println(msg);
}


void WebSocketsClientMock::sendTXT(String msg) {
    Serial.print("[WS Mock] Sending websocket message: ");
    Serial.println(msg);
}


void WebSocketsClientMock::loop() {

}

// -- WIFI MOCK -- //
