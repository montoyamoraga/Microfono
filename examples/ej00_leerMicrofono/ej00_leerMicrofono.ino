// leer un microfono electret y mostrar en el monitor serial
// la amplitud del sonido, en formato para el plotter serial

// conexiones:
// modulo del microfono: VCC a 5V o 3.3V, GND a tierra (GND),
// salida (OUT) a A0

// incluir biblioteca
#include "Microfono.h"

// crear un microfono en la patita A0
Microfono microfono(A0);

void setup()
{
  // abrir comunicacion serial
  Serial.begin(9600);
}

void loop()
{
  // escuchar durante 50 milisegundos
  microfono.leer();

  // imprimir amplitud pico a pico,
  // en Arduino Uno de 0 en silencio hasta 1023
  Serial.println(microfono.getAmplitud());
}
