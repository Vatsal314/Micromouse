/*
 * The purpose of this code is to count the ouput pulses or
 * the encoder outputs as you rotate the Motor shaft. You can run the
 * same code on the Arduino Uno, Arduino Nano, Arduino Mega, etc.
 */
#define M1_ENC_A 9// pin2 of the Arduino
#define M1_ENC_B 11// pin 3 of the Arduino
// these two pins has the hardware interrupts as well.
#define M2_ENC_A 12 // pin2 of the Arduino
#define M2_ENC_B 14

#define M1_PWM 37
#define M1_in2 34
#define M1_in1 35

#define M2_PWM 36
#define M2_in2 33
#define M2_in1 38

int m1_count_pulses = 0;
int m2_count_pulses = 0;

void setup()
{
  Serial.begin(9600);                // activates the serial communication
  pinMode(M1_ENC_A, INPUT); // sets the Encoder_output_A pin as the input
  pinMode(M1_ENC_B, INPUT); // sets the Encoder_output_B pin as the input
  attachInterrupt(digitalPinToInterrupt(M1_ENC_A), M1_Encoder_Interrupt, RISING);

  pinMode(M2_ENC_A, INPUT); // sets the Encoder_output_A pin as the input
  pinMode(M2_ENC_B, INPUT); // sets the Encoder_output_B pin as the input
  attachInterrupt(digitalPinToInterrupt(M2_ENC_A), M2_Encoder_Interrupt, RISING);

  pinMode(M1_PWM, OUTPUT);
  pinMode(M1_in1, OUTPUT);
  pinMode(M1_in2, OUTPUT);

  pinMode(M2_PWM, OUTPUT);
  pinMode(M2_in1, OUTPUT);
  pinMode(M2_in2, OUTPUT);
}

void loop()
{
  digitalWrite(M1_in1, HIGH);
  digitalWrite(M1_in2, LOW);

  digitalWrite(M2_in1, HIGH);
  digitalWrite(M2_in2, LOW);

  analogWrite(M1_PWM, 255);
  analogWrite(M2_PWM, 255);
  
  Serial.println("Result A: ");
  Serial.println(m1_count_pulses);
  Serial.print("Results B:");
  Serial.println(m2_count_pulses);
  delay(100);
}

void M1_Encoder_Interrupt()
{
  int b = digitalRead(M1_ENC_B);
  if (b > 0)
  {
    m1_count_pulses++;
  }
  else
  {
    m1_count_pulses--;
  }
}
void M2_Encoder_Interrupt()
{
  int c = digitalRead(M2_ENC_B);
  if (c > 0)
  {
    m2_count_pulses++;
  }
  else
  {
    m2_count_pulses--;
  }
}
