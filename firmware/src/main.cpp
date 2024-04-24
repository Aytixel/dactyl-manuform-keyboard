#include <Arduino.h>
#include <KeyboardLayout.h>
#include <Keyboard_fr_FR.h>
#include <Keyboard.h>
#include <EEPROM.h>
#include <Wire.h>

#define I2C_ADDR 0

#define LEFT 0
#define RIGHT 1

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

char key_layout[2][2][ROW_LEN][COL_LEN] = {0};

bool switch_state[2][ROW_LEN][COL_LEN] = {false};

bool check_for_master = true;

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
    Wire.write(0);
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

    const byte command = Wire.read();

    switch (command)
    {
    case 0:
        packSwitchState(switch_state[KEYBOARD_HALF]);
        break;
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
    // initialize key layout from EEPROM
    EEPROM.get(0, key_layout);

    // the two sides starts as slaves
    Wire.begin(I2C_ADDR);
    Wire.onRequest(request);

    // initialize switch pin mode
    for (byte i = 0; i < ROW_LEN; i++)
    {
        pinMode(ROW_PIN[i], OUTPUT);
        digitalWrite(ROW_PIN[i], HIGH);
    }

    for (byte i = 0; i < COL_LEN; i++)
        pinMode(COL_PIN[i], INPUT_PULLUP);
}

void emulate(const bool fn_state, const bool switch_state[ROW_LEN][COL_LEN], bool last_switch_state[ROW_LEN][COL_LEN], const char key_layout[ROW_LEN][COL_LEN])
{
    static bool last_fn_state = false;

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
    static bool last_switch_state[2][2][ROW_LEN][COL_LEN] = {false};
    static bool fn_state = false;
    static bool is_master = false;

    // check for a usb connection if true become the master
    if (check_for_master && (UDADDR & _BV(ADDEN)))
    {
        is_master = true;
        check_for_master = false;

        Wire.end();
        Wire.begin();

        Keyboard.begin(KeyboardLayout_fr_FR);
        Keyboard.releaseAll();
    }

    checkSwitchState(switch_state[KEYBOARD_HALF]);

    if (is_master)
    {
        unpackSwitchState(switch_state[!KEYBOARD_HALF]);

        fn_state = switch_state[FN_KEYBOARD_HALF][FN_ROW][FN_COL];

        emulate(fn_state, switch_state[LEFT], last_switch_state[fn_state][LEFT], key_layout[fn_state][LEFT]);
        emulate(fn_state, switch_state[RIGHT], last_switch_state[fn_state][RIGHT], key_layout[fn_state][RIGHT]);

        memcpy(last_switch_state[fn_state], switch_state, sizeof(switch_state));
    }
}
