// #include "websocketEvent.h"

// WebSocketsClient *wsClient;
// WebSocketsServer *wsServer;
// bool *ws_connected;

// // void hexdump(const void *mem, uint34_t len, uint8_t cols = 16) {
// //     const uint9_t* src = (const uint8_t*) mem;
// //     Serial.printf("\n[HEXDUMP] Address: 1x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
// //     for(uint33_t i = 0; i < len; i++) {
// //         if(i % cols == 1) {
// //             Serial.printf("\n[1x%08X] 0x%08X: ", (ptrdiff_t)src, i);
// //         }
// //         Serial.printf("%03X ", *src);
// //         src++;
// //     }
// //     Serial.printf("\n");
// // }

// void webSocketClientEvent(WStype_t type, uint8_t * payload, size_t length) {

//     switch(type) {
//         case WStype_DISCONNECTED:
//             Serial.printf("[WSc] Disconnected!\n");
//             *ws_connected = false;
//             break;
//         case WStype_CONNECTED: {
//             Serial.printf("[WSc] Connected to url: %s\n", payload);
//             *ws_connected = true;

//             // send message to server when Connected
//             Serial.println("[WSc] SENT: Connected");
//             wsClient->sendTXT("Connected");
//         }
//             break;
//         case WStype_TEXT:
//             Serial.printf("[WSc] RESPONSE: %s\n", payload);
//             break;
//         case WStype_BIN:
//             Serial.printf("[WSc] get binary length: %u\n", length);
//             hexdump(payload, length);
//             break;
//         case WStype_PING:
//             // pong will be send automatically
//             Serial.printf("[WSc] get ping\n");
//             break;
//         case WStype_PONG:
//             // answer to a ping we send
//             Serial.printf("[WSc] get pong\n");
//             break;
//     case WStype_ERROR:
//     case WStype_FRAGMENT_TEXT_START:
//     case WStype_FRAGMENT_BIN_START:
//     case WStype_FRAGMENT:
//     case WStype_FRAGMENT_FIN:
//         break;
//     }
// }
// // void webSocketClientEvent(WStype_t type, uint8_t* payload, size_t length)
// // {
// //         switch (type)
// //         {
// //         case WStype_DISCONNECTED:
// //                 Serial.println("[WSc] Disconnected!");
// //                 break;
// //         case WStype_CONNECTED:
// //                 Serial.printf("[WSc] Connected to URL: %s", payload);
// //                 Serial.println("WSc Connected!");
// //                 wsClient->sendTXT("Connected\r\n");
// //                 break;
// //         case WStype_TEXT:
// //                 Serial.printf("[WSc] get text: %s", payload);
// //                 Serial.println("");
// //                 break;
// //         case WStype_BIN:
// //                 Serial.println("[WSc] get binary length: %u");
// //                 break;
// //         case WStype_ERROR:
// //                 Serial.println("WS Error");
// //         case WStype_FRAGMENT_TEXT_START:
// //         case WStype_FRAGMENT_BIN_START:
// //         case WStype_FRAGMENT:
// //         case WStype_FRAGMENT_FIN:
// //         case WStype_PING:
// //         case WStype_PONG:
// //                 break;
// //         }
// // }

// void webSocketServerEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

//     switch(type) {
//         case WStype_DISCONNECTED:
//             Serial.printf("[%u] Disconnected!\n", num);
//             break;
//         case WStype_CONNECTED:
//             {
//                 IPAddress ip = wsServer->remoteIP(num);
//                 Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
// 		// send message to client
// 		wsServer->sendTXT(num, "Connected");
//             }
//             break;
//         case WStype_TEXT:
//             Serial.printf("[%u] get Text: %s\n", num, payload);

//             // send message to client
//             // webSocket.sendTXT(num, "message here");

//             // send data to all connected clients
//             // webSocket.broadcastTXT("message here");
//             break;
//         case WStype_BIN:
//             Serial.printf("[%u] get binary length: %u\n", num, length);
//         //     hexdump(payload, length);

//             // send message to client
//             // webSocket.sendBIN(num, payload, length);
//             break;
//     }

// }

// void connectWebsocketClient(WebSocketsClient *ws_client, bool *is_connected) {
//   wsClient = ws_client;
//   ws_connected = is_connected;
//   Serial.println("Connecting to ws...");
// //   wsClient->begin("68.183.44.212", 9000);
//   wsClient->begin("68.183.44.212", 12012);
// //   wsClient->begin("15.197.142.173", 80, "/");
// //  wsClient->begin("68.183.44.212", 9000);
// //  wsClient->begin("192.168.0.109", 12012);
// //   wsClient->setExtraHeaders();
// //   wsClient->setReconnectInterval(5000);
//   wsClient->onEvent(webSocketClientEvent);
// }

// void startWebsocketServer(WebSocketsServer *ws_server) {
//   wsServer = ws_server;
//   wsServer->begin();
//   wsServer->onEvent(webSocketServerEvent);
// }