#include <Arduino.h>
#include <HID-Project.h>
#include <EEPROM.h>
#include <Wire.h>

#define I2C_ADDR 0

#define LEFT 0
#define RIGHT 1

#define LAYER_COUNT 6
#define SIDE_COUNT 2
#define ROW_LEN 6
#define COL_LEN 7

#define SIDE_LEN ROW_LEN * COL_LEN
#define LAYER_LEN SIDE_LEN * SIDE_COUNT
#define ALL_LAYER_LEN LAYER_LEN * LAYER_COUNT

#define METHOD_GET_KEY_STATE 0
#define METHOD_GET_KEY_LAYOUT 1
#define METHOD_SET_KEY 2

#define CODE_LAYER_PRESS 255
#define CODE_LAYER_RELEASE 254

// variables
const uint8_t ROW_PIN[ROW_LEN] = {4, 5, 6, 7, 8, 9};
const uint8_t COL_PIN[COL_LEN] = {A2, A1, A0, 15, 14, 16, 10};

uint16_t key_layout[LAYER_COUNT][SIDE_COUNT][ROW_LEN][COL_LEN] = {0};

bool last_switch_state[SIDE_COUNT][ROW_LEN][COL_LEN] = {false};
bool switch_state[SIDE_COUNT][ROW_LEN][COL_LEN] = {false};
uint8_t layer = 0;
uint8_t last_layer = 0;

bool check_for_master = true;
bool is_master = false;
uint8_t method = METHOD_GET_KEY_STATE;

// functions
void packSwitchState(const bool switch_state[ROW_LEN][COL_LEN])
{
    for (uint8_t row = 0; row < ROW_LEN; row++)
    {
        uint8_t buf = 0;

        for (uint8_t col = 0; col < COL_LEN; col++)
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

    for (uint8_t row = 0; row < ROW_LEN; row++)
    {
        uint8_t buf = Wire.read();

        for (uint8_t col = 0; col < COL_LEN; col++)
            switch_state[row][col] = (buf >> col) & 0b00000001;
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

void setKey(const uint8_t buf[4])
{
    const uint8_t layer = buf[0] & 0b00000111;
    const uint8_t side = (buf[1] & 0b01000000) >> 6;
    const uint8_t row = (buf[1] & 0b00111000) >> 3;
    const uint8_t col = buf[1] & 0b00000111;

    const uint16_t position = (
            (uint16_t)col
                + (COL_LEN * (uint16_t)row)
                + (SIDE_LEN * (uint16_t)side)
                + (LAYER_LEN * (uint16_t)layer)
        ) * sizeof(uint16_t);

    key_layout[layer][side][row][col] = ((uint16_t)buf[2] << 8) + buf[3];
    EEPROM.update(position + 1, buf[2]);
    EEPROM.update(position, buf[3]);
}

void receive(int length)
{
    method = Wire.read();

    if (method == METHOD_SET_KEY && length == 5)
    {
        const uint8_t buf[4] = {
            (uint8_t)Wire.read(),
            (uint8_t)Wire.read(),
            (uint8_t)Wire.read(),
            (uint8_t)Wire.read()
        };

        setKey(buf);
    }
}

void checkSwitchState(bool switch_state[ROW_LEN][COL_LEN])
{
    for (uint8_t row = 0; row < ROW_LEN; row++)
    {
        digitalWrite(ROW_PIN[row], LOW);

        for (uint8_t col = 0; col < COL_LEN; col++)
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
    for (uint8_t i = 0; i < ROW_LEN; i++)
    {
        pinMode(ROW_PIN[i], OUTPUT);
        digitalWrite(ROW_PIN[i], HIGH);
    }

    for (uint8_t i = 0; i < COL_LEN; i++)
        pinMode(COL_PIN[i], INPUT_PULLUP);
}

void press(uint16_t key) {
    // drop every key outside of usable usage id range
    if (key && (key & 0x00FF) <= 0xE7)
        NKROKeyboard.press(key);
}

void release(uint16_t key) {
    // drop every key outside of usable usage range
    if (key && (key & 0x00FF) <= 0xE7)
        NKROKeyboard.release(key);
}

bool emulate(
    const bool switch_state[SIDE_COUNT][ROW_LEN][COL_LEN],
    bool last_switch_state[SIDE_COUNT][ROW_LEN][COL_LEN],
    const uint16_t key_layout[SIDE_COUNT][ROW_LEN][COL_LEN]
)
{
    bool switch_state_changed = false;

    for (uint8_t side = 0; side < SIDE_COUNT; side++)
    {
        for (uint8_t row = 0; row < ROW_LEN; row++)
        {
            for (uint8_t col = 0; col < COL_LEN; col++)
            {
                if (
                    switch_state[side][row][col] == last_switch_state[side][row][col]
                )
                    continue;

                // process special keys
                uint8_t low_key = key_layout[side][row][col] & 0x00FF;
                uint8_t high_key = (key_layout[side][row][col] & 0xFF00) >> 8;

                if (
                    (low_key == CODE_LAYER_PRESS && switch_state[side][row][col])
                        || (low_key == CODE_LAYER_RELEASE && !switch_state[side][row][col])
                )
                {
                    layer = high_key;
                    continue;
                }

                // should ignore some special keys
                switch_state_changed = true;

                if (switch_state[side][row][col])
                    press(key_layout[side][row][col]);
                else
                    release(key_layout[side][row][col]);
            }
        }
    }

    return switch_state_changed;
}

// this function allows key combinations between layers
void switch_layer(
    const bool switch_state_changed,
    const bool switch_state[SIDE_COUNT][ROW_LEN][COL_LEN],
    const uint16_t key_layout[SIDE_COUNT][ROW_LEN][COL_LEN],
    const uint16_t last_key_layout[SIDE_COUNT][ROW_LEN][COL_LEN]
) {
    if (switch_state_changed && last_layer != layer)
    {
        for (uint8_t side = 0; side < SIDE_COUNT; side++)
        {
            for (uint8_t row = 0; row < ROW_LEN; row++)
            {
                for (uint8_t col = 0; col < COL_LEN; col++)
                {
                    if (switch_state[side][row][col])
                    {
                        if (key_layout[side][row][col] != last_key_layout[side][row][col])
                        {
                            press(key_layout[side][row][col]);
                            release(last_key_layout[side][row][col]);
                        }
                    }
                    else
                        release(last_key_layout[side][row][col]);
                }
            }
        }

        last_layer = layer;
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
        NKROKeyboard.begin();
        NKROKeyboard.releaseAll();
    }

    checkSwitchState(switch_state[KEYBOARD_HALF]);

    if (is_master)
    {
        unpackSwitchState(switch_state[!KEYBOARD_HALF]);

        bool switch_state_changed = emulate(
            switch_state,
            last_switch_state,
            key_layout[layer]
        );

        switch_layer(
            switch_state_changed,
            switch_state,
            key_layout[layer],
            key_layout[last_layer]
        );

        memcpy(last_switch_state, switch_state, sizeof(switch_state));

        if (Serial.available())
        {
            switch (Serial.read())
            {
            case METHOD_GET_KEY_LAYOUT:
                Serial.write((char *)key_layout, sizeof(key_layout));
                break;
            case METHOD_SET_KEY:
            {
                const uint8_t buf[4] = {
                    (uint8_t)Serial.read(),
                    (uint8_t)Serial.read(),
                    (uint8_t)Serial.read(),
                    (uint8_t)Serial.read()
                };

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
