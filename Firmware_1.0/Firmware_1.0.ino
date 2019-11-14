/*
 * Projeto PlantSafe, com foco de coletar humidade do solo e regar se necessario.
 * Salvando a informação no banco, para que o usuario possa ver onde estiver.
 * 
 * 
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

 /* Variaveis para internet  */

const char* ssid = "SEU WIFI";
const char* password =  "SENHA DO WIFI";


 /* Variaveis para Sensor de humidade  */

int    umidade =0;
int percentUmidade = 35;
int rele=32;
int led=26;
const int pinoSensor = 35; //PINO UTILIZADO PELO SENSOR
int valorLido; //VARIÁVEL QUE ARMAZENA O PERCENTUAL DE UMIDADE DO SOLO
int analogSoloSeco = 4095; //VALOR MEDIDO COM O SOLO SECO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int analogSoloMolhado = 1800; //VALOR MEDIDO COM O SOLO MOLHADO (VOCÊ PODE FAZER TESTES E AJUSTAR ESTE VALOR)
int percSoloSeco = 0; //MENOR PERCENTUAL DO SOLO SECO (0% - NÃO ALTERAR)
int percSoloMolhado = 100; //MAIOR PERCENTUAL DO SOLO MOLHADO (100% - NÃO ALTERAR)

 /* Variaveis auxiliares  */
 int contadorDeVerificacao=0;



int medirUmidade(){
 valorLido = constrain(analogRead(pinoSensor),analogSoloMolhado,analogSoloSeco); //MANTÉM valorLido DENTRO DO INTERVALO (ENTRE analogSoloMolhado E analogSoloSeco)
 valorLido = map(valorLido,analogSoloMolhado,analogSoloSeco,percSoloMolhado,percSoloSeco); //EXECUTA A FUNÇÃO "map" DE ACORDO COM OS PARÂMETROS PASSADOS
 return valorLido;
  }
void escreverUmidade(int valor){
   Serial.print("Umidade do solo: "); //IMPRIME O TEXTO NO MONITOR SERIAL
 Serial.print(valor); //IMPRIME NO MONITOR SERIAL O PERCENTUAL DE UMIDADE DO SOLO
 Serial.println("%"); //IMPRIME O CARACTERE NO MONITOR SERIAL
  }

 void liberarAgua(){
    Serial.println("Umidade abaixo do limite configurado, regando");
    Serial.println("Abrindo a corrente");
    digitalWrite(rele,HIGH); 
    delay(7000);// liberando água por 7 segundos
    Serial.println("Fechando a corrente");
    digitalWrite(rele,LOW);
    Serial.println("Esperando a água -parar-");
    delay(15000);//delay de 15s 
  }


void internetConectar(){
    Serial.println("tentand conectar");
    WiFi.begin(ssid, password); 
    // WiFi.localIP().toString() == "0.0.0.0" WiFi.status() != WL_CONNECTED
    Serial.println("passou wifi e senha");
    while (WiFi.status() != WL_CONNECTED) { //Check for the connection
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
   
    Serial.println("Connected to the WiFi network");
  }

void internetPost(int umidade,bool regada){
        //String var = "{\"name\":\"FROM\",\"email\":\"Ware@mail.com\"}"; // montando o Json
        const size_t CAPACITY = JSON_OBJECT_SIZE(2);
        StaticJsonDocument<CAPACITY> doc;
        JsonObject root = doc.to<JsonObject>();
        root["umidade"] = umidade;
        root["regada"] = regada;
        //serializeJson(doc, Serial);
        String enviar;
        serializeJson(doc, enviar);
        
     if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
     
       HTTPClient http;
       http.useHTTP10();
       http.setReuse(true); 
     
       http.begin("http://suaAPI.com/POST");  //Especificando o link/rota
       http.addHeader("Content-Type", "application/json");   //Especificando que é Json
     
       int httpResponseCode = http.POST(enviar);   //Envia o Json via POST
     
       if(httpResponseCode>0){
     
        String response = http.getString();    //Retorna o "feedback"
     
        Serial.println(httpResponseCode);   //Retorna o codigo de status
        Serial.println(response);           //Retorna o conteúdo
     
       }else{
     
        Serial.print("Erro ao enviar o POST: ");
        Serial.println(httpResponseCode);
        Serial.println(WiFi.localIP().toString());
     
       }
     
       http.end();  //Liberar os recursos
     }else{
     
        Serial.println("Erro na conexão WiFi");
        Serial.println(WiFi.status());
     }

    WiFi.disconnect();// Disconecto do wifi, porque fica dando erro -1 ( o serv recusa)
  }

  void intro(){
      Serial.println("Iniciando sistema Plant Safe 1.0");
      Serial.print("Carregando");
      int i =0;
      for(i=0;i<11;i++){Serial.print(".");delay(500);}
      Serial.println("\nPlant Safe carregado!");
      
      
    
    }

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(rele, OUTPUT);
  intro();

}

void loop() {
     umidade = medirUmidade();
  if(umidade<percentUmidade){
    escreverUmidade(umidade);
    liberarAgua();
    internetConectar();
    internetPost(umidade,true);
    }else{
      escreverUmidade(umidade);     
      contadorDeVerificacao++;

      if(contadorDeVerificacao>30){//depois de 5 minutos ele vai enviar para o banco a informação
          Serial.println("O contador está em: "+contadorDeVerificacao);
          Serial.println("vai conectar");
         internetConectar();
         Serial.println("vai enviar ");
         internetPost(umidade,false);//falta enviar a temp(bonus) e se foi regado ou N em forma de bool
         contadorDeVerificacao =0;
         Serial.println("Agora em: "+contadorDeVerificacao);
        }
      }
  delay(10000); //A cada 10s ele verifica novamente
}
