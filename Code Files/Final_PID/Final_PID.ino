#include <Wire.h>
#include <VL53L0X.h>
#include <MPU6050_light.h>

#define TOF1_PIN 39
#define TOF2_PIN 40
#define TOF3_PIN 41

VL53L0X sensor1;
VL53L0X sensor2;
VL53L0X sensor3;

int a;
int b;
int c;

MPU6050 mpu(Wire);

unsigned long timer = 0;


void setup()
{
  Serial.begin(9600);
  Serial.println("jdsbaj");

    pinMode(TOF1_PIN, OUTPUT);
    pinMode(TOF2_PIN, OUTPUT);
    pinMode(TOF3_PIN, OUTPUT);

    digitalWrite(TOF1_PIN, LOW);
    digitalWrite(TOF2_PIN, LOW);
    digitalWrite(TOF3_PIN, LOW);
    delay(500);
    Serial.println("jdsbaj");

    Wire.begin();
    digitalWrite(TOF1_PIN, HIGH);
    delay(150);
    sensor1.init(true);
    delay(100);
    sensor1.setAddress((uint8_t)01);

    digitalWrite(TOF2_PIN, HIGH);
    delay(150);
    sensor2.init(true);
    delay(100);
    sensor2.setAddress((uint8_t)02);
    Serial.println("jdsbaj");

    digitalWrite(TOF3_PIN, HIGH);
    delay(150);
    sensor3.init(true);
    delay(100);
    sensor3.setAddress((uint8_t)03);

    Serial.println("addresses set");
    
    sensor1.startContinuous();
    sensor2.startContinuous();
    sensor3.startContinuous();
    Serial.println("jdsbaj");

    // Initialize MPU6050 sensor
    byte status = mpu.begin();
    Serial.print(F("MPU6050 status: "));
    Serial.println(status);
    while (status != 0) {
      Serial.println("Could not connect to MPU6050");
      delay(1000); // Keep trying to connect
      status = mpu.begin();
    }

    Serial.println(F("Calculating offsets, do not move MPU6050"));
    delay(1000);
    mpu.calcOffsets(true, true); // gyro and accelero
    Serial.println("Done!\n");

}

void loop()
{
    a = sensor1.readRangeContinuousMillimeters();
    b = sensor2.readRangeContinuousMillimeters();
    c = sensor3.readRangeContinuousMillimeters();

    // Print sensor readings
    Serial.print("Sensor 1: ");
    Serial.print(a);
    Serial.print(" mm\t");

    Serial.print("Sensor 2: ");
    Serial.print(b);
    Serial.print(" mm\t");

    Serial.print("Sensor 3: ");
    Serial.print(c);
    Serial.print(" mm\n");

     // Read MPU6050 sensor
    mpu.update();
    if ((millis() - timer) > 10) { // print data every 10ms
      Serial.print("X: ");
      Serial.print(mpu.getAngleX());
      Serial.print("\tY: ");
      Serial.print(mpu.getAngleY());
      Serial.print("\tZ: ");
      Serial.println(mpu.getAngleZ());
      timer = millis();
      delay(400);
    }

}