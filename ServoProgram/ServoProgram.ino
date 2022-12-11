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

int in;
//#############################

int ojo_izq_x = 0;
int ojo_izq_y = 2;
int ojo_der_x = 4;
int ojo_der_y = 6;
int parpadosUp = 8;
int parpadoDown = 10;

void setup() {

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  Serial.begin(115200);
  Serial.setTimeout(10);

  
  setServo(0, 37);
  setServo(2, 29);
  setServo(4, 17);
  setServo(6, 22);

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

     
      //ojo izquierdo va de 15 a 60 en el X || de 10 a 48 en el Y
      //ojo derecho va de 0 a 35 - 15 default en el X || de 0 a 45 en el Y
      // el parpado superior va de 0 a 130 - 90 default
      // el parpado inferior de 30 a 120 - 90 default
      
    

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

  int xizq = map(x, 0, 40, 15, 60);
  setServo(0, xizq);
  int yizq = map(y, 0, 40, 10, 48);
  setServo(2, yizq);
  
  int xder = map(x, 0, 40, 0, 35);
  setServo(4, xder);
  int yder = map(y, 40, 0, 0, 45);
  setServo(6, yder);
  
  int xInv = map(x, 40, 0, 0, 40);
  int yInv = map(y, 0, 40, 40, 0);

  //Movimiento de los ojos
  //setServo(4, x);
  //setServo(4, x);
  //setServo(2, yInv);
  //setServo(6, y);

  int parpadosUp = map(y, 0, 40, 90, 0);
  int parpadosDown = map(y, 40, 0, 120, 40);
  
  //Movimiento de parpados
  //setServo(8, parpadosUp);
  //setServo(10, parpadosDown);
  delay(10);

}
