#include "SensorColor.h"
// llama a la clase SensorColor

// Constructor
SensorColor::SensorColor() {}

// Destructor
SensorColor::~SensorColor() {}


// Configura los pines del sensor
void SensorColor::configurar() {
  pinMode(patitaS0, OUTPUT);
  pinMode(patitaS1, OUTPUT);
  pinMode(patitaS2, OUTPUT);
  pinMode(patitaS3, OUTPUT);
  pinMode(patitaSalida, INPUT);   // salida de frecuencia
  pinMode(patitaActivar, OUTPUT);

  // Escala de frecuencia al 20% para mejor estabilidad
  // De esta manera no es tan sensible y precibe los colores que nosotros queremos
  digitalWrite(patitaS0, HIGH);
  digitalWrite(patitaS1, LOW);

  // Activar sensor (LOW = activo)
  digitalWrite(patitaActivar, LOW);
}

// Lee los tres colores básicos del sensor con cada fotodiódo
void SensorColor::leer() {
  lecturaRojo = leerRojo();
  lecturaVerde = leerVerde();
  lecturaAzul = leerAzul();
}

// Cada color se obtiene filtrando el fotodiodo correspondiente
int SensorColor::leerRojo() {
  digitalWrite(patitaS2, LOW);
  digitalWrite(patitaS3, LOW);
  int valor = pulseIn(patitaSalida, LOW);
  delay(tiempoPausa);
  return normalizar(valor, 2000);
}

int SensorColor::leerVerde() {
  digitalWrite(patitaS2, HIGH);
  digitalWrite(patitaS3, HIGH);
  int valor = pulseIn(patitaSalida, LOW);
  delay(tiempoPausa);
  return normalizar(valor, 2000);
}

int SensorColor::leerAzul() {
  digitalWrite(patitaS2, LOW);
  digitalWrite(patitaS3, HIGH);
  int valor = pulseIn(patitaSalida, LOW);
  delay(tiempoPausa);
  return normalizar(valor, 2000);
}

// Escala los valores de frecuencia a un rango manejable (0–50)
int SensorColor::normalizar(int valor, int maximoEntrada) {
  if (valor > maximoEntrada) valor = maximoEntrada;
  return (valor * 50) / maximoEntrada;
}

// Compara si un valor está cerca de otro dentro de tolerancia
// Es medianamente laxo a la hora de aceptar colores correctos
bool SensorColor::cercaDe(int valor, int objetivo) {
  return abs(valor - objetivo) <= 0; // tolerancia ±1
}

// Determina el color actual comparando patrones RGB
void SensorColor::determinarColor() {
// Los valores son resultado de una calibración en las condiciones reales
// Dentro del conducto donde salen los dulces
  if (cercaDe(lecturaRojo, 12) && cercaDe(lecturaVerde, 7) && cercaDe(lecturaAzul, 8)) {
    colorDetectado = "ROJO";
    // String resultado de detectar rojo activa los escenarios correspondientes
    obtenerColor = 1;
  } 
  else if (cercaDe(lecturaRojo, 7) && cercaDe(lecturaVerde, 5) && cercaDe(lecturaAzul, 4)) {
    colorDetectado = "AZUL";
    // String resultado de detectar azul activa los escenarios correspondientes
    obtenerColor = 2;
  } 
  else if (cercaDe(lecturaRojo, 11) && cercaDe(lecturaVerde, 6) && cercaDe(lecturaAzul, 7)) {
    colorDetectado = "NARANJO";
    // String resultado de detectar naranjo activa los escenarios correspondientes
    obtenerColor = 3;
  } 
  else if (cercaDe(lecturaRojo, 10) && cercaDe(lecturaVerde, 4) && cercaDe(lecturaAzul, 6)) {
    colorDetectado = "VERDE";
    // String resultado de detectar verde activa los escenarios correspondientes
    obtenerColor = 4;
  } 
  else {
    colorDetectado = "NADA"; 
    // sin coincidencia clara
    obtenerColor = 0;
  }
}

// Muestra los valores leídos y el color detectado por Serial
void SensorColor::mostrarSerial() {
  Serial.print("Rojo = ");
  Serial.print(lecturaRojo);
  Serial.print("  Verde = ");
  Serial.print(lecturaVerde);
  Serial.print("  Azul = ");
  Serial.print(lecturaAzul);
  Serial.print("  -->  ");

  if(colorDetectado != "NADA"){
    Serial.println("Detecto + colorDetectado");
  } else {
    Serial.println("No detecto color");
  }
}