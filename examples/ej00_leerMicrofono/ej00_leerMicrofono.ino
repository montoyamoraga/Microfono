// leer un microfono electret
// mostrar en el monitor serial
// la amplitud del sonido
// en formato para plotter serial

// conexiones:
// modulo del microfono: VCC al voltaje de la placa, GND a tierra (GND),
// salida (OUT) a A0
// en Arduino Uno usa 5V. en placas de 3.3V (por ejemplo Nano 33 o ESP32)
// usa 3.3V, con 5V la salida puede danar la patita

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
