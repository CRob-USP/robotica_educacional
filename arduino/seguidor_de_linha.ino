//Curso de Arduino
//Robô seguidor de linha

//portas do arduino que controlam o motor 1 (Saidas OUT1 e OUT2 da ponte H)
#define IN_1 2
#define IN_2 3

//portas do arduino que controlam o motor 2 (Saidas OUT3 e OUT4 da ponte H)
#define IN_3 4
#define IN_4 5

//porta do arduino que controla a velocidade do motor 1
#define EN_A 6

//porta do arduino que controla a velocidade do motor 2
#define EN_B 7

//portas de leitura dos sensores
#define sensor_dir 40
#define sensor_esq 42

//Tempo de giro
#define tempo 400

//A constante "tempo" determina o tempo que os motores vão atuar
//em resposta a detecção do sensor, ou seja, quando o sensor detecta a 
//linha, é por um tempo muito pequeno, dessa forma esse tempo
//não é suficiente para o robô corrigir sua trejetória. Por isso entra
//a um delay(tempo) para aumentar o tempo de reação a correção de trajetória

//Variaveis de PWM para controle de velocidade do motor
int vel_motor1 = 70;
int vel_motor2 = 70;

int leitura_dir = 0;
int leitura_esq = 0;

void setup() {
  Serial.begin(1200);

  //Habilita os pinos do arduino como saida de tensão (V)
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(EN_A, OUTPUT);
  pinMode(EN_B, OUTPUT);

  //Habilita os pinos do arduino como entrada de tensão (V)
  pinMode(sensor_dir, INPUT);
  pinMode(sensor_esq, INPUT);
}

void loop() {

  //Váriaveis que recebem as leituras dos sensores
  leitura_dir = digitalRead(sensor_dir);
  leitura_esq = digitalRead(sensor_esq);

  Serial.print("Sensor esquerda: ");
  Serial.println(leitura_esq);
  Serial.print("Sensor direita: ");
  Serial.println(leitura_dir);


  if (leitura_dir == 0 && leitura_esq == 1) {
    //Robô vira para a direita
    //sentido de giro do motor 1
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    analogWrite(EN_A, vel_motor1);

    //sentido de giro do motor 2
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
    analogWrite(EN_B, vel_motor2);
    delay(tempo);
  }

  else if (leitura_dir == 1 && leitura_esq == 0) {
    //Robô vira para a esquerda
    //sentido de giro do motor 1
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    analogWrite(EN_A, vel_motor1);

    //sentido de giro do motor 2
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
    analogWrite(EN_B, vel_motor2);
    delay(tempo);
  }

  else {
    //Robô para frente
    //sentido de giro do motor 1
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    analogWrite(EN_A, vel_motor1);

    //sentido de giro do motor 2
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
    analogWrite(EN_B, vel_motor2);
  }
}
