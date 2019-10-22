////////////////////////////////////////////////////////////////////////////////////
// Este programa controla um Arduino com um motor de passo, sensores e um botão.  //
// Para obter os dados abra a macro do EXCEL na porta em que o Arduino estiver    //
// conectado.                                                                     //
// Autor: Matheus J. Castro                                                       //
// Versão: 3.2                                                                    //
// Última modificação: 01/12/2018                                                 //
// Repositório Oficial: https://github.com/MatheusJCastro/Arduino_Code_for_LAB_IV //
////////////////////////////////////////////////////////////////////////////////////

#include <Stepper.h> //biblioteca do motor de passo

#define botao 5 //pino do interrruptor
#define sensor A1 //pino do sensor

const float num_medidas = 64; //apenas numeros em potencia de 2 (64 = 2^6);
const int delaymed = 100; //delay de cada medida

const int sPR = 2048; //numero de passos por revolução (apenas potencia de dois)
Stepper myStepper(sPR, 8,10,9,11); //define o numero de passos e os pinos do motor

const double passos_por_medida = (sPR)/(2*num_medidas); //numero de passos que devem ser movidos para cada medida

void setup() 
{ 
  Serial.begin(9600); //inicia a serial
  pinMode(botao,INPUT); //define o pino do interruptor como entrada
} 
  
void loop() 
{ 
  if (digitalRead(botao) == HIGH) //se o interruptor é pressionado, inicia as medições
  { 
    Serial.println("CLEARSHEET");
    Serial.println("CLEARDATA");
    Serial.println("LABEL,Angle (graus),Voltage (V)");
    
    double grau_atual = 0;
      
    for (int x = 0; x < num_medidas; x++) //inicia o conjunto de medidas
    {
      double medida = 0;
      double med_ant = analogRead(sensor); //faz a primeira medida de referencia
      for (int repeat = 0; repeat < 100; repeat++) //faz a media entre 100 medidas
      {
        medida = analogRead(sensor); //le o sensor
        medida = (medida + med_ant)/2;
        med_ant = medida;
        delay(5);
      }
      medida = (5*medida)/1023; //transforma o nº de 10 bits para 0-5 volts
      
      Serial.print("DATA,");
      Serial.print(grau_atual);
      Serial.print(",");
      Serial.println(medida);
            
      grau_atual = grau_atual + 180/num_medidas; //define qual o grau da proxima medida
      myStepper.setSpeed(3); //define a velocidade
      myStepper.step(passos_por_medida); //move o numero de passos para a proxima medida
      delay(delaymed); //aguarda um tempo
    }

    delay(delaymed);
    myStepper.setSpeed(8);
    myStepper.step(sPR/2 + passos_por_medida); //anda 180° para reiniciar o proximo conjunto de medidas
    delay(delaymed);
  }
}
