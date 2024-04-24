#include <KeyboardLayout.h>
#include <Keyboard_fr_FR.h>
#include <Keyboard.h>
#include <EEPROM.h>
#include <Wire.h>

#define I2C_ADDR 0

#define LEFT true
#define RIGHT false

#define FN_ROW 4
#define FN_COL 6
#define FN_KEYBOARD_HALF RIGHT

#define KEY_ALT KEY_LEFT_ALT
#define KEY_ALTGR KEY_RIGHT_ALT

#define ROW_LEN 6
#define COL_LEN 7

// variables
const byte ROW_PIN[ROW_LEN] = {4, 5, 6, 7, 8, 9};
const byte COL_PIN[COL_LEN] = {A2, A1, A0, 15, 14, 16, 10};

const char key_layout[2][2][ROW_LEN][COL_LEN] = {
  { // normal keys
    { // right
      {KEY_E_GRAVE , '_' , KEY_C_CEDILLA , KEY_A_GRAVE , ')' , '=' , KEY_INSERT},
      {'u' , 'i' , 'o' , 'p' , KEY_CIRCUMFLEX , '$' , '<' },
      {'j' , 'k' , 'l' , 'm' , KEY_U_GRAVE , '*' , '{' },
      {'\0', 'n' , ',' , ';' , ':' , '!' , '}' },
      {'\0', KEY_BACKSPACE, KEY_DELETE, KEY_HOME, KEY_END, '`' , '\0'},
      {'\0', KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_LEFT_ARROW, KEY_DOWN_ARROW, '\0', '\0'},
    },
    { // left
      {KEY_ESC, '&' , KEY_E_ACUTE, '"' , '\'', '(' , '-' },
      {'\t', 'a' , 'z' , 'e' , 'r' , 't' , 'y' },
      {KEY_CAPS_LOCK, 'q' , 's' , 'd' , 'f' , 'g' , 'h' },
      {'[' , 'w' , 'x' , 'c' , 'v' , 'b' , '\0'},
      {']' , KEY_LEFT_GUI, KEY_PAGE_DOWN, KEY_PAGE_UP, '\n', ' ' , '\0'},
      {'\0', '\0', KEY_ALT, KEY_ALTGR, KEY_LEFT_CTRL, KEY_LEFT_SHIFT, '\0'},
    }
  },
  { // function keys
    { // right
      {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
      {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
      {'\0', KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12},
      {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
      {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
      {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
    },
    { // left
      {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
      {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
      {KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, '\0'},
      {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
      {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
      {'\0', '\0', '\0', '\0', '\0', '\0', '\0'},
    }
  }
}; 

bool last_switch_state[2][2][ROW_LEN][COL_LEN] = {false};
bool switch_state[2][ROW_LEN][COL_LEN] = {false};
bool fn_state = false;
bool last_fn_state = false;

bool keyboard_half = EEPROM[0];
bool check_for_master = true;
bool is_master = false;

// functions
void packSwitchState(bool switch_state[ROW_LEN][COL_LEN]) {
  for (byte row = 0; row < ROW_LEN; row++) {
    byte buf = 0;
    
    for (byte col = 0; col < COL_LEN; col++)
        buf += switch_state[row][col] << col;
    
    Wire.write(buf);
  }
}

void unpackSwitchState(bool switch_state[ROW_LEN][COL_LEN]) {
  for (byte row = 0; row < ROW_LEN; row++) {
      byte buf = Wire.read();

      for (byte col = 0; col < COL_LEN; col++) {
        switch_state[row][col] = buf & 1;
        buf >>= 1;
      }
  }
}

void request() {
  // if there is a request the master has been decided so stop checking for the master
  check_for_master = false;
    
  packSwitchState(switch_state[keyboard_half]);
}

void checkSwitchState(bool switch_state[ROW_LEN][COL_LEN]) {
  for (byte row = 0; row < ROW_LEN; row++) {
    digitalWrite(ROW_PIN[row], LOW);
      
    for (byte col = 0; col < COL_LEN; col++) 
      switch_state[row][col] = !digitalRead(COL_PIN[col]);
    
    digitalWrite(ROW_PIN[row], HIGH);
  }
}

// program
void setup() {
  // the two sides starts as slaves
  Wire.begin(I2C_ADDR);
  Wire.onRequest(request);

  // initialize switch pin mode
  for (byte i = 0; i < ROW_LEN; i++) {
    pinMode(ROW_PIN[i], OUTPUT);
    digitalWrite(ROW_PIN[i], HIGH);
  }
    
  for (byte i = 0; i < COL_LEN; i++)
    pinMode(COL_PIN[i], INPUT_PULLUP);
}

void emulate(bool switch_state[ROW_LEN][COL_LEN], bool last_switch_state[ROW_LEN][COL_LEN], char key_layout[ROW_LEN][COL_LEN]) {
  for (byte row = 0; row < ROW_LEN; row++) {
    for (byte col = 0; col < COL_LEN; col++) {
      if (switch_state[row][col] != last_switch_state[row][col] && key_layout[row][col]) {
        if (switch_state[row][col])
          Keyboard.press(key_layout[row][col]);
        else
          Keyboard.release(key_layout[row][col]);

        if (last_fn_state != fn_state) {
          Keyboard.releaseAll();
          
          memset(last_switch_state[last_fn_state], 0, sizeof(last_switch_state[last_fn_state]));
      
          last_fn_state = fn_state;
        }
      }
    }
  }
}

void loop() {
  // check for a usb connection if true become the master
  if (check_for_master && (UDADDR & _BV(ADDEN))) {
    is_master = true;
    check_for_master = false;
    
    Wire.end();
    Wire.begin();
    
    Keyboard.begin(KeyboardLayout_fr_FR);
    Keyboard.releaseAll();
  }
  
  checkSwitchState(switch_state[keyboard_half]);

  if (is_master) {
    Wire.requestFrom(I2C_ADDR, 6);
    
    unpackSwitchState(switch_state[!keyboard_half]);

    fn_state = switch_state[FN_KEYBOARD_HALF][FN_ROW][FN_COL];
    
    emulate(switch_state[LEFT], last_switch_state[fn_state][LEFT], key_layout[fn_state][LEFT]);
    emulate(switch_state[RIGHT], last_switch_state[fn_state][RIGHT], key_layout[fn_state][RIGHT]);
    
    memcpy(last_switch_state[fn_state], switch_state, sizeof(switch_state));
  }
}
