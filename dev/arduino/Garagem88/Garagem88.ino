int alarme = 13;
int garagem = 12;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete



void setup () {
  Serial.begin (9600);
  pinMode (garagem, OUTPUT); 
  pinMode (alarme, OUTPUT);  
  
  digitalWrite (garagem, LOW);
  digitalWrite (alarme, LOW);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

void loop () {
  
  if (stringComplete) {
    //Serial.println(inputString); 
    // clear the string:
    inputString = "";
    stringComplete = false;  
      
    if (inputString == "#on") {
      activate(alarme);
    }
    if (inputString == "#of") {
      activate(alarme);
    }
    if (inputString == "#op") {
      activate(garagem);
    }
    if (inputString == "#cl") {
      activate(garagem);
    }
  }
}

void activate (int pin) {
  digitalWrite (alarme, HIGH);
  delay(1000);
  digitalWrite (alarme, LOW);
  Serial.println("#do");
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } 
  }
}



