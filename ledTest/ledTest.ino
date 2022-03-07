//RS --> GPIO33
//E --> GPIO32
//D7 --> GPIO25
//D6 --> GPIO26
//D5 --> GPIO27
//D4 --> GPIO19

//White --> GPIO15
//Amber --> GPIO4
//Green--> GPIO16
//Blue --> GPIO17
//Red --> GPIO18
void setup() {
  pinMode(15,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(18,OUTPUT);


  digitalWrite(15,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(16,HIGH);
  digitalWrite(17,HIGH);
  digitalWrite(18,HIGH);
}

void loop() {}
