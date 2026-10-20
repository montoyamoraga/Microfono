#include "../src/Hardware.h"

#include "hardware/adc.h"
#include "pico/stdlib.h"

namespace MicrofonoHardware
{

    // la ultima entrada del adc es el sensor de temperatura,
    // las anteriores son gpio desde ADC_BASE_PIN
    static bool esPatitaAnaloga(uint8_t patita)
    {
        return patita >= ADC_BASE_PIN &&
               patita < ADC_BASE_PIN + NUM_ADC_CHANNELS - 1;
    }

    void configurarEntradaAnaloga(uint8_t patita)
    {
        // iniciar el adc solo una vez,
        // aunque haya varios microfonos
        static bool adcIniciado = false;
        if (!adcIniciado)
        {
            adc_init();

#ifdef PICO_SMPS_MODE_PIN
            // poner el regulador de la placa en modo pwm,
            // hace menos ruido en el adc. en pico y pico 2 es gpio 23.
            // pico w no tiene esta patita, ahi se omite
            gpio_init(PICO_SMPS_MODE_PIN);
            gpio_set_dir(PICO_SMPS_MODE_PIN, GPIO_OUT);
            gpio_put(PICO_SMPS_MODE_PIN, 1);
#endif

            adcIniciado = true;
        }

        if (esPatitaAnaloga(patita))
        {
            adc_gpio_init(patita);
        }
    }

    uint16_t leerPatita(uint8_t patita)
    {
        // una patita sin adc se lee como silencio
        if (!esPatitaAnaloga(patita))
        {
            return 0;
        }

        // en pico la patita es un gpio con adc,
        // por ejemplo gpio 26 es la entrada 0
        adc_select_input(patita - ADC_BASE_PIN);
        return adc_read();
    }

    uint32_t milisegundos()
    {
        return to_ms_since_boot(get_absolute_time());
    }

}
