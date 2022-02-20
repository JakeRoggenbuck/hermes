/*
 * Copyright 2022, Pals for pALS, All rights reserved.
 *
 * Devices:
 * 	- Arduino Uno
 *
 * 	- Accelerometer
 * 	- Heart beat sensor
 *
 * 	- Microphone
 * 	- Speaker
 */

#define DO_LOG true

// Min and max for an accelerometer axis
#define RAW_MIN 0
#define RAW_MAX 1032

// Take a sample to reduce noise in accelerometer reading
#define SAMPLE_SIZE 10

const int x_input = A0;
const int y_input = A1;
const int z_input = A2;

enum DeviceState {
    AWAITING,
    MIC_ON,
    USER_DANGER,
    ERROR,
};

class Device {
  public:
    DeviceState state;
}

void setup() {
#if DO_LOG
    Serial.begin(9600);
    Serial.println("Logging started.");
#endif
}

int read_axis(int axis_pin) {
    long reading = 0;

    analogRead(axis_pin);
    delay(1);

    for (int i = 0; i < SAMPLE_SIZE; i++) {
        reading += analogRead(axis_pin);
    }
    return reading / SAMPLE_SIZE;
}

void loop() {
    int raw_x = read_axis(x_input);
    int raw_y = read_axis(y_input);
    int raw_z = read_axis(z_input);

    // Maps raw values (e.g. raw_x) that exist in the range [RAW_MIN, RAW_MAX]
    // to the range [-3000, 3000]
    long scaled_x = map(raw_x, RAW_MIN, RAW_MAX, -3000, 3000);
    long scaled_y = map(raw_y, RAW_MIN, RAW_MAX, -3000, 3000);
    long scaled_z = map(raw_z, RAW_MIN, RAW_MAX, -3000, 3000);

    float axel_x = scaled_x / 1000.0;
    float axel_y = scaled_y / 1000.0;
    float axel_z = scaled_z / 1000.0;

    delay(200);
}
