#include <time.h>
#include <WiFi.h>
#include <TimeLib.h>
#include <TimeAlarms.h>
#include <PubSubClient.h>

int wday, hours, minutes, seconds;

//MQTT
const char* mqttServer = "mqtt.eclipseprojects.io";
const int mqttPort = 1883;
const char* clientId = "ESP32_Client";
const char* topic = "signal/control";
const int ledPin = 15;  // Pino do LED

//wifi
const char* ssid     = "iPhone de Marcelo";
const char* password = "senhaforte11";


//variaveis de tempo
long timezone = -3;
byte daysavetime = 1;


WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);

  connectWifi();
 
  configServerTime();

  configESPTime();

  conectarMQTT();


  Alarm.alarmRepeat(dowMonday, 10, 19, 0, sendSignal);
  Alarm.alarmRepeat(dowMonday, 10, 20, 0, sendSignal);

  //Configuração de pinos
  pinMode(ledPin, OUTPUT);
}

void loop() {

  //imprimir o horário no Serial
  struct tm tmstruct ;
  tmstruct.tm_year = 0;
  getLocalTime(&tmstruct);
  String date = (String((tmstruct.tm_year) + 1900) + "-" + String(( tmstruct.tm_mon) + 1) + "-" + String(tmstruct.tm_mday) + "-" + String(tmstruct.tm_wday));
  String hour = (String(tmstruct.tm_hour) + ":" + String(tmstruct.tm_min) + ":" + String(tmstruct.tm_sec));

  Serial.println("Date: " + date + " - Time: " + hour);

  delay(1000);

  Alarm.delay(0);

  if (!client.connected()) {
    reconectarMQTT();
  }
  client.loop();
}

void sendSignal(){
  Serial.println("Sinal");
  digitalWrite(15,HIGH);
  delay(2000);
  digitalWrite(15,LOW);
}

void receiveSchedule(){

}

void connectWifi(){
   // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();

  //Conexao WIFI
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int i = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20) {
      ESP.restart();
    }
    i++;
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void configServerTime(){
  //Conexão com o servidor de tempo
  Serial.println("Contacting Time Server");
  configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
}

void configESPTime(){
  struct tm tmstruct ;
  tmstruct.tm_year = 0;
  getLocalTime(&tmstruct);
  //Configurar o horário do ESP
  setTime(tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec, tmstruct.tm_mday, tmstruct.tm_mon, (tmstruct.tm_year) + 1900);
}

void conectarMQTT() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Conectando ao servidor MQTT...");
    if (client.connect(clientId)) {
      Serial.println("Conectado ao servidor MQTT");
      client.subscribe(topic);
    } else {
      Serial.println("Falha na conexão MQTT. Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void reconectarMQTT() {
  while (!client.connected()) {
    Serial.println("Tentando se reconectar ao servidor MQTT...");
    if (client.connect(clientId)) {
      Serial.println("Reconectado ao servidor MQTT");
      client.subscribe(topic);
    } else {
      Serial.println("Falha na reconexão. Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void callback(char* receivedTopic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Mensagem recebida do tópico ");
  Serial.print(receivedTopic);
  Serial.print(": ");
  Serial.println(message);

  if (message == "sinal") {
    digitalWrite(ledPin, HIGH);
    delay(2000);
    digitalWrite(ledPin, LOW);
  } else {
   

    //Converte a String em um array de char
    char charArray[message.length() + 1];
    message.toCharArray(charArray, sizeof(charArray));

    // Primeira chamada do strtok para obter o primeiro número
    char *token = strtok(charArray, "-");
    
    

    // Converte e atribui os números às variáveis
    wday = atoi(token);
    token = strtok(NULL, "-");
    hours = atoi(token);
    token = strtok(NULL, "-");
    minutes = atoi(token);
    token = strtok(NULL, "-");
    seconds = atoi(token);


    if(wday == 0){
      Alarm.alarmRepeat(dowSunday, hours, minutes, seconds, sendSignal);
    } else if(wday == 1){
      Alarm.alarmRepeat(dowMonday, hours, minutes, seconds, sendSignal);
    } else if(wday == 2){
      Alarm.alarmRepeat(dowTuesday, hours, minutes, seconds, sendSignal);
    } else if(wday == 3){
      Alarm.alarmRepeat(dowWednesday, hours, minutes, seconds, sendSignal);
    }else if(wday == 4){
      Alarm.alarmRepeat(dowThursday, hours, minutes, seconds, sendSignal);
    } else if(wday == 5){
      Alarm.alarmRepeat(dowFriday, hours, minutes, seconds, sendSignal);
    } else if(wday == 6){
      Alarm.alarmRepeat(dowSaturday, hours, minutes, seconds, sendSignal);
    } 
  }
}