// medir el ruido del microfono en silencio al partir,
// y despues mostrar la amplitud sin ese ruido

// conexiones:
// modulo del microfono: VCC a 3V3, GND a tierra (GND),
// salida (OUT) a gpio 26
// no conectes el modulo a 5V (VBUS o VSYS), solo a 3V3

#include "Microfono.h"

#include <stdio.h>

#include "pico/stdlib.h"

int main()
{
    // abrir comunicacion serial por usb
    stdio_init_all();

    // crear un microfono en gpio 26 (entrada 0 del adc)
    Microfono microfono(26);

    // escuchar 1 segundo en silencio y guardar el ruido.
    // no hagas ruido mientras tanto
    microfono.calibrar();

    // para usar un valor medido antes, sin calibrar:
    // microfono.setRuido(20);

    while (true)
    {
        // escuchar durante 50 milisegundos
        microfono.leer();

        // imprimir ruido y amplitud.
        // en silencio la amplitud queda cerca de 0
        printf("ruido:%u,amplitud:%u\n", microfono.getRuido(), microfono.getAmplitud());
    }
}
