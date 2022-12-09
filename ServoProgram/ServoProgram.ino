#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  102 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  512 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  408 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2048 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;


//#############################
String x;
char *ejes[2];
char *separador = NULL;
//#############################

void setup() {

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  Serial.begin(115200);
  Serial.setTimeout(10);


}

void setServo(uint8_t n_servo, int angulo) {
  int duty;
  duty = map(angulo, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(n_servo, 0, duty);
}
//el ojo izquierdo gira maximo hasta 30° en el eje X - servo 0 y hasta 45 en el eje Y

void loop() {

  while (!Serial.available());
    x = Serial.readString();

    byte index = 0;
    separador = strtok(x.c_str(), ",");  // delimiter
    while (separador != NULL)
    {
      ejes[index] = separador;
      index++;
      separador = strtok(NULL, ",");
    }
  
    Serial.print("Eje X: ");
    Serial.print(ejes[0]);
    Serial.print(" || ");
    Serial.print("Eje Y: ");
    Serial.print(ejes[1]);

  int x = String(ejes[0]).toInt();
  int y = String(ejes[1]).toInt();

  int xInv = map(x, 40, 0, 0, 40);
  int yInv = map(y, 0, 40, 40, 0);

  //Movimiento de los ojos
  setServo(0, x);
  setServo(4, x);
  setServo(2, yInv);
  setServo(6, y);

  int parpadosUp = map(y, 0, 40, 90, 0);
  int parpadosDown = map(y, 40, 0, 40, 120);
  
  //Movimiento de parpados
  setServo(8, parpadosUp);
  setServo(10, parpadosDown);
  delay(10);
  



}
