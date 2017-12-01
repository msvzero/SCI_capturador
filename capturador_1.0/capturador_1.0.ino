#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>

const int LDRPin = 0;//Analogico

EthernetClient cliente;
/*Configuraciones de red */
const char* servidor = "192.168.1.101";
const unsigned long BOUD_RATE = 9600; // velocidad de conexion serial

/*Configuracion del dispositivo capturador*/
const String dispositivoId = "5a1c8f1aa2b6bc067e5618e9";

void setup() {
  pinMode(LDRPin, INPUT);
  iniciarSerial();
  iniciarEthernet();

}
void loop() {
  if (conexion(servidor)) {
    int iluminacion = analogRead(LDRPin);
    Serial.println(iluminacion);
    peticionPut(iluminacion, dispositivoId, servidor );
  }
  desconexion();
  espera();


}

/*----------------------------------------------------*/
//Realizar una peticion HTTP POST
void peticionPut(int iluminacion, String dispositivoId, char* servidor ) {
  String uri = "/actuadores/" + dispositivoId + "/" + String(iluminacion);
  Serial.println(uri);
  Serial.print("PUT ");
  Serial.println(uri);

  cliente.print("PUT ");
  cliente.print(uri);
  cliente.println(" HTTP/1.0");
  cliente.print("Host: ");
  cliente.println(servidor);
  cliente.println("Connection: close");
  cliente.println();
}
//Inicializa la comunicacion serial
void iniciarSerial() {
  Serial.begin(BOUD_RATE);
  while (!Serial) {
    // espera que se inicialize el puerto serial.
  }
  Serial.println("Comunicacion serial lista");
}
//Conectarse a la red ethernet
void iniciarEthernet() {
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  if (!Ethernet.begin(mac)) {
    Serial.println("Fallo de configuracion ethernet");
    return;
  }
  Serial.println("Ethernet listo");
  delay(1000);
}
//Conectarse a un servidor HTTP
boolean conexion(const char* servidor) {
  Serial.print("Conectandose a ");
  Serial.print(servidor);
  boolean ok =  cliente.connect(servidor, 3000);
  if (ok) {
    Serial.print("Conectado a");
    Serial.println(servidor);
  } else {
    Serial.print("Conexion fallida");
  }
  return ok;
}
//Desconectarse del servidor
void desconexion() {
  Serial.println("Desconectandose");
  cliente.stop();
}

//Pause antes de conectarse nuevamente y realizar la siguiente peticion.
void espera() {
  Serial.println("Esperando 5 segundos");
  delay(5000);
}


