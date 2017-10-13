// This #include statement was automatically added by the Particle IDE.
#include "Ubidots.h"

// This #include statement was automatically added by the Particle IDE.
#include <Ubidots.h>

#define TOKEN "A1E-ygI38GYFsO4oeK4XklfOEsVDwQUA3e"  // Put here your Ubidots TOKEN

Ubidots ubidots(TOKEN);

int useageTime = 0;//in ms
int totalUseageTime = 0; //in minutes
int motionSensorPin = D1;
int ledPin = D4;
int lastState = HIGH;
int lastChange = 0;
int lastSyncMin;
boolean doPublish = false;

void setup() {
    Serial.begin(115200);
    pinMode(motionSensorPin, INPUT_PULLUP);
    pinMode(ledPin,OUTPUT);
    attachInterrupt(motionSensorPin, readMotion, CHANGE);
    lastChange = millis();
    Particle.variable("timeInHall",useageTime);
    Particle.function("ForceSync", forceSendData);
    Particle.function("FlashLight", flashLED);
    //ubidots.setDebug(true); //Uncomment this line for printing debug messages
}
void loop() {
    if(Time.minute()%15 == 0 && Time.minute() != lastSyncMin) {
        if(Time.minute()%30 == 0) {
            doPublish = true;
        } else {
            doPublish = false;
        }
        publishDataSequence();
    }
    Serial.println(digitalRead(motionSensorPin));
   // analogWrite(ledPin, analogRead(motionSensorPin));
    if(digitalRead(motionSensorPin) == HIGH) {
        digitalWrite(ledPin, HIGH);
    } else {
        digitalWrite(ledPin, LOW);
    }

    //readMotion();
}

int flashLED(String useless) {
    digitalWrite(ledPin,HIGH);
    delay(1000);
    digitalWrite(ledPin,LOW);
    return 1;
}

int forceSendData(String useless) {
    publishDataSequence();
    return 0;
}
void publishDataSequence() {
    sendData();
    ifttPublish();
    lastSyncMin = Time.minute();
}

void readMotion() {
    int motion = digitalRead(motionSensorPin);
    int spent = 0;
    if(lastState == LOW && motion == HIGH) {
        spent = millis() - lastChange;
        useageTime += spent;
        lastState = motion;
    } else if(lastState == HIGH && motion == LOW) {
        lastChange = millis();
        lastState = motion;
    } else if(lastState == LOW && motion == LOW){
        spent = millis() - lastChange;
        useageTime += spent;
        lastState = motion;
    } else {

    }
    //Serial.println(motion);
}

void sendData() {
    if(Particle.connected() == 0) {
        waitUntil(Particle.connected);
        sendData();
    } else {
        totalUseageTime = useageTime/60000.0;
        useageTime = 0;
        char context[25];
        unsigned long t = ubidots.ntpUnixTime();
        ubidots.add("TimeInUse", totalUseageTime, context);
        ubidots.sendAll();
    }
}

void ifttPublish() {
    if(doPublish) {
        if(Particle.connected() == 0) {
            waitUntil(Particle.connected);
            sendData();
        } else {
            Particle.publish("DataSent", totalUseageTime);
            totalUseageTime = 0;
            Serial.print("Data Sent and reset");
        }
    }
    flashLED("hello");
}
