/*
ELETRICA PINHO 

EM BREVE SERÁ ATUALIZADO 
NO APLICATIVO ESTA DISPONIVEL GOOGLE








*/



#include <ESP8266WiFi.h>


//Nome da sua rede Wifi
const char* ssid = "SUA REDE";

//Senha da rede
const char* password = "SUA SENHA ";

//IP do ESP (para voce acessar pelo browser)
IPAddress ip(192, 168, 88, 175);

//IP do roteador da sua rede wifi
IPAddress gateway(192, 168, 88, 1);

//Mascara de rede da sua rede wifi
IPAddress subnet(255, 255, 255, 0);

//Criando o servidor web na porta 80
WiFiServer server(8090);


int const  rele1 = 0;
int const  rele2 = 13;
int const botao1 = 2;
int const botao2 = 16;
int estado1 = 0;
int estado2 = 0;
String statusled;

//prototipo de funcao 

void funcaodelay();

//Funcao que sera executada apenas ao ligar o ESP8266
void setup() {
  //Preparando o pino, que esta lidago ao rele
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);

  digitalWrite(rele1, LOW);
  digitalWrite(rele2, LOW);
  //Conectando a rede Wifi
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);


  //Verificando se esta conectado,
  //caso contrario, espera um pouco e verifica de novo.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //Iniciando o servidor Web
  server.begin();
}

//Funcao que sera executada indefinidamente enquanto o NodeMCU estiver ligado.
void loop() {



  estado1 = digitalRead(botao1);
  estado2 = digitalRead(botao2);

  funcaodelay();

  if (estado1 == LOW) {
    digitalWrite(rele1, !digitalRead(rele1));
  }

  funcaodelay();
  if (estado2 == LOW) {
    digitalWrite(rele2, !digitalRead(rele2));
  }








  //Verificando se o servidor esta pronto.
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  //Verificando se o servidor recebeu alguma requisicao
  while (!client.available()) {
    delay(1);
  }

  //Obtendo a requisicao vinda do browser
  String req = client.readStringUntil('\r');

  
  if (req == "GET /favicon.ico HTTP/1.1") {
    req = client.readStringUntil('\r');
  }

  client.flush();

  //Iniciando o buffer que ira conter a pagina HTML que sera enviada para o browser.
  String buf = "";

  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  // buf += "<meta http-equiv='refresh' content='5'/>";

  // buf += "<head> ";

  buf += "<meta charset='UTF-8'> ";


  if (digitalRead(rele1)) {
    statusled = "ligado";
  } else {
    statusled = "Desligado";
  }

  buf += "<a href='?f=rele1' class='btn btn-sea'><button>RELE1 " + statusled + "</button></a> ";


  if (digitalRead(rele2)) {
    statusled = "ligado";
  } else {
    statusled = "Desligado";
  }


  buf += "<a href='?f=rele2' class='btn btn-sea'><button>RELE2 " + statusled + "</button></a> ";

  buf += "</body> ";
  buf += "</html> ";

  //Enviando para o browser a 'pagina' criada.
  client.print(buf);
  client.flush();





  if (req.indexOf("rele1") > 0 ) {
    digitalWrite(rele1, !digitalRead(rele1));
  }

  if (req.indexOf("rele2") > 0 ) {
    digitalWrite(rele2, !digitalRead(rele2));
  }



}




void funcaodelay() {

  delay(100);

}
