//const int ledPin = LED_BUILTIN;  // Usually GPIO2 (inverted logic: LOW = ON, HIGH = OFF)
const int BreakerPin1 = D5;
const int IsolatorPin1 = D6;

const int BreakerPin2 = D7;
const int IsolatorPin2 = D8;


void setup() {
//  pinMode(ledPin, OUTPUT);
  pinMode(BreakerPin1, OUTPUT);
  pinMode(IsolatorPin1, OUTPUT);
  pinMode(BreakerPin2, OUTPUT);
  pinMode(IsolatorPin2, OUTPUT);

//  digitalWrite(ledPin, HIGH); // Turn off LED initially (inverted logic)
  digitalWrite(BreakerPin1, HIGH);
  digitalWrite(IsolatorPin1, HIGH);
  digitalWrite(BreakerPin2, HIGH);
  digitalWrite(IsolatorPin2, HIGH);

}

void loop() {

  digitalWrite(BreakerPin1, HIGH);
  digitalWrite(IsolatorPin1, HIGH);
  digitalWrite(BreakerPin2, HIGH);
  digitalWrite(IsolatorPin2, HIGH);
  delay(1000); // Check every second

  digitalWrite(BreakerPin1, LOW);
  digitalWrite(IsolatorPin1, LOW);
  digitalWrite(BreakerPin2, LOW);
  digitalWrite(IsolatorPin2, LOW);
  delay(3000); // Check every second

}
