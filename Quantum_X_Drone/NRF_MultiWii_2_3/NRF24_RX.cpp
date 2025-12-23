#include "Arduino.h"
#include "config.h"
#include "def.h"
#include "types.h"
#include "MultiWii.h"
#include "NRF24_RX.h"

#if defined(NRF24_RX)

int16_t nrf24_rcData[RC_CHANS];

const uint64_t pipe = 0xE8E8F0F0E1LL; //Must be the same as Transmitter
RF24 radio(7, 5);// CE, CSN

RF24Data strData;

void resetRF24Data(){
  strData.throttle = 0;
  strData.yaw = 128;
  strData.pitch = 128;
  strData.roll = 128;
  strData.AUX1 = 0;
  strData.AUX2 = 0;
}

void NRF24_Init() {
  resetRF24Data();
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setAutoAck(false);
  radio.openReadingPipe(1,pipe);
  radio.startListening();  
}

void NRF24_Read_RC() {
  static unsigned long lastRecvTime = 0;
  unsigned long now = millis();
  if (radio.available()) {
    failsafeCnt = 0;
    radio.read(&strData, sizeof(RF24Data));
    lastRecvTime = now;
  }
  else{
    failsafeCnt++;     
  }
  if (now - lastRecvTime > 1000) {
    resetRF24Data();
  }
  nrf24_rcData[THROTTLE]  = map(strData.throttle, 0, 255, 1000, 2000);
  nrf24_rcData[ROLL]      = map(strData.roll,    0, 255, 1000, 2000);
  nrf24_rcData[PITCH]     = map(strData.pitch,     0, 255, 1000, 2000);
  nrf24_rcData[YAW]       = map(strData.yaw,      0, 255, 1000, 2000);
  nrf24_rcData[AUX1]      = map(strData.AUX1,     0, 1,   1000, 2000);
  nrf24_rcData[AUX2]      = map(strData.AUX2,     0, 1,   1000, 2000);  
  nrf24_rcData[AUX3]      = map(strData.AUX3,     0, 1,   1000, 2000);
  nrf24_rcData[AUX4]      = map(strData.AUX4,     0, 1,   1000, 2000);  
}


#endif
