// leer un microfono electret sin bloquear el programa,
// mientras el led de la placa parpadea a su propio ritmo

// conexiones:
// modulo del microfono: VCC a 5V o 3.3V, GND a tierra (GND),
// salida (OUT) a A0

// incluir biblioteca
#include "Microfono.h"

// crear un microfono en la patita A0
Microfono microfono(A0);

// para el parpadeo del led
unsigned long ultimoCambioLed = 0;
bool ledEncendido = false;

void setup()
{
  // abrir comunicacion serial
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // tomar una muestra. devuelve true cuando
  // termina una ventana de 50 milisegundos
  if (microfono.escuchar())
  {
    Serial.println(microfono.getAmplitud());
  }

  // el led sigue parpadeando, porque escuchar() no bloquea
  if (millis() - ultimoCambioLed >= 500)
  {
    ultimoCambioLed = millis();
    ledEncendido = !ledEncendido;
    digitalWrite(LED_BUILTIN, ledEncendido);
  }
}
