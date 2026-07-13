#define ENCA 9
#define ENCB 11
#define PWM 0
#define in2 1
#define in1 28

double start_time;
int pos = 0;
int i;

void setup() {
  Serial.begin(9600);
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  pinMode (PWM, OUTPUT);
  pinMode (in1, OUTPUT);
  pinMode (in2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
}

void loop() {
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      analogWrite (PWM, 255);
      delay(1000);

      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
      Serial.println(pos);
      delay(1000);
      Serial.println(pos);
}

void readEncoder(){
  int b = digitalRead(ENCB);
  if(b > 0){
    pos++;
  }
  else{
    pos--;
  }
  
}