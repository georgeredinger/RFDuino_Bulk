/*
 
/*
This sketch demonstrates how to do error free bulk data
transfer over Bluetooth Low Energy 4.

The data rate should be approximately:
  - 32 kbit/sec at 1.5ft (4000 bytes per second)
  - 24 kbit/sec at 40ft (3000 bytes per second)
  
This sketch sends 20 20 byte packets to

*/

#include <RFduinoBLE.h>

int packets = 20; 

// flag used to start sending
int flag = false;

// variables used in packet generation
int ch;
int packet;

int start;

void setup() {
  RFduinoBLE.begin();
}

void RFduinoBLE_onConnect() {
  packet = 0;
  ch = 'A';
  start = 0;
  flag = true;
}
void loop() {
  if (flag)
  {
    // generate the next packet
    char buf[21];
    sprintf(buf,"%04dEFGHIJKLMNOPQRST",packet);
        
    // send is queued (the ble stack delays send to the start of the next tx window)
    while (! RFduinoBLE.send(buf, 20))
      ;  // all tx buffers in use (can't send - try again later)

    if (! start)
      start = millis();

    packet++;
    if (packet >= packets)
    {
      int end = millis();
      float secs = (end - start) / 1000.0;
      int bps = ((packets * 20) * 8) / secs; 
        while(RFduinoBLE.radioActive) ;
        flag = false;
    }
  }
}

