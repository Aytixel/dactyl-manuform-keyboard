#include <EEPROM.h>

bool ask_bool(String str) {
  while (1) {
    Serial.print(str);
    Serial.print(" (true/false): ");

    while (!Serial.available());

    String value = Serial.readStringUntil('\n');

    Serial.println(value);

    if (value == String("true")) return true;
    if (value == String("false")) return false;
  }
}

void clear_from_eeprom(unsigned int *addr) {
  Serial.println("\nClearing unused EEPROM bytes ...");
  for (unsigned int i = *addr; i < EEPROM.length(); i++) 
    EEPROM.update(i, 0);
  Serial.println("EEPROM cleared!");
}

void put_eeprom(unsigned int *addr, void *value, unsigned int value_size) {
  for (unsigned int i = 0; i < value_size; i++) {
    EEPROM.update(*addr, *(byte *)(value + i));
    *addr += 1;
  }
}

void setup() {
  Serial.begin(115200);
  
  while(!Serial);
  
  Serial.println("EEPROM size : " + String(EEPROM.length()));
  
  unsigned int addr = 0;
  bool is_left = ask_bool("Is the left side ?");

  Serial.println("\nEEPROM writting ...");

  put_eeprom(&addr, &is_left, sizeof(is_left));

  Serial.println("EEPROM usage : "  + String(addr) + '/' + String(EEPROM.length()));

  clear_from_eeprom(&addr);
}

void loop() {}
