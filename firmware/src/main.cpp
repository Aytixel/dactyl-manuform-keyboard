#include <Arduino.h>
#include <KeyboardLayout.h>
#include <Keyboard_fr_FR.h>
#include <Keyboard.h>
#include <EEPROM.h>
#include <Wire.h>

#define I2C_ADDR 0

#define LEFT 0
#define RIGHT 1

#define KEY_ALT KEY_LEFT_ALT
#define KEY_ALTGR KEY_RIGHT_ALT

#define ROW_LEN 6
#define COL_LEN 7

#define MODE_KEY 0
#define MODE_GET_KEY_LAYOUT 1
#define MODE_SET_KEY 2
#define MODE_GET_FN_KEY 3
#define MODE_SET_FN_KEY 4

// variables
const byte ROW_PIN[ROW_LEN] = {4, 5, 6, 7, 8, 9};
const byte COL_PIN[COL_LEN] = {A2, A1, A0, 15, 14, 16, 10};

char key_layout[2][2][ROW_LEN][COL_LEN] = {0};
byte fn_keyboard_half = 0;
byte fn_row = 0;
byte fn_col = 0;

bool last_switch_state[2][2][ROW_LEN][COL_LEN] = {false};
bool switch_state[2][ROW_LEN][COL_LEN] = {false};
bool fn_state = false;
bool last_fn_state = false;

bool check_for_master = true;
bool is_master = false;
byte mode = MODE_KEY;

// functions
void packSwitchState(const bool switch_state[ROW_LEN][COL_LEN])
{
    for (byte row = 0; row < ROW_LEN; row++)
    {
        byte buf = 0;

        for (byte col = 0; col < COL_LEN; col++)
            buf += switch_state[row][col] << col;

        Wire.write(buf);
    }
}

void unpackSwitchState(bool switch_state[ROW_LEN][COL_LEN])
{
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(MODE_KEY);
    Wire.endTransmission();
    Wire.requestFrom(I2C_ADDR, 6);

    for (byte row = 0; row < ROW_LEN; row++)
    {
        byte buf = Wire.read();

        for (byte col = 0; col < COL_LEN; col++)
        {
            switch_state[row][col] = buf & 1;
            buf >>= 1;
        }
    }
}

void request()
{
    // if there is a request the master has been decided so stop checking for the master
    check_for_master = false;

    switch (mode)
    {
    case MODE_KEY:
        packSwitchState(switch_state[KEYBOARD_HALF]);
        break;
    }
}

void setKey(const byte buf[2])
{
    const byte fn = (buf[0] & 0b10000000) >> 7;
    const byte side = (buf[0] & 0b01000000) >> 6;
    const byte row = (buf[0] & 0b00111000) >> 3;
    const byte col = buf[0] & 0b00000111;

    key_layout[fn][side][row][col] = buf[1];
    EEPROM.update(col + (COL_LEN * row) + (COL_LEN * ROW_LEN * side) + (COL_LEN * ROW_LEN * 2 * fn), buf[1]);
}

void setFnKey(const byte buf)
{
    fn_keyboard_half = (buf & 0b01000000) >> 6;
    fn_row = (buf & 0b00111000) >> 3;
    fn_col = buf & 0b00000111;

    EEPROM.update(168, buf);
}

void receive(int length)
{
    mode = Wire.read();

    if (mode == MODE_SET_KEY && length == 3)
    {
        const byte buf[2] = {(unsigned char)Wire.read(), (unsigned char)Wire.read()};

        setKey(buf);
    }

    if (mode == MODE_SET_FN_KEY && length == 2)
    {
        setFnKey(Wire.read());
    }
}

void checkSwitchState(bool switch_state[ROW_LEN][COL_LEN])
{
    for (byte row = 0; row < ROW_LEN; row++)
    {
        digitalWrite(ROW_PIN[row], LOW);

        for (byte col = 0; col < COL_LEN; col++)
            switch_state[row][col] = !digitalRead(COL_PIN[col]);

        digitalWrite(ROW_PIN[row], HIGH);
    }
}

// program
void setup()
{
    // the two sides starts as slaves
    Wire.begin(I2C_ADDR);
    Wire.onRequest(request);
    Wire.onReceive(receive);

    // initialize switch pin mode
    for (byte i = 0; i < ROW_LEN; i++)
    {
        pinMode(ROW_PIN[i], OUTPUT);
        digitalWrite(ROW_PIN[i], HIGH);
    }

    for (byte i = 0; i < COL_LEN; i++)
        pinMode(COL_PIN[i], INPUT_PULLUP);
}

void emulate(const bool switch_state[ROW_LEN][COL_LEN], bool last_switch_state[ROW_LEN][COL_LEN], const char key_layout[ROW_LEN][COL_LEN])
{
    for (byte row = 0; row < ROW_LEN; row++)
    {
        for (byte col = 0; col < COL_LEN; col++)
        {
            if (switch_state[row][col] != last_switch_state[row][col] && key_layout[row][col])
            {
                if (switch_state[row][col])
                    Keyboard.press(key_layout[row][col]);
                else
                    Keyboard.release(key_layout[row][col]);

                if (last_fn_state != fn_state)
                {
                    Keyboard.releaseAll();

                    memset(last_switch_state[last_fn_state], 0, sizeof(last_switch_state[last_fn_state]));

                    last_fn_state = fn_state;
                }
            }
        }
    }
}

void loop()
{
    // check for a usb connection if true become the master
    if (check_for_master && (UDADDR & _BV(ADDEN)))
    {
        is_master = true;
        check_for_master = false;

        // initialize key layout from EEPROM
        EEPROM.get(0, key_layout);
        fn_keyboard_half = (EEPROM[168] & 0b01000000) >> 6;
        fn_row = (EEPROM[168] & 0b00111000) >> 3;
        fn_col = EEPROM[168] & 0b00000111;

        // enable serial communication
        Serial.begin(115200);

        // switch I2C connection from slave to master
        Wire.end();
        Wire.begin();

        // enable keyboard output
        Keyboard.begin(KeyboardLayout_fr_FR);
        Keyboard.releaseAll();
    }

    checkSwitchState(switch_state[KEYBOARD_HALF]);

    if (is_master)
    {
        unpackSwitchState(switch_state[!KEYBOARD_HALF]);

        fn_state = switch_state[1][4][6];

        emulate(switch_state[LEFT], last_switch_state[fn_state][LEFT], key_layout[fn_state][LEFT]);
        emulate(switch_state[RIGHT], last_switch_state[fn_state][RIGHT], key_layout[fn_state][RIGHT]);

        memcpy(last_switch_state[fn_state], switch_state, sizeof(switch_state));

        if (Serial.available())
        {
            switch (Serial.read())
            {
            case MODE_GET_KEY_LAYOUT:
                Serial.write((char *)key_layout, sizeof(key_layout));
                break;
            case MODE_SET_KEY:
            {
                const byte buf[2] = {(byte)Serial.read(), (byte)Serial.read()};

                setKey(buf);

                Wire.beginTransmission(I2C_ADDR);
                Wire.write(MODE_SET_KEY);
                Wire.write(buf, sizeof(buf));
                Wire.endTransmission();
                break;
            }
            case MODE_GET_FN_KEY:
                Serial.write(EEPROM[168]);
                break;
            case MODE_SET_FN_KEY:
            {
                const byte buf = Serial.read();

                setFnKey(buf);

                Wire.beginTransmission(I2C_ADDR);
                Wire.write(MODE_SET_FN_KEY);
                Wire.write(buf);
                Wire.endTransmission();
                break;
            }
            }
        }
    }
}
