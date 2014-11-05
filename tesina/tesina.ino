
/* Filippo Bertoncelli progetto tesina*/
#include <EEPROM.h>

char dato;
int mills;
int indS = 0;
boolean dirR = 0,dirL = 0;
int prevmills = 0;
boolean control;
int intervallo=0x23;
int interval = 1000;
int indW = 0;
int indR = 0;
int salv[]={
  0,0,0,0,0,0,0,0,0,0};
byte directive;
boolean interr=false;


void setup(){
  EEPROM.write(1011,0x80);//svolta destra
  EEPROM.write(1012,0x23);//tempo svolta
  EEPROM.write(1013,0x05);//avanti
  EEPROM.write(1014,0x64);//tempo avanti
  EEPROM.write(1015,0x40);//svolta destra
  EEPROM.write(1016,0x23);//tempo svolta
  EEPROM.write(1017,0x01);//fine intervallo svolta
  EEPROM.write(1018,0x40);//svolta destra
  EEPROM.write(1019,0x23);//tempo svolta
  EEPROM.write(1020,0x05);//avanti
  EEPROM.write(1021,0x64);//tempo avanti
  EEPROM.write(1022,0x80);//svolta destra
  EEPROM.write(1023,0x23);//tempo svolta
  Serial.begin(9600);
  attachInterrupt(0,toggle,FALLING); 
  pinMode(4,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(5,OUTPUT); 
  pinMode(6,OUTPUT);
  pinMode(A0,INPUT);
  pinMode(A5,INPUT);
  pinMode(A1,INPUT); 
}

void loop(){
  if (interr){
    mills = millis();
    if(mills-prevmills>interval)
    {
      if (indR == 1014 || indR == 1020){
        control = true;
        Serial.println("control vero");
      }
      Serial.println("intervallo finito");
      prevmills = mills;
      directive = EEPROM.read(indR);
      Serial.print("indice = ");
      Serial.println(indR);
      Serial.print("direttiva = ");
      Serial.println(directive);
      Serial.print("tempo = ");
      Serial.println(EEPROM.read(indR+1));
      indR++;
      if (directive == 0xff)
      {    
        Serial.println("fine percorso"); 
        analogWrite(5,0);
        analogWrite(6,0);
        interr = false;
      }
      else if(directive == 2)
      {
        indR = 0;
        Serial.println("reinizio percorso");
        interr = true;
        interval = 0;
      }
      else if(directive == 1)
      {
        indR = salv[indS--];
        interr = true;
        interval = 0;
      }
      else{
        Serial.println("perc");
        dirR = directive & 0x80;
        dirL = directive & 0x40;

        interval = EEPROM.read(indR)* 0x0A;
        if(dirL != dirR && indR < 1010) intervallo = interval; 
        indR++;
        Serial.print("intervallo = ");
        if (dirL!= dirR && indR >1010)interval=intervallo;
        Serial.println(interval);
        digitalWrite(4,dirR);
        digitalWrite(7,dirL);
        analogWrite(5,115);
        analogWrite(6,100);
      }

      if((directive == 0x80 || directive == 0x40)&& (indR == 1022 || indR == 1016))
      {
        indR = salv[indS--];
      }
    }
    else if(!((indR <= 1015 && indR >= 1012)||(indR <=1021 && indR >= 1018)))
    {

      seeObstacle();     
    } 
  }
  else{
    analogWrite(5,0);
    analogWrite(6,0);
  } 
}

//-----------------------------------------------------------
void seeObstacle()
{
  if((digitalRead(A0)== LOW)||(digitalRead(A5)== LOW)){
    analogWrite(5,0);
    analogWrite(6,0);
    Serial.println("see obstacle");
    avoidObstacle();
  }       
}
void avoidObstacle()
{
  salv[indS++] = indR;
  interval = 0;
  control = false;
  Serial.println("control falso");
  if(digitalRead(A0)== false && digitalRead(A5)== true){
    indR = 1011;  
  }
  else if(digitalRead(A0)== true && digitalRead(A5)== false){
    indR = 1018;  
  }
  else{
    if(random(2))
      indR = 1011;
    else
      indR = 1018;  
  }
}
//-----------------------------------------------------
void toggle()
{
  int s = 1;
  unsigned long vel = 0;
  Serial.print('j');
  if(interr == false)
  {
    indR = 0;
    interr = true;
    indS = 0; 
  }
  else
    interr = false; 

}//--------------------------------------------------
//---------------------------------------------------

void serialEvent(){
  byte x;
  int bat;
  dato = Serial.read();
  if (dato =='$'){
    delay(5);
    while(Serial.available()){
      x = Serial.read();
      EEPROM.write(indW,x);
      Serial.print((char)EEPROM.read(indW));
      Serial.print("  ");
      Serial.println(indW);
      indW++;
    }
    indW=0;  
  }
  if (dato == '#'){
    bat = analogRead(A1);
    bat = map(bat, 0, 1023, 0, 5000); 
    Serial.print("Battery ");
    Serial.println(bat);
  }
}

