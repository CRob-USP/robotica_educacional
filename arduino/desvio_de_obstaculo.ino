//Programa: Conectando Sensor Ultrassonico HC-SR04 ao Arduino
 
//Carrega a biblioteca do sensor ultrassonico
#include <Ultrasonic.h>
 
//Define os pinos para o trigger e echo
#define pino_trigger 4
#define pino_echo 5

//Define os pinos para a Ponte H
#define IN1  2
#define IN2  3
#define IN3  4
#define IN4  5
#define ENA  6
#define ENB  7

int vel = 180;
int tempo = 200;
int tempogiro = 300;

//Robô para e dá um giro de meia volta
void desvioRobo();
//Robô segue em frente até detectar um obstáculo
void sigaFrente();
 
//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic(pino_trigger, pino_echo);
 
void setup()
{
  Serial.begin(9600);
  Serial.println("Lendo dados do sensor...");
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
}
 
void loop()
{
  //Le as informacoes do sensor, em cm
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();
  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  //microsec é uma variável que contém o tempo de voo do sinal 
  //ultrassônico, ou seja, o tempo (em microssegundos) que o sinal
  //levou para ir até o objeto e voltar ao sensor. Esse valor
  // geralmente é obtido após disparar o sensor ultrassônico e medir o tempo do eco.
  //Exibe informacoes no serial monitor
  Serial.print("Distancia em cm: ");
  Serial.print(cmMsec);

  if(cmMsec < 20.0){
    desvioRobo();
  }
  else{
    sigaFrente();
  }
}
void desvioRobo(){
  //Robô para por um determinado tempo
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  //Tempo em que o robô fica parado
  delay(tempo);

  //Invertendo os sentidos de giro da Ponte H
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, vel);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, vel);

  //Tempo de giro para uma meia volta
  delay(tempogiro);

}

void sigaFrente(){
  //Configuração da Ponte H para o robô seguir em frente
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, vel);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, vel);
}