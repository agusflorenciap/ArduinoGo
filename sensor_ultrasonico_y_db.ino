#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <MySQL_Encrypt_Sha1.h>
#include <MySQL_Packet.h>

#include <SPI.h>
#include <Ethernet.h>

// Configuracion del Ethernet Shield
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFF, 0xEE}; // Direccion MAC
IPAddress ip(192,168,100,177); //IP del arduino
IPAddress server (192,168,100,107); // Direccion IP del servidor (VM)
EthernetClient client; 

//configuracion mysql
char user[] = "root";
char password[] = "hola";

//query

char INSERT_DATA[] = "INSERT INTO ultrasonico.distancia (distancia) VALUES (%d)";
char query[128];
char distancia;

MySQL_Connection conn((Client *)&client);

const int Trigger = 2;   //Pin digital 2 para el Trigger del sensor
const int Echo = 3;   //Pin digital 3 para el Echo del sensor

void setup() {
  Ethernet.begin(mac, ip); // Inicializamos el Ethernet Shield
  Serial.begin(9600);//iniciailzamos la comunicación
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
  Serial.print ("conectando...");
  if(conn.connect(server, 3306, user, password)){
    delay(1000); // Esperamos 1 segundo de cortesia
    Serial.println ("Conexion exitosa");
     MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save
    //dtostrf(50.125, 1, 1, distancia);
    //sprintf(query, INSERT_DATA, 0);
    // Execute the query
    cur_mem->execute(query);
    // Note: since there are no results, we do not need to read any data
    // Deleting the cursor also frees up memory used
    delete cur_mem;
    Serial.println("Data recorded.");
   
 }
  else
     Serial.println("Connection fallida");
  
}

void loop()
{
  long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros

  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  d = t/59;             //escalamos el tiempo a una distancia en cm
  
  Serial.print("Distancia: ");
  Serial.print(d);      //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();

// Inicializamos la query
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    
    //dtostrf(d, 1, 1, distancia);
    sprintf(query, INSERT_DATA, d);
    // Ejecutamos
    cur_mem->execute(query);
    delete cur_mem;
    
    delay(100);          //Hacemos una pausa de 100ms
}
