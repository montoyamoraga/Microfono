// medir el ruido del microfono en silencio al partir,
// y despues mostrar la amplitud sin ese ruido

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

  // escuchar 1 segundo en silencio y guardar el ruido.
  // no hagas ruido mientras tanto
  microfono.calibrar();

  // para usar un valor medido antes, sin calibrar:
  // microfono.setRuido(20);
}

void loop()
{
  // escuchar durante 50 milisegundos
  microfono.leer();

  // imprimir ruido y amplitud, en formato para el plotter serial.
  // en silencio la amplitud queda en 0
  Serial.print("ruido:");
  Serial.print(microfono.getRuido());
  Serial.print(",amplitud:");
  Serial.println(microfono.getAmplitud());
}
