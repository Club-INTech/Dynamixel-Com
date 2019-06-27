#include "Arduino.h"
#include "DynamixelUtils.h"
#include "DynamixelManager.h"
#include "XL430.h"
#include <vector>


static DynamixelManager* manager = new DynamixelManager(&Serial1);
static XL430* motor1 = new XL430(1,*manager);
static XL430* motor2 = new XL430(2,*manager);
static XL430* motor3 = new XL430(3,*manager);

static std::vector<XL430*> motors{motor1,motor2,motor3};

void setup()
{
    Serial.begin(115200);
    pinMode(13,OUTPUT);

    digitalWrite(13,HIGH);

    motor1->toggleTorque(true);
    motor2->toggleTorque(true);
    motor3->toggleTorque(true);

    delay(1000);

    digitalWrite(13,LOW);
}

void loop()
{
    static float angle = 0;
    static float oldAngle = 0;

    int id = 1;

    if(Serial.available())
    {
        oldAngle = angle;
        String input = Serial.readString(10);

        id = input.substring(0,1).toInt();
        angle = input.substring(2).toFloat();
        Serial.printf("Moteur %i, angle %f \r\n",id, angle);
    }

    if(oldAngle != angle)
    {
        oldAngle = angle;
        digitalWrite(13,HIGH);
        motors.at(id-1)->setGoalAngle(angle);
    }

    digitalWrite(13, LOW);

    delay(100);
}