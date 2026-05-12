#ifndef SENSOR_COLOR_H
#define SENSOR_COLOR_H

//  Clase: SensorColor
//  Sensor de color TCS230 / TCS3200
//  Permite leer RGB y determinar un color definido

#include <Arduino.h>

class SensorColor {

public:
  // Constructor y destructor
  SensorColor();
  ~SensorColor();

  // Configuración inicial del sensor 
  void configurar();

  // Lecturas 
  void leer();
  int leerRojo();
  int leerVerde();
  int leerAzul();

  // Utilidades 
  int normalizar(int valor, int maximoEntrada);
  bool cercaDe(int valor, int objetivo);

  // Lógica de detección 
  void determinarColor();
  void mostrarSerial();

  // Variables internas 
  int lecturaRojo = 0;
  int lecturaVerde = 0;
  int lecturaAzul = 0;

  int tiempoPausa = 90; 
  // retardo entre lecturas (ms)

  // Pines del sensor
  int patitaS0 = 2;
  int patitaS1 = 3;
  int patitaS2 = 4;
  int patitaS3 = 5;
  int patitaSalida = 6;
  int patitaActivar = 7;

  // Color detectado (texto)
  String colorDetectado = "NADA";

  // Identificador numérico opcional
  int obtenerColor;
};

#endif