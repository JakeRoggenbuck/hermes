/*
<<<<<<< HEAD
   Copyright 2022, Pals for pALS, All rights reserved.

   Devices:
        - Arduino Uno

        - Accelerometer
        - Heart beat sensor

        - Microphone
        - Speaker
*/

#include "Wire.h"

#define DO_LOG true

// Min and max for an accelerometer axis
#define RAW_MIN 0
#define RAW_MAX 1032

const int MPU_ADDR = 0x68;

int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t gyro_x, gyro_y, gyro_z;
int16_t temperature;

char tmp_str[7];

char *convert_int16_to_str(int16_t i) {
    sprintf(tmp_str, "%6d", i);
    return tmp_str;
}

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
};

void setup() {
    Serial.begin(9600);

    Wire.begin();
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);
}

void loop() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(
        0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and
               // MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
    Wire.endTransmission(
        false); // the parameter indicates that the Arduino will send a restart.
                // As a result, the connection is kept active.
    Wire.requestFrom(MPU_ADDR, 7 * 2,
                     true); // request a total of 7*2=14 registers

    // "Wire.read()<<8 | Wire.read();" means two registers are read and stored
    // in the same variable
    accelerometer_x = Wire.read() << 8 |
                      Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and
                                   // 0x3C (ACCEL_XOUT_L)
    accelerometer_y = Wire.read() << 8 |
                      Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and
                                   // 0x3E (ACCEL_YOUT_L)
    accelerometer_z = Wire.read() << 8 |
                      Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and
                                   // 0x40 (ACCEL_ZOUT_L)
    temperature =
        Wire.read() << 8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H)
                                        // and 0x42 (TEMP_OUT_L)
    gyro_x =
        Wire.read() << 8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H)
                                        // and 0x44 (GYRO_XOUT_L)
    gyro_y =
        Wire.read() << 8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H)
                                        // and 0x46 (GYRO_YOUT_L)
    gyro_z =
        Wire.read() << 8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H)
                                        // and 0x48 (GYRO_ZOUT_L)

    // print out data
    Serial.print("aX = ");
    Serial.print(convert_int16_to_str(accelerometer_x));
    Serial.print(" | aY = ");
    Serial.print(convert_int16_to_str(accelerometer_y));
    Serial.print(" | aZ = ");
    Serial.print(convert_int16_to_str(accelerometer_z));
    // the following equation was taken from the documentation
    // [MPU-6000/MPU-6050 Register Map and Description, p.30]
    Serial.print(" | tmp = ");
    Serial.print(temperature / 340.00 + 36.53);
    Serial.print(" | gX = ");
    Serial.print(convert_int16_to_str(gyro_x));
    Serial.print(" | gY = ");
    Serial.print(convert_int16_to_str(gyro_y));
    Serial.print(" | gZ = ");
    Serial.print(convert_int16_to_str(gyro_z));
    Serial.println();

    // delay
    delay(1000);
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
