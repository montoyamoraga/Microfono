#include "./../Hardware.h"

#include <Arduino.h>

namespace MicrofonoHardware
{

    void configurarEntradaAnaloga(uint8_t patita)
    {
        // en Arduino analogRead no necesita configuracion previa
        (void)patita;
    }

    uint16_t leerPatita(uint8_t patita)
    {
        return analogRead(patita);
    }

    uint32_t milisegundos()
    {
        return millis();
    }
}
