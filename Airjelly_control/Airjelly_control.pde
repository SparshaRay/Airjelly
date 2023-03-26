
import processing.serial.*;
import controlP5.*;

ControlP5 cp5;
Serial myPort;
PImage img;

String[] data = {"<", "", ",", "", ",", "", ",-0.1", ">"};
int flapDelay = 100;
int yaw = 0;
int pitch = 0;
boolean flag = false;

void setup() {
  size(500,500);
  background(25);
  noStroke();
  textSize(25);
  textAlign(CENTER);
  img = loadImage("img.jpeg");
  image(img, 0, 0);
  
  myPort = new Serial(this, "/dev/rfcomm0", 9600);
  
  cp5 = new ControlP5(this);
    
  cp5.addSlider("flapDelay")
     .setPosition(150,100)
     .setSize(200,20)
     .setRange(4,2048)
     .setValue(2048)
     ; 
     
   text("Up-Down Control", 250, 65);
   textSize(15);
   text("(lesser values means faster flapping)", 250, 85);
   textSize(25);

   
   cp5.addSlider("yaw")
     .setPosition(150,260)
     .setSize(200,20)
     .setRange(-100,100)
     .setValue(0)
     ;   
     
   text("Tilt sideways", 250, 245);
     
     
   cp5.addSlider("pitch")
     .setPosition(150,420)
     .setSize(200,20)
     .setRange(-100,100)
     .setValue(0)
     ; 
     
   text("Tilt forward-backward", 250, 405);
}

void draw() {
  if (frameCount%60==0 && flag) {
    transmit();
    flag = false;
  }
}

void transmit() {
  data[1] = str(flapDelay);
  data[3] = str(yaw);
  data[5] = str(pitch);
  myPort.write(join(data, ""));
  println("Broadcast raised 200OK flag at rfcomm0, timestamp=", frameCount);
}

void controlEvent(ControlEvent interrupt)  {
  flag = true;
}
