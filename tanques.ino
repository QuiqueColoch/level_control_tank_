/*********
  Enrique Coloch
  Complete project details at:
  
*********/

#include <SoftwareSerial.h>

#define sensorQ1  A0
#define sensorQ2  A1
#define sensorQ3  A2

#define luz0 5
#define luz1 6
#define luz2 7

int lecturaS0=0;
int lecturaS1=0;
int lecturaS2=0;

//altura de tanques

const int H_Q = 193; 
float H_QR = 193.00;
const int CAPACIDAD = 1178;

int nivelQ0;
int nivelQ1;
int nivelQ2;
float xQ0;
float xQ1;
float xQ2;
int a0;
int a1;
int a2;


SoftwareSerial Bt(3, 4);
int input_serial=0;

void setup(){
  Serial.begin(9600);  
  Bt.begin(9600);
  pinMode(sensorQ1,INPUT);
  pinMode(luz0,OUTPUT);
  pinMode(luz1,OUTPUT);
  pinMode(luz2,OUTPUT);
  
  digitalWrite(luz0,LOW);
  digitalWrite(luz1,LOW);
  digitalWrite(luz2,LOW);
}

int readSensors(){

  int samplesQuantity = 20;
  int promedio=0;
  int samples[samplesQuantity];
  int sumatoria;

  sumatoria=0;
  for(int i =0; i< samplesQuantity; i++){
    samples[i] = analogRead(A0);
    delay(200);
  }

  for(int i=0; i < samplesQuantity ;i++){
    sumatoria = sumatoria + samples[i];
  }

  lecturaS0 = (int) sumatoria / samplesQuantity;

  sumatoria=0;
  
  for(int i=0; i< samplesQuantity; i++){
    samples[i] = analogRead(A1);
  }

  for(int i=0; i < samplesQuantity ;i++){
    sumatoria = sumatoria + samples[i];
  }

  lecturaS1 = (int) sumatoria / samplesQuantity;

  sumatoria=0;
  
  for(int i =0; i< samplesQuantity; i++){
    samples[i] = analogRead(A2);
  }

  for(int i=0; i < samplesQuantity ;i++){
    //Serial.print(String(samples[i]) +",");
    sumatoria = sumatoria + samples[i];
  }

  //delay(200);
  lecturaS2 = (int) sumatoria / samplesQuantity;
  //Serial.println(lecturaS2);
}

int calculoNivel(int valor){
  int nivel;
  long distancia;
  int h;
  
  distancia = 500 * (long) valor;
  distancia = distancia /1023;
  if(distancia >= H_Q ){
     nivel = 0;
  }else if(distancia <=0 ){
    nivel = 100;
  }else{
    h = H_Q - distancia;
    nivel = (int) h * 100;
    nivel = (int) nivel / H_Q;
  }  
  return nivel;
}

float obtener_distancia_h(int valor){
  //long distancia;
  //int h;

  float distancia;
  float h;
  
  distancia = 500 * (float) valor;
  distancia = distancia /1023;

  if(distancia >= H_QR ){
    h = 0;
  }else if(distancia <=0 ){
    h = H_QR;
  }else{
    h = H_QR - distancia;    
  }  
  
  return h;
}

void loop(){
  
  String recibido;
  
  readSensors();
  
  if (Serial.available()>0){
    recibido = Serial.readString();  
    input_serial = recibido.toInt();
    Serial.flush();
  }
  Serial.println("INPUTSERIAL: " + String(input_serial));
  
  /*
  lecturaS0 = input_serial;
  lecturaS1 = input_serial;
  lecturaS2 = input_serial;
  */
  
  nivelQ0 =  calculoNivel(lecturaS0);
  nivelQ1 =  calculoNivel(lecturaS1);
  nivelQ2 =  calculoNivel(lecturaS2);

  xQ0 = obtener_distancia_h(lecturaS0);
  xQ1 = obtener_distancia_h(lecturaS1);
  xQ2 = obtener_distancia_h(lecturaS2);

  Serial.println("Esta es la distancia x: " + String(xQ0));
  
  if(nivelQ0 <= 20){
    digitalWrite(luz0,LOW);
    a0 = 1;
  }else{
    digitalWrite(luz0,HIGH);
    a0 = 0;
  }

  if(nivelQ1 <= 20){
    digitalWrite(luz1,LOW);
    a1 = 1;
  }else{
    digitalWrite(luz1,HIGH);
    a1 = 0;
  }

  if(nivelQ2 <= 20){
    digitalWrite(luz2,LOW);
    a2 = 1;
  }else{
    digitalWrite(luz2,HIGH);
    a2 = 0;
  }

  String frame = "<T,N0,N1,N2,x0,x1,x2,A0,A1,A2>";

  frame.replace("N0",String(nivelQ0,DEC));
  frame.replace("N1",String(nivelQ1,DEC));
  frame.replace("N2",String(nivelQ2,DEC));

  frame.replace("x0",String(xQ0,2));
  frame.replace("x1",String(xQ1,2));
  frame.replace("x2",String(xQ2,2));

  frame.replace("A0",String(a0,DEC));
  frame.replace("A1",String(a1,DEC));
  frame.replace("A2",String(a2,DEC));
  
  Serial.println("nivel_0 : " + String(nivelQ0));
  Serial.println("nivel_1 : " + String(nivelQ1));
  Serial.println("nivel_2 : " + String(nivelQ2));
  Serial.println("xQ0 : " + String(xQ0));
  Serial.println("xQ1 : " + String(xQ1));
  Serial.println("xQ2 : " + String(xQ2));
  Serial.println("A0 : " + String(a0));
  Serial.println("A1 : " + String(a1));
  Serial.println("A2 : " + String(a2));
  
  Bt.println(frame);
  Serial.println(frame);
  //delay(1000);
  

}
