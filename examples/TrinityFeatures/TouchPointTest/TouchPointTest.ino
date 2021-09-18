/*
This is un example howto use Touch Intrrerupts
The bigger the threshold, the more sensible is the touch
*/

int threshold = 40; //This is a decent value from testing
bool touch8detected = false;
bool touch9detected = false;

void gotTouch8(){
 touch8detected = true;
}

void gotTouch9(){
 touch9detected = true;
}

void setup() {
  Serial.begin(115200);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Interrupt Test");
  touchAttachInterrupt(T8, gotTouch8, threshold);
  touchAttachInterrupt(T9, gotTouch9, threshold);
}

void loop(){
  if(touch8detected){
    touch8detected = false;
    Serial.println("Touch 8 detected");
  }
  if(touch9detected){
    touch9detected = false;
    Serial.println("Touch 9 detected");
  }
}
