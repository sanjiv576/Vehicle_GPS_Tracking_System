// Arduino to NodeMCU GPS data transfer

void setup() {

  //  Serial communication port of NodeMCU
  Serial.begin(115200);
  Serial.print(" Waiting for the connection ");
  while (!Serial) {
    Serial.print(" .");
  }
}

void loop(){
  if(Serial.available()){
    Serial.print("Connection found");
    Serial.write(Serial.read());
  }
}
