#include "ble_module.h"
#include <bluefruit.h>

BLEDfu   bledfu;   // OTA DFU service
BLEDis   bledis;   // Device information service
BLEUart  bleuart;  // UART over BLE
BLEBas   blebas;   // Battery service

// Buffer and index for receiving time update characters
static uint8_t myTime[6];
static int serialindex = 0;

static void startAdv(void) {
  // Setup advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.ScanResponse.addName();

  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244); // in units of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);
  Bluefruit.Advertising.start(0); // Advertise indefinitely
}

static void connect_callback(uint16_t conn_handle) {
  BLEConnection *connection = Bluefruit.Connection(conn_handle);
  char central_name[32] = {0};
  connection->getPeerName(central_name, sizeof(central_name));
  Serial.print("Connected to ");
  Serial.println(central_name);
}

static void disconnect_callback(uint16_t conn_handle, uint8_t reason) {
  (void)conn_handle;
  (void)reason;
  Serial.println();
  Serial.print("Disconnected, reason = 0x");
  Serial.println(reason, HEX);
}

void initBLE() {
  Serial.println("Bluefruit52 BLEUART Example");
  Serial.println("---------------------------");

  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // Begin BLE services (DFU must be added first if used)
  bledfu.begin();
  
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  bleuart.begin();

  blebas.begin();
  blebas.write(100);

  // Start advertising
  startAdv();

  Serial.println("Use Adafruit's Bluefruit LE app to connect in UART mode.");
  Serial.println("Once connected, send characters to update the time.");
}

void updateBLE() {
  uint8_t ch;
  // Process BLE UART data
  while (bleuart.available()) {
    ch = (uint8_t)bleuart.read();
    Serial.write(ch);
    if (serialindex < 6) {
      // Convert character to number and store
      myTime[serialindex] = ch - '0';
      serialindex++;
    } else {
      uint8_t myHour   = myTime[0] * 10 + myTime[1];
      uint8_t myMinute = myTime[2] * 10 + myTime[3];
      uint8_t mySecond = myTime[4] * 10 + myTime[5];
      setTime(myHour, myMinute, mySecond, 1, 1, 2024);
      for (int i = 0; i < 6; i++) {
        Serial.print(myTime[i]);
      }
      serialindex = 0;
    }
  }
}
