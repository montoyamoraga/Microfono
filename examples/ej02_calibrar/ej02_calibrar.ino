// medir el ruido del microfono en silencio al partir,
// y despues mostrar la amplitud sin ese ruido

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
  // en silencio la amplitud queda cerca de 0
  Serial.print("ruido:");
  Serial.print(microfono.getRuido());
  Serial.print(",amplitud:");
  Serial.println(microfono.getAmplitud());
}
