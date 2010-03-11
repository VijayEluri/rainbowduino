/* Com:
 * Abstraction of Serial / Wire commmunication
 * message :
 * 1. open BYTE 255
 * 2. adress BYTE 
 * 3. msg length BYTE
 * 4. data
 *      COMMAND
 *      DATA
 */
 
#ifndef Com_h
#define Com_h

#define BAUD_RATE 57600

#define I2C_START_ADR 64
#define I2C_MASTER_ADR 7
#define I2C_SLAVE_ADR 10

#define I2C_CMD_HELLO 23
#define I2C_CMD_OK 42

#define I2C_WAIT_FOR_ADDRESS_TIMEOUT 500 //ms
#define I2C_WAIT_FOR_ADDRESS_RETRYS 3 //number of retrys to get an slave adress

#define I2C_EEPROM_ADR 0 //where the gets written to

#define STARTUP_DELAY_FACTOR 100 //in ms

#define MESSAGE_BUFFER_SIZE 32 //max 128
#define MESSAGE_START 255
#define MESSAGE_OK 1

#include <Wire.h>
#include "WProgram.h"
//DEBUG #include "Rainbowduino.h"

 extern "C" {
 // callback function
     typedef void (*conCallbackFunction)(void);
 }

class Connection {

public:
  bool master;
  volatile uint8_t i2c_address;
  volatile uint8_t last_slave_address;
  volatile uint8_t slave_address_to_register;
  
  void loop();
  void begin();
  void beginMaster(uint8_t master_address = I2C_MASTER_ADR, bool update_adress = true);
  void beginSlave(uint8_t slave_address, bool update_adress = true);
  void onMessageAvailable(conCallbackFunction newFunction);
  int read();
  uint8_t process(uint8_t serialByte);
  uint8_t processMessage(uint8_t serialByte);
  uint8_t available();
  
  void registerPendingSlave();
  
  void ok(uint8_t command, uint8_t param);
  
  //TODO
  void write(uint8_t data);
  
  uint8_t inputBufferLength; //number of bytes to receive

private: 
  bool messageAwaiting; //status wheter Message parsing is in progress
  
  conCallbackFunction callback;
  
  uint8_t buffer1[MESSAGE_BUFFER_SIZE]; // Buffer that holds the data
  byte* inputBuffer; // Pointer to current data

  uint8_t inputBufferIndex; // Index where to write the data
  
  uint8_t buffer2[MESSAGE_BUFFER_SIZE]; // Buffer that holds the data
  byte* outputBuffer;
  uint8_t outputBufferLength;
  uint8_t outputBufferIndex; // Index where to write the data  
};

extern Connection Con;
#endif