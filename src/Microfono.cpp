#include "Microfono.h"

#include "Hardware.h"

Microfono::Microfono(uint8_t nuevaPatita)
{
    patita = nuevaPatita;
    ventana = 50;
    minimo = 0;
    maximo = 0;

    // sin restar ruido hasta que se llame a calibrar() o setRuido()
    ruido = 0;

    ventanaAbierta = false;
    inicioVentana = 0;
    minimoParcial = 0;
    maximoParcial = 0;

    MicrofonoHardware::configurarEntradaAnaloga(patita);
}

void Microfono::setPatita(uint8_t nuevaPatita)
{
    patita = nuevaPatita;

    MicrofonoHardware::configurarEntradaAnaloga(patita);
}

void Microfono::setVentana(uint16_t milisegundos)
{
    if (milisegundos == 0)
    {
        milisegundos = 1;
    }

    ventana = milisegundos;
}

void Microfono::leer()
{
    // descartar una ventana que haya quedado a medias
    // y escuchar una completa desde ahora
    ventanaAbierta = false;
    while (!escuchar())
    {
    }
}

bool Microfono::escuchar()
{
    if (!ventanaAbierta)
    {
        // partir al reves, asi la primera muestra
        // reemplaza al minimo y al maximo
        minimoParcial = 0xFFFF;
        maximoParcial = 0;
        inicioVentana = MicrofonoHardware::milisegundos();
        ventanaAbierta = true;
    }

    uint16_t muestra = MicrofonoHardware::leerPatita(patita);

    if (muestra < minimoParcial)
    {
        minimoParcial = muestra;
    }
    if (muestra > maximoParcial)
    {
        maximoParcial = muestra;
    }

    // restar en vez de comparar con un tiempo final,
    // asi funciona aunque milisegundos() de la vuelta
    if (MicrofonoHardware::milisegundos() - inicioVentana < ventana)
    {
        return false;
    }

    minimo = minimoParcial;
    maximo = maximoParcial;
    ventanaAbierta = false;

    return true;
}

void Microfono::calibrar(uint16_t milisegundos)
{
    // guardar el promedio y no la mayor amplitud,
    // asi un golpe o una falla durante la calibracion
    // no deja al microfono sordo
    uint32_t sumaAmplitudes = 0;
    uint32_t ventanas = 0;

    // escuchar al menos una ventana completa,
    // aunque milisegundos sea menor que la ventana
    uint32_t inicio = MicrofonoHardware::milisegundos();
    do
    {
        leer();

        sumaAmplitudes += maximo - minimo;
        ventanas++;
    } while (MicrofonoHardware::milisegundos() - inicio < milisegundos);

    ruido = sumaAmplitudes / ventanas;
}

void Microfono::setRuido(uint16_t nuevoRuido)
{
    ruido = nuevoRuido;
}

uint16_t Microfono::getRuido()
{
    return ruido;
}

uint16_t Microfono::getAmplitud()
{
    uint16_t amplitud = maximo - minimo;

    // restar el ruido sin bajar de 0
    if (amplitud <= ruido)
    {
        return 0;
    }

    return amplitud - ruido;
}

uint16_t Microfono::getMinimo()
{
    return minimo;
}

uint16_t Microfono::getMaximo()
{
    return maximo;
}
