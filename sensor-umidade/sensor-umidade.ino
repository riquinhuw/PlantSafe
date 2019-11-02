
const int pinoSensor = A0; //PINO UTILIZADO PELO SENSOR
int valorLido; //VARIÁVEL QUE ARMAZENA O PERCENTUAL DE UMIDADE DO SOLO

int analogSoloSeco = 1023; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado = 380; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)
 
void setup(){
 Serial.begin(9600); //INICIALIZA A SERIAL
 pinMode(7, OUTPUT);
 Serial.println("Lendo a umidade do solo..."); //IMPRIME O TEXTO NO MONITOR SERIAL
 delay(2000); //INTERVALO DE 2 SEGUNDOS
}
 
void loop(){  
  
  
  if(medirUmidade()<45){
    Serial.println("Umidade abaixo do limite configurado, regando");
    Serial.println("Abrindo a corrente");
    digitalWrite(7,HIGH);
    delay(2000);
    Serial.println("Fechando a corrente");
    digitalWrite(7,LOW);
    }
  delay(1000);  //INTERVALO DE 1 SEGUNDO
}

int medirUmidade(){
 valorLido = constrain(analogRead(pinoSensor),analogSoloMolhado,analogSoloSeco); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
 valorLido = map(valorLido,analogSoloMolhado,analogSoloSeco,percSoloMolhado,percSoloSeco); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS
 Serial.print("Umidade do solo: "); //IMPRIME O TEXTO NO MONITOR SERIAL
 Serial.print(valorLido); //IMPRIME NO MONITOR SERIAL O PERCENTUAL DE UMIDADE DO SOLO
 Serial.println("%"); //IMPRIME O CARACTERE NO MONITOR SERIAL
 return valorLido;
  
  }
