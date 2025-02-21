const int ldrPin = A0;   // LDR link A0
const int led1Pin = 11;   // LED1 link D3
const int led2Pin = 10;   // LED2 link D4

int lastLightLevel = 0;  // the past light level
bool ledState = false;   // control LED1 / LED2 open and close
const int threshold = 10; // If the light changes by more than a certain value, it changes.

void setup() {
    pinMode(led1Pin, OUTPUT);
    pinMode(led2Pin, OUTPUT);
    digitalWrite(led1Pin, HIGH);  // open LED 1
    digitalWrite(led2Pin, LOW);   // close LED 2

    Serial.begin(9600); 
}

void loop() {
    int lightLevel = analogRead(ldrPin);  // the light level, help change the light
    Serial.print("Light level: ");
    Serial.println(lightLevel);

    // when the light level change, changed LED
    if (abs(lightLevel - lastLightLevel) > threshold) {
        ledState = !ledState; // switch LED
        Serial.println("Light changed! Switching LED.");
        lastLightLevel = lightLevel;  // detect the light have the change
    }

    // LED
    digitalWrite(led1Pin, ledState ? LOW : HIGH);
    digitalWrite(led2Pin, ledState ? HIGH : LOW);

    delay(100);  // delay a little
}
