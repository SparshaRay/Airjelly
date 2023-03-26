
#include <Servo.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

Servo flap1;
Servo flap2;
Servo flap3;
Servo flap4;

const byte numChars = 24;
char receivedChars[numChars];
char tempChars[numChars];
char messageFromPC[numChars] = {0};
int yaw = 0;
int pitch = 0;
int flapdelay = 8000;
boolean newData = false;


float pos1 = 0;
float pos2 = 0;
float pos3 = 0;
float pos4 = 0;

float initpos = 0;
float finalpos = 150;

float offset1 = 0;
float offset2 = 0;
float offset3 = 0;
float offset4 = 0;

const float strokevel =  0.2;
const float factor = 1000;

float angvel1 = strokevel;
float angvel2 = strokevel;
float angvel3 = strokevel;
float angvel4 = strokevel;

float returnvel = -0.4;


void setup() {
  mySerial.begin(9600);
  mySerial.println("System Online");
  flap1.attach(12);
  flap2.attach(11);
  flap3.attach(10);
  flap4.attach(9);
}

void loop() {

  recvWithStartEndMarkers();
  if (newData == true) {
      strcpy(tempChars, receivedChars);
      parseData();
      newData = false;
      mySerial.println("200OK");
  }

  pos1 = pos1 + angvel1;
  pos2 = pos2 + angvel2;
  pos3 = pos3 + angvel3;
  pos4 = pos4 + angvel4;
  flap1.write(offset1 + pos1);
  flap2.write(offset2 + pos2);
  flap3.write(offset3 + pos3);
  flap4.write(offset4 + pos4);

  if (pos1>finalpos){
    angvel1 = 0;
  }
  if (pos2>finalpos){
    angvel2 = 0;
  }
  if (pos3>finalpos){
    angvel3 = 0;
  }
  if (pos4>finalpos){
    angvel4 = 0;
  }

  if (pos1>finalpos && pos2>finalpos && pos3>finalpos && pos4>finalpos) {
    reset();
  }

  if (pos1<initpos && pos2<initpos && pos3<initpos && pos4<initpos) {
    angvel1 = strokevel + yaw/factor;
    angvel2 = strokevel + pitch/factor;
    angvel3 = strokevel - yaw/factor;
    angvel4 = strokevel - pitch/factor;
  }

  delayMicroseconds(flapdelay);
}

void reset() {
  angvel1 = returnvel;
  angvel2 = returnvel;
  angvel3 = returnvel;
  angvel4 = returnvel;
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (mySerial.available() > 0 && newData == false) {
        rc = mySerial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0';
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}


void parseData() {

    char * strtokIndx;

    strtokIndx = strtok(tempChars, ",");
    flapdelay = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    yaw = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    pitch = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    returnvel = atof(strtokIndx);

}
