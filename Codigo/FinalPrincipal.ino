//Código para arduino R4
//Controla salida de audio, reconocimiento de color y la señal que llega a R3 para controlar pantalla

// Bibliotecas externas
#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

// Clase propia
#include "SensorColor.h"

// Crear objetos
SensorColor sensorColor;
DFRobotDFPlayerMini myDFPlayer;

// Variables de control
const int pinSalida = 13;   
// Pin para activar señal hacia R3 (LED/entrada digital)

bool reproduciendo = false;                    
// indica si hay audio activo
unsigned long tiempoInicioReproduccion = 0;    
// tiempo inicial del audio
const unsigned long duracionReproduccion = 15000; 
// 15 segundos máximo de reproducción

// Control de colores
String colorDetectado = "NADA";
String ultimoColor = "NADA";

void setup() {
  pinMode(pinSalida, OUTPUT);
  digitalWrite(pinSalida, LOW); // empieza apagado
  Serial.begin(115200);
  Serial1.begin(9600);

  sensorColor.configurar();

  if (!myDFPlayer.begin(Serial1, true, true)) {
    Serial.println("Error al iniciar DFPlayer");
    //avisa que falla
  } else {
    Serial.println("DFPlayer listo");
    //avisa que no falla
  }

  myDFPlayer.volume(20);  
  // volumen entre 0 y 30 (15 = medio)
}

void loop() {
  // Leer color actual del sensor
  sensorColor.leer();
  sensorColor.determinarColor();
  sensorColor.mostrarSerial();

  String nuevoColor = sensorColor.colorDetectado;

  // Detecta constantemente hasta recibir un color válido
  //Si detecta un color válido y no hay audio sonando inicia reproducción
  //Tras recibir el color válido no aceptará mas hasta que termine la reproducción actual
  if (nuevoColor != "NADA" && !reproduciendo && nuevoColor != ultimoColor) {
    colorDetectado = nuevoColor;
    ultimoColor = nuevoColor;
    reproduciendo = true;
    tiempoInicioReproduccion = millis();
    
    digitalWrite(pinSalida, HIGH);  
    // Si llega este color válido 
    // Enciende pin 13 → señal al R3
    Serial.println("Audio iniciado");

    // Reproduce el audio correspondiente
    if (colorDetectado == "ROJO") {
      myDFPlayer.playFolder(1, 1);  
      // Carpeta 01, archivo 001.mp3
      // Respuesta enojada
      Serial.println("Reproduciendo ROJO");
    }
    else if (colorDetectado == "AZUL") {
      myDFPlayer.playFolder(2, 2);  
      // Carpeta 02, archivo 002.mp3
      // Respuesta triste
      Serial.println("Reproduciendo AZUL");
    }
    else if (colorDetectado == "NARANJO") {
      myDFPlayer.playFolder(3, 3);  
      // Carpeta 03, archivo 003.mp3
      // Respuesta loca
      Serial.println("Reproduciendo NARANJO");
    }
    else if (colorDetectado == "VERDE") {
      myDFPlayer.playFolder(4, 4);  
      // Carpeta 04, archivo 004.mp3
      // Respuesta feliz
      Serial.println("Reproduciendo VERDE");
    }
  }
  
  //Cada reproducción de audio se detiene a los 15 segundos no importa qué
  //Por seguridad
  if (reproduciendo && (millis() - tiempoInicioReproduccion > duracionReproduccion)) {
    myDFPlayer.stop();
    reproduciendo = false;
    colorDetectado = "NADA";
    ultimoColor = "NADA";
    digitalWrite(pinSalida, LOW);   
    // Apaga pin 13 y envía señal al R3
    Serial.println(" Audio detenido por tiempo límite");
  }

  // Si DFPlayer informa que terminó el audio por sí mismo
  //Escenario normal
  if (myDFPlayer.available()) {
    uint8_t tipo = myDFPlayer.readType();
    int valor = myDFPlayer.read();

    if (tipo == DFPlayerPlayFinished) {
      Serial.println(" Audio finalizado por DFPlayer");
      reproduciendo = false;
      colorDetectado = "NADA";
      ultimoColor = "NADA";
      digitalWrite(pinSalida, LOW); 
      // Apaga pin 13 → señal al R3
    }
  }
}