//Máquina de estados finitos aplicada a um seguidor de linha
//Biblioteca responsavel por fazer o calculo da distancia pelo sesnor ultrassonico

//Define os pinos para o trigger e echo do sensor de distancia
int PinTrigger = 8; // Pino usado para disparar os pulsos do sensor
int PinEcho = 9; // pino usado para ler a saida do sensor

float tempoEcho = 0;
const float VelocidadeSom_mpors = 340; // em metros por segundo
const float VelocidadeSom_mporus = 0.000340; // em metros por microsegundo
float distancia;
float distminima = 12.00;

//Inicializa o sensor nos pinos definidos acima
//Ultrasonic ultrasonic(pino_trigger, pino_echo);

//Pinos de conexao do modulo do sensor de cor
#define pinS0 30
#define pinS1 32
#define pinS2 34
#define pinS3 36
#define pinOut 38

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

//portas de leitura dos sensores de linha
#define sensordireita 40
#define sensoresquerda 42

//variáveis de leitura do sensor de cor
unsigned int valorVermelho = 0;
unsigned int valorVerde = 0;
unsigned int valorAzul = 0;
unsigned int valorBranco = 0;

//Variaveis de PWM para controle de velocidade do motor
int vel_motor1 = 120; //robo 1 = 80  robo 2 = 120
int vel_motor2 = 120; //robo 2 = 80 robo 2 = 120
int vel_min = 120;

//tempo de giro na função de seguir a linha
int tempo = 160;

//Variável que recebe a leitura dos sensores de linha
int leituradireita;
int leituraesquerda;

//variável de estado da máquina de estados
int estado = 0;

//variaveis de entrada da maquina de estados
int azul;
int verde;
int vermelho;
int branco;
int obstaculo;

//Função máquina de estados
void maquinaEstados();
//Função que identifica linha
void identificaLinha();
//Função que identifica cor
void identificaCor();
//Função que faz a leitura do sensor de distancia e identifica sem tem obstaculo
void identificaObstaculo();
//Função Seguir Linha
void segueLinha();
//Função de movimento de giro do robô
void giro(int);
//Função de movimento do robô para frente
void frente(int, int);
//Função de movimento do robô para trás
void tras(int);
//Função que faz o robô parar
void pare(int, int);
//Função que dispara pulsos no sensor ultrassônico
void DisparaPulsoUltrassonico();
//Função que calcula a distância dos obstaculo
float CalculaDistancia(float);

void setup() {
  Serial.begin(9600);
  //Configure o pino especificado para funcionar como entrada ou saída
  //Pinagem para o sensor de COR
  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
  pinMode(pinOut, INPUT);

  digitalWrite(pinS0, HIGH);
  digitalWrite(pinS1, LOW);

  //Pinagem para controle da ponte H
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(EN_A, OUTPUT);
  pinMode(EN_B, OUTPUT);

  //Pinagem para os sensores de linha
  pinMode(sensordireita, INPUT);
  pinMode(sensoresquerda, INPUT);

  //Pinagem para o sesnsor de distancia
  // Configura pino de Trigger como saída e inicializa com nível baixo
  pinMode(PinTrigger, OUTPUT);
  digitalWrite(PinTrigger, LOW);
  pinMode(PinEcho, INPUT); // configura pino ECHO como entrada

}

void loop() {
  //passo 1 -> fazer a leitura dos sensores
  //passo 2 -> deve ser feito um filtro nessas leituras para a entrada da maquina de estados
  //passo 3 -> Com base nas entradas consideradas como verdadeiro ou falso
  //terá uma lógica de seleção para alternar os estados
  identificaLinha();
  identificaObstaculo();
  maquinaEstados();

  /*
  Serial.print("Azul: ");
  Serial.println(azul);
  Serial.print("Verde: ");
  Serial.println(verde);
  Serial.print("Vermelho ");
  Serial.println(vermelho);
  */
}

void maquinaEstados(){
  Serial.println("Entrando no estado 0");
  switch(estado){
    case 0:
      Serial.println("Estado 0");
      //comandos
      if(obstaculo){
        estado = 1;
      }else{
        segueLinha();
      }
    break;
    case 1:
      Serial.println("Estado 1");
      //comandos
      //movimento dos motores - parar e dar ré
      pare(500, 0);
      
      estado = 2;
    break;
    case 2:
      Serial.println("Estado 2");
      //comandos
      identificaCor();

      if(azul){
        estado = 3;
      }
      else if(verde){
        estado = 4;
      }
      else if(vermelho){
        estado = 5;
      }
      else{
        estado = 6;
      }

      //movimento dos motores - trás
      tras(300);

    break;
    case 3:
      Serial.println("Estado 3");
      //comandos
      //movimento dos motores - meia volta
      giro(800, 1);
      
      estado = 0; 
    break;
    case 4:
      Serial.println("Estado 4");
      //comandos
      //movimento dos motores - desvio pela esquerda
      //Giro para esquerda
      giro(300, 1);
      //Frente
      frente(600);
      //Giro para direita
      giro(300, 0);
      //Frente
      frente(600);
      //Giro para direita
      giro(300, 0);
      //Frente
      frente(600);
      //Giro para esquerda
      giro(300, 1);

      estado = 0;
    break;
    case 5:
      Serial.println("Estado 5");
      //comandos
      //movimento dos motores - desvio pela direita
      //Giro para direita
      giro(300, 0);
      //Frente
      frente(600);
      //Giro para esquerda
      giro(300, 1);
      //Frente
      frente(600);
      //Giro para esquerda
      giro(300, 1);
      //Frente
      frente(600);
      //Giro para direita
      giro(300, 1);

      estado = 0;
    break;
    case 6:
      Serial.println("Estado 6");
      //comandos
      // movimento dos motores - parar
      pare(100, 0);
    break;
  }
}

void identificaLinha(){
  //fazendo a leitura dos sensores de linha
  leituraesquerda = digitalRead(sensoresquerda);
  leituradireita = digitalRead(sensordireita);
  Serial.println("oleoleoleole");
}
void segueLinha(){
  Serial.print("Sensor esquerda: ");
  Serial.println(leituraesquerda);
  Serial.print("Sensor direita: ");
  Serial.println(leituradireita);


  if (leituradireita == 0 && leituraesquerda == 1) { //inverter em 1 && 0 para robo 2
    //Robô vira para a direita
    //sentido de giro do motor 1
    //invertendo o sentido de rotação do motor 1
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    analogWrite(EN_A, vel_min);

    //sentido de giro do motor 2
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
    analogWrite(EN_B, vel_min);
    delay(tempo);
  }

  else if (leituradireita == 1 && leituraesquerda == 0) { //inverter em 0 && 1 para robo 2
    //Robô vira para a esquerda
    //sentido de giro do motor 1
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    analogWrite(EN_A, vel_min);

    //sentido de giro do motor 2
    //invertendo o sentido re rotação do motor 2
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
    analogWrite(EN_B, vel_min);
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
void identificaCor(){
  Serial.println("UUUIIIIIIIIIIII");
  //Função que faz a leitura das faixas de cores
  //Vermelho
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);
  valorVermelho = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH); //A função pulseIn mede a duração do pulso
  Serial.print("Faixa de vermelho: ");
  Serial.println(valorVermelho);
  
  //Sem filtro
  digitalWrite(pinS2, HIGH);
  valorBranco = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
  Serial.print("Faixa de branco: ");
  Serial.println(valorBranco);

  //Azul
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, HIGH);
  valorAzul = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
  Serial.print("Faixa de Azul: ");
  Serial.println(valorAzul);

  //Verde
  digitalWrite(pinS2, HIGH);
  valorVerde = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
  Serial.print("Faixa de Verde: ");
  Serial.println(valorVerde);
  
  //Algoritimo que detecta a cor lida pelo sensor de cor
  //Verifica se a cor vermelha foi detectada
  if ((valorVermelho < valorAzul) &&
      (valorVermelho < valorVerde) &&
      (valorBranco < 100)) {
    Serial.println("Vermelho");
    azul = 0; 
    verde = 0;
    vermelho = 1;
    branco = 0;
    

  } else if ((valorAzul < valorVermelho) &&  //Verifica se a cor azul foi detectada
             (valorAzul < valorVerde) &&
             (valorBranco < 100)) {
    Serial.println("Azul");
    azul = 1; 
    verde = 0;
    vermelho = 0;
    branco = 0;

  } else if ((valorVerde < valorVermelho) &&  //Verifica se a cor verde foi detectada
             (valorVerde < valorAzul) &&
             (valorBranco < 100)) {
    Serial.println("Verde");
    azul = 0; 
    verde = 1;
    vermelho = 0;
    branco = 0;
  }
  else{
    azul = 0; 
    verde = 0;
    vermelho = 0;
    branco = 1;
  }
}

void identificaObstaculo(){
  // Envia pulso para o disparar o sensor
  DisparaPulsoUltrassonico();
  // Mede o tempo de duração do sinal no pino de leitura(us)
  tempoEcho = pulseIn(PinEcho, HIGH);
  distancia = CalculaDistancia(tempoEcho);
  Serial.println("Distancia em metros: ");
  Serial.println(distancia);
  Serial.println("Distancia em centimetros: ");
  Serial.println(distancia*100);
  

  if((distancia*100) <= distminima){
    obstaculo = 1;
  }else{
    obstaculo = 0;
  }

}

// Funçao para enviar o pulso de trigger
void DisparaPulsoUltrassonico(){
  // Para fazer o HC-SR04 enviar um pulso ultrassonico, nos temos
  // que enviar para o pino de trigger um sinal de nivel alto
  // com pelo menos 10us de duraçao
  digitalWrite(PinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(PinTrigger, LOW);
}

// Função para calcular a distancia em metros
float CalculaDistancia(float tempo_us){
  return((tempo_us*VelocidadeSom_mporus)/2);
}

void giro(int t, int sentido){
  //Robô vira no sentido antiorário se sentido = 1
  if(sentido){
    //sentido de giro do motor 1
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    analogWrite(EN_A, vel_motor1);

    //sentido de giro do motor 2
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
    analogWrite(EN_B, vel_motor2);
    delay(t);
  }
  else{
    //sentido de giro do motor 1
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    analogWrite(EN_A, vel_motor1);

    //sentido de giro do motor 2
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
    analogWrite(EN_B, vel_motor2);
    delay(t);
  }
}

void frente(int t){
  //sentido de giro do motor 1
    digitalWrite(IN_1, HIGH);
    digitalWrite(IN_2, LOW);
    analogWrite(EN_A, vel_motor1);

    //sentido de giro do motor 2
    digitalWrite(IN_3, HIGH);
    digitalWrite(IN_4, LOW);
    analogWrite(EN_B, vel_motor2);
    delay(t);
}

void tras(int t){
  //sentido de giro do motor 1
    digitalWrite(IN_1, LOW);
    digitalWrite(IN_2, HIGH);
    analogWrite(EN_A, vel_motor1);

    //sentido de giro do motor 2
    digitalWrite(IN_3, LOW);
    digitalWrite(IN_4, HIGH);
    analogWrite(EN_B, vel_motor2);
    delay(t);
}

void pare(int t, int sempre){
  while(sempre){
    analogWrite(EN_A, LOW);
    analogWrite(EN_B, LOW);
  }
  analogWrite(EN_A, LOW);
  analogWrite(EN_B, LOW);
  delay(t);
}
