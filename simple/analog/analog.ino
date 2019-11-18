#define BAUD_RATE 9600

#define PIN_OUTPUT 6
#define PIN_INPUT A2

int input, output;

char format[32];

void setup() {
  Serial.begin(BAUD_RATE);
  pinMode(PIN_OUTPUT, OUTPUT);
}

void loop() {
  input = analogRead(PIN_INPUT);
  output = input / 4;
  analogWrite(PIN_OUTPUT, output);
  sprintf(format, "%d,%d", input, output);
  Serial.println(format);
}
