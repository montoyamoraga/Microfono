// leer un microfono electret y mostrar por usb
// la amplitud del sonido, en formato para el plotter serial

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

    while (true)
    {
        // escuchar durante 50 milisegundos
        microfono.leer();

        // imprimir amplitud pico a pico,
        // de 0 en silencio hasta 4095
        printf("%u\n", microfono.getAmplitud());
    }
}
