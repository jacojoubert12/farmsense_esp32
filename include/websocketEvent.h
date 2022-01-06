#include <WebSocketsClient.h>
#include <WebSocketsServer.h>

void webSocketClientEvent(WStype_t type, uint8_t* payload, size_t length);
void webSocketServerEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
void startWebsocketServer(WebSocketsServer *ws_server);
void connectWebsocketClient(WebSocketsClient *ws_client, bool *connected);