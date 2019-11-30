 #include <SparkFun_MAG3110.h>
#include <SparkFun_TB6612.h>

const int offsetA = 1;
const int offsetB = 1;
int migrating = 0;
int migrated = 0;

#define AIN1 2
#define BIN1 6
#define AIN2 4
#define BIN2 7
#define PWMA 3
#define PWMB 5
#define STBY 9

Motor motor2(BIN1,BIN2,PWMB, offsetB, STBY);
Motor motor1(AIN1,AIN2,PWMA, offsetA, STBY);

MAG3110 mag = MAG3110(); //Instantiate MAG3110

void setup() {
  Serial.begin(9600);

  Wire.begin();             //setup I2C bus
  Wire.setClock(400000);    // I2C fast mode, 400kHz

  mag.initialize(); //Initialize the MAG3110
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

  int x, y, z;

  if(!mag.isCalibrated()) //If we're not calibrated
  {
    if(!mag.isCalibrating()) //And we're not currently calibrating
    {
      Serial.println("Entering calibration mode");
      mag.enterCalMode(); //This sets the output data rate to the highest possible and puts the mag sensor in active mode
    }
    else
    {
      //Must call every loop while calibrating to collect calibration data
      //This will automatically exit calibration
      //You can terminate calibration early by calling mag.exitCalMode();
      mag.calibrate();
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
  else
  {
    Serial.println("Calibrated!");
    digitalWrite(LED_BUILTIN, HIGH);
  }
  mag.readMag(&x, &y, &z);

  Serial.print("X: ");
  Serial.print(x);
  Serial.print(", Y: ");
  Serial.print(y);
  Serial.print(", Z: ");
  Serial.println(z);

  Serial.print("Heading: ");
  Serial.println(mag.readHeading());

  Serial.println("--------");

  

  if(mag.readHeading() > 10 && mag.readHeading() < 170 && mag.isCalibrated() && migrated == 0) {    // NORTHEAST TO SOUTHEAST
      Serial.println("GOING LEFT");
      left(motor1,motor2,85);
  }
  else if (mag.readHeading() < -10 && mag.readHeading() > -170 && mag.isCalibrated() && migrated == 0){                      // NORTHWEST TO SOUTHWEST
    Serial.println("GOING RIGHT");
    right(motor1,motor2,85);
  }
  else {
    if (migrating < 15 && mag.isCalibrated()) {
      Serial.println("TRAVELING NORTH");
      back(motor1, motor2, 85);
      migrating += 1;
      migrated = 1;
    }
    Serial.println("MIGRATED");
  }
  delay(250);




    
  }
