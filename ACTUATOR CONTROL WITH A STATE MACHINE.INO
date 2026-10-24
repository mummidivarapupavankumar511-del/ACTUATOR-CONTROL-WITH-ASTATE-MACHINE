#include <DHT.h>
#include <Servo.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define SERVO_PIN 9

DHT dht(DHTPIN, DHTTYPE);
Servo myServo;

const float TEMP_ON = 30.0;
const float TEMP_OFF = 27.0;

bool actuatorState = false;

void setup() {
  Serial.begin(9600);

  dht.begin();
  myServo.attach(SERVO_PIN);

  myServo.write(0);

  Serial.println("DHT22 and Servo Control Example");
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read DHT22 sensor!");
    delay(2000);
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  // Hysteresis state machine
  if (!actuatorState && temperature > TEMP_ON) {
    actuatorState = true;
    myServo.write(90);
    Serial.println("Actuator ON - Servo at 90 degrees");
  }
  else if (actuatorState && temperature < TEMP_OFF) {
    actuatorState = false;
    myServo.write(0);
    Serial.println("Actuator OFF - Servo at 0 degrees");
  }
  else {
    Serial.println(actuatorState
      ? "Actuator remains ON"
      : "Actuator remains OFF");
  }

  delay(2000);
}
