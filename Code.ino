//se incluye la librería del módulo SD
#include <SD.h>
//se incluye la librería para controvlar el audífono                          
#include <TMRpcm.h>                      

//define el pin CS del módulo de SD.
#define ChipSelectPin 4            

//se crea un objeto para la librería de control del audífono
TMRpcm audifono;                 

const int waterDetectorPin = 24;     
int waterDetectorState = 0;         
const int chargeConsultButton = 9;

int analogValue = 0;
float voltage = 0;
float batteryPercentage = 0;
int percentageIndicator;


//Selección de puertos del arduino para los sensores
//sensor derecho
const int trigSenDer = 43;
const int echoSenDer = 42;

//sensor medio
const int trigSenMed = 3;
const int echoSenMed = 2;

//sensor izquierdo
const int trigSenIzq = 5;
const int echoSenIzq = 6;

//sensor inferior
const int trigSenInf = 7;
const int echoSenInf = 8;

const int trigSenUp = 33;
const int echoSenUp = 28;

//pines del motor para la rueda izquierda
const int ruedaIzq1 = 11;
const int ruedaIzq2 = 16;

//pines del motor para la rueda derecha
const int ruedaDer1 = 10;
const int ruedaDer2 = 17;

//pines del motor para el sistema de vibración izquierdo
const int vibIzq = 18;

//pines del motor para el sistema de vibración medio
const int vibMed = 15;

//pines del motor para el sistema de vibración derecho
const int vibDer = 14;


const int chargeConsult = 53;

int tolRegulator = A0;  // analog pin used to connect the potentiometer
int tolerance;    // variable to read the value from the analog pin
int regulatedDistanceSound;

//cuando el Arduino se inicia:
void setup(){
  
  pinMode(waterDetectorPin, INPUT);
  
  pinMode(53,OUTPUT);
  pinMode(18,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(14,OUTPUT);

  //se definen los pines de la rueda izquierda como salidas
  pinMode(ruedaIzq1, OUTPUT);
  pinMode(ruedaIzq2, OUTPUT);
  
  //se definen los pines de la rueda derecha como salidas
  pinMode(ruedaDer1, OUTPUT);
  pinMode(ruedaDer2, OUTPUT);

//se inicia la comunicación serial (forma de comunicación entre la computadora y el circuito electrónico)
  Serial.begin (9600);   

//se definen los pines trigger de los sensores como salidas y los echo como entradas
  pinMode(trigSenDer, OUTPUT);
  pinMode(echoSenDer, INPUT);
  pinMode(trigSenMed, OUTPUT);
  pinMode(echoSenMed, INPUT);
  pinMode(trigSenIzq, OUTPUT);
  pinMode(echoSenIzq, INPUT);
  pinMode(trigSenInf, OUTPUT);
  pinMode(echoSenInf, INPUT);
  pinMode(trigSenUp, OUTPUT);
  pinMode(echoSenUp, INPUT);
  
  //se define el pin del audífono.
  audifono.speakerPin = 46;      
  
  //se comprueba si hay una tarjeta SD y si puede ser inicializada
  if (!SD.begin(ChipSelectPin)) { 
    
    Serial.println("No SD available");    
    //si no hay, no se hace nada más
    return;                              
  }  

  audifono.setVolume(2);                   
  audifono.play("h.wav");


}


//se inicia a programar los diferentes métodos a utilizar.

//método moverIzq
void moverIzq(){
  
  Serial.println("DERECHO ******DERECHO**");  
  
  //se gira a la izquierda
  digitalWrite(ruedaIzq1, LOW);
  digitalWrite(ruedaIzq2, HIGH);
  digitalWrite(ruedaDer1, HIGH);
  digitalWrite(ruedaDer2, LOW);
  
  //el motor del sistema de vibración derecho se activa
  digitalWrite(vibDer, HIGH);
  
  //Se selecciona el nivel de volumen (del 0 al 7)
  audifono.setVolume(2);         
               
  //el audio "3" se reproduce
  audifono.play("l.wav");
          
}

//se repite el proceso para el método "moverDer"
void moverDer(){

  Serial.println("IZQUIERDO ******IZQUIERDO  ****");
  
  digitalWrite(ruedaIzq1, HIGH);
  digitalWrite(ruedaIzq2, LOW);
  digitalWrite(ruedaDer1, LOW);
  digitalWrite(ruedaDer2, HIGH);
  digitalWrite(vibIzq, HIGH);
  audifono.setVolume(2);                    
  audifono.play("r.wav"); 
}

//se repite el proceso para el método "moverAtrás", pero no se incluye el audio
void moverAtras(){
 //se gira hacia atrás
 digitalWrite(ruedaIzq1, HIGH);
 digitalWrite(ruedaIzq2, LOW);
 digitalWrite(ruedaDer1, HIGH);
 digitalWrite(ruedaDer2, LOW);
 digitalWrite(vibDer, HIGH);
 digitalWrite(vibMed, HIGH);
 digitalWrite(vibIzq, HIGH);
}
void consultCharge(){
digitalWrite(chargeConsult, HIGH);
audifono.play(batteryPercentage + ".wav");
delay(3000);

}



void loop(){

tolerance = analogRead(tolRegulator);          
tolerance = map(tolerance, 0, 1023, 1, 300);    
regulatedDistanceSound =  tolerance + ".wav";

waterDetectorState = digitalRead(waterDetectorPin);
if (waterDetectorState == HIGH) {
  moverAtras();
  delay(500);
  audifono.play("w.wav");
  delay(2000);
}

  analogValue = analogRead(A1);
  voltage = 0.0048*analogValue;
  batteryPercentage = (100*voltage)/5;
  if (chargeConsultButton == HIGH){
    consultCharge();
  }


//se crean las variables "tiempo" y "distancia"
long tiempo, distancia;

//el trigger del sensor se desactiva...
digitalWrite(trigSenDer, LOW); 
//...durante 2 milisegundos
delayMicroseconds(2);

//el trigger del sensor se activa...
digitalWrite(trigSenDer, HIGH);
//...durante 10 milisegundos
delayMicroseconds(10);

//luego se vuelve a desactivar
digitalWrite(trigSenDer, LOW);

//el tiempo es igual a lo que el echo del sensor registra
tiempo = pulseIn(echoSenDer, HIGH);

/*se establece que la distancia es igual al tiempo que le tomó al sonido ir y regresar multipicado por la velocidad aproximada del sonido (0.3432 cm/us) sobre 2. 
Esto se hace para convertir la distancia a cm.
 */
distancia= tiempo*0.03432/2; 
Serial.println("DERECHO: ");
Serial.println(distancia);

//Si la distancia es menor al valor de la "tolerancia", previamente establecido, y a su vez es mayor a 0, entonces...
if (distancia < tolerance && distancia > 0 )
{
  //...se activa el método "moverIzq"
  moverIzq();
  delay(2000);
}
//se repite el proceso anerior pero para el sensor del medio.
long tiempo2, distancia2;
digitalWrite(trigSenMed, LOW);
delayMicroseconds(2);
digitalWrite(trigSenMed, HIGH);
delayMicroseconds(10);
digitalWrite(trigSenMed, LOW);
tiempo2 = pulseIn(echoSenMed, HIGH);
distancia2= tiempo2*0.03432/2;
Serial.println("CENTRAL: ");
Serial.println(distancia2);


if (distancia2 < tolerance && distancia2 > 0)
{
  moverAtras();
  //en este caso, después de activar el método "moverAtras", se selecciona el volumen del audio del 0 al 7.
  audifono.setVolume(2);                    
  //se reproduce el audio "4.wav"
  audifono.play("f.wav");
  delay(2000);
}  
long tiempo3, distancia3;
digitalWrite(trigSenIzq, LOW);
delayMicroseconds(2);
digitalWrite(trigSenIzq, HIGH);
delayMicroseconds(10);
digitalWrite(trigSenIzq, LOW);
tiempo3 = pulseIn(echoSenIzq, HIGH);
distancia3= tiempo3*0.03432/2;
Serial.println("IZQUIERDO: ");
Serial.println(distancia3);
if (distancia3 < tolerance && distancia3 > 0 )
{
  moverDer();
  delay(2000);
}

long tiempo4, distancia4;
digitalWrite(trigSenInf, LOW);
delayMicroseconds(2);
digitalWrite(trigSenInf, HIGH);
delayMicroseconds(10);
digitalWrite(trigSenInf, LOW);
tiempo4 = pulseIn(echoSenInf, HIGH);
distancia4= tiempo4*0.03432/2;
Serial.println("INFERIOR: ");
Serial.println(distancia4);
if (distancia4 > 20)

{
  moverAtras();
   Serial.println("INFERIOR ******INFERIOR**");
  audifono.setVolume(2);                   
  audifono.play("d.wav");
  delay(2000);
}

long tiempo5, distancia5;
digitalWrite(trigSenUp, LOW);
delayMicroseconds(2);
digitalWrite(trigSenUp, HIGH);
delayMicroseconds(10);
digitalWrite(trigSenUp, LOW);
tiempo5 = pulseIn(echoSenUp, HIGH);
distancia5= tiempo4*0.03432/2;
Serial.println("SUPERIOR: ");
Serial.println(distancia5);
if (distancia5 < 200){
  moverAtras();
  audifono.play("u.wav");
}

//si todas las distancias son mayores al valor de "tolerancia" o son iguales a 0, entonces...
if ((distancia > tolerance | distancia == 0)  && (distancia2 > tolerance | distancia2 == 0) && (distancia3 > tolerance | distancia3 == 0 ) && (distancia4 > tolerance | distancia4 == 0 )){
  
//se apagan los motores de las ruedas
  digitalWrite(ruedaIzq1, LOW); 
  digitalWrite(ruedaIzq2, LOW);
  digitalWrite(ruedaDer1, LOW);
  digitalWrite(ruedaDer2, LOW);
  
//el motor cada sistema de vibración se desactiva
  digitalWrite(vibDer, LOW);
  digitalWrite(vibMed, LOW);
  digitalWrite(vibIzq, LOW);

//se desactiva el volumen
  audifono.setVolume(0); 

// se activa el pin 13 para que pueda activar la base del transistor e iniciar la carga automática
}
}
