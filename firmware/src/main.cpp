#include <Arduino.h>
#include <KeyboardLayout.h>
#include <Keyboard_fr_FR.h>
#include <Keyboard.h>
#include <EEPROM.h>
#include <Wire.h>

#define I2C_ADDR 0

#define LEFT 0
#define RIGHT 1

#define LAYER_LEN 2
#define ROW_LEN 6
#define COL_LEN 7

#define METHOD_GET_KEY_STATE 0
#define METHOD_GET_KEY_LAYOUT 1
#define METHOD_SET_KEY 2

// variables
const byte ROW_PIN[ROW_LEN] = {4, 5, 6, 7, 8, 9};
const byte COL_PIN[COL_LEN] = {A2, A1, A0, 15, 14, 16, 10};

char key_layout[LAYER_LEN][2][ROW_LEN][COL_LEN] = {0};

bool last_switch_state[LAYER_LEN][2][ROW_LEN][COL_LEN] = {false};
bool switch_state[2][ROW_LEN][COL_LEN] = {false};
bool fn_state = false;
bool last_fn_state = false;

bool check_for_master = true;
bool is_master = false;
byte method = METHOD_GET_KEY_STATE;

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
    Wire.write(METHOD_GET_KEY_STATE);
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

    switch (method)
    {
    case METHOD_GET_KEY_STATE:
        packSwitchState(switch_state[KEYBOARD_HALF]);
        break;
    }
}

void setKey(const byte buf[2])
{
    const byte layer = (buf[0] & 0b10000000) >> 7;
    const byte side = (buf[0] & 0b01000000) >> 6;
    const byte row = (buf[0] & 0b00111000) >> 3;
    const byte col = buf[0] & 0b00000111;

    key_layout[layer][side][row][col] = buf[1];
    EEPROM.update(col + (COL_LEN * row) + (COL_LEN * ROW_LEN * (side + 2 * layer)), buf[1]);
}

void receive(int length)
{
    method = Wire.read();

    if (method == METHOD_SET_KEY && length == 3)
    {
        const byte buf[2] = {(byte)Wire.read(), (byte)Wire.read()};

        setKey(buf);
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

    // initialize switch pin method
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
            case METHOD_GET_KEY_LAYOUT:
                Serial.write((char *)key_layout, sizeof(key_layout));
                break;
            case METHOD_SET_KEY:
            {
                const byte buf[2] = {(byte)Serial.read(), (byte)Serial.read()};

                setKey(buf);

                Wire.beginTransmission(I2C_ADDR);
                Wire.write(METHOD_SET_KEY);
                Wire.write(buf, sizeof(buf));
                Wire.endTransmission();
                break;
            }
            }
        }
    }
}
