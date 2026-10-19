// leer un microfono electret sin bloquear el programa,
// mientras el led de la placa parpadea a su propio ritmo

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

    // led de la placa
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // crear un microfono en gpio 26 (entrada 0 del adc)
    Microfono microfono(26);

    // para el parpadeo del led
    uint32_t ultimoCambioLed = 0;
    bool ledEncendido = false;

    while (true)
    {
        // tomar una muestra. devuelve true cuando
        // termina una ventana de 50 milisegundos
        if (microfono.escuchar())
        {
            printf("%u\n", microfono.getAmplitud());
        }

        // el led sigue parpadeando, porque escuchar() no bloquea
        uint32_t ahora = to_ms_since_boot(get_absolute_time());
        if (ahora - ultimoCambioLed >= 500)
        {
            ultimoCambioLed = ahora;
            ledEncendido = !ledEncendido;
            gpio_put(PICO_DEFAULT_LED_PIN, ledEncendido);
        }
    }
}
