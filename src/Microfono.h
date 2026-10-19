#ifndef MICROFONO_H
#define MICROFONO_H

#include <stdint.h>

/**
 * \~spanish
 * @brief Lee un microfono electret y entrega su volumen.
 *
 * El microfono va con un modulo amplificador (por ejemplo MAX4466 o MAX9814)
 * cuya salida se conecta a una patita analoga. En silencio la salida queda
 * cerca de la mitad del rango, y el sonido la hace subir y bajar.
 *
 * leer() toma muestras durante una ventana de tiempo, por defecto 50
 * milisegundos, y guarda la lectura minima y la maxima. La amplitud es la
 * diferencia entre las dos (pico a pico): mientras mas fuerte el sonido,
 * mayor la amplitud.
 *
 * \~english
 * @brief Reads an electret microphone and returns its volume.
 *
 * The microphone goes with an amplifier module (for example MAX4466 or
 * MAX9814) whose output is connected to an analog pin. In silence the output
 * stays near the middle of the range, and sound makes it go up and down.
 *
 * leer() takes samples during a time window, 50 milliseconds by default, and
 * stores the minimum and the maximum reading. The amplitude is the difference
 * between the two (peak to peak): the louder the sound, the larger the
 * amplitude.
 */
class Microfono
{
public:
    /**
     * \~spanish
     * @brief Crea un microfono y configura la patita como entrada analoga.
     * @param nuevaPatita patita donde esta conectada la salida del modulo.
     *
     * \~english
     * @brief Creates a microphone and configures the pin as an analog input.
     * @param nuevaPatita pin the module's output is connected to.
     */
    Microfono(uint8_t nuevaPatita);

    /**
     * \~spanish
     * @brief Cambia la patita guardada, sin volver a configurarla.
     * @param nuevaPatita nueva patita.
     *
     * \~english
     * @brief Changes the stored pin, without configuring it again.
     * @param nuevaPatita new pin.
     */
    void setPatita(uint8_t nuevaPatita);

    /**
     * \~spanish
     * @brief Configura cuanto tiempo toma muestras cada leer().
     *
     * Una ventana mas larga detecta mejor los sonidos graves, pero
     * leer() se demora mas. El valor inicial es 50. Un 0 se toma como 1.
     *
     * @param milisegundos duracion de la ventana.
     *
     * \~english
     * @brief Configures how long each leer() takes samples for.
     *
     * A longer window detects low sounds better, but leer() takes longer.
     * The initial value is 50. A 0 is treated as 1.
     *
     * @param milisegundos window length.
     */
    void setVentana(uint16_t milisegundos);

    /**
     * \~spanish
     * @brief Escucha el microfono durante la ventana.
     *
     * Bloquea el programa mientras dura la ventana, y despues actualiza
     * el minimo, el maximo y la amplitud.
     *
     * \~english
     * @brief Listens to the microphone during the window.
     *
     * It blocks the program while the window lasts, and then updates
     * the minimum, the maximum and the amplitude.
     */
    void leer();

    /**
     * \~spanish
     * @brief Escucha el microfono sin bloquear el programa.
     *
     * Cada llamada toma una sola muestra. Cuando se completa la ventana,
     * actualiza el minimo, el maximo y la amplitud, devuelve `true`, y la
     * proxima llamada empieza una ventana nueva. Hay que llamarla en cada
     * vuelta del loop: mientras mas seguido, mas muestras por ventana.
     *
     * Si se llama a leer() a mitad de una ventana, esa ventana se descarta.
     *
     * @return `true` si hay una medicion nueva, `false` si la ventana sigue abierta.
     *
     * \~english
     * @brief Listens to the microphone without blocking the program.
     *
     * Each call takes a single sample. When the window is complete, it
     * updates the minimum, the maximum and the amplitude, returns `true`,
     * and the next call starts a new window. Call it on every pass of the
     * loop: the more often, the more samples per window.
     *
     * If leer() is called in the middle of a window, that window is discarded.
     *
     * @return `true` if there is a new measurement, `false` if the window is still open.
     */
    bool escuchar();

    /**
     * \~spanish
     * @brief Mide el ruido del microfono en silencio y lo guarda.
     *
     * Aun en silencio la amplitud no es 0, por el ruido de la placa y del
     * modulo. calibrar() escucha durante el tiempo indicado, guarda la mayor
     * amplitud que encuentra como ruido, y desde ahi getAmplitud() se lo resta.
     *
     * Bloquea el programa mientras dura. Hay que llamarla en silencio,
     * por ejemplo en setup().
     *
     * @param milisegundos cuanto tiempo escuchar, 1000 si no se indica.
     *
     * \~english
     * @brief Measures the microphone's noise in silence and stores it.
     *
     * Even in silence the amplitude is not 0, because of noise from the
     * board and the module. calibrar() listens for the given time, stores
     * the largest amplitude it finds as noise, and from then on getAmplitud()
     * subtracts it.
     *
     * It blocks the program while it lasts. Call it in silence,
     * for example in setup().
     *
     * @param milisegundos how long to listen, 1000 if not given.
     */
    void calibrar(uint16_t milisegundos = 1000);

    /**
     * \~spanish
     * @brief Configura el ruido a mano, sin calibrar.
     *
     * Sirve para usar un valor medido antes. Con 0 se deja de restar ruido.
     *
     * @param nuevoRuido amplitud que se resta a cada medicion.
     *
     * \~english
     * @brief Sets the noise by hand, without calibrating.
     *
     * Useful for reusing a value measured earlier. With 0 no noise is
     * subtracted.
     *
     * @param nuevoRuido amplitude subtracted from each measurement.
     */
    void setRuido(uint16_t nuevoRuido);

    /**
     * \~spanish
     * @brief Ruido que se resta a cada medicion.
     * @return ruido guardado, 0 si no se ha calibrado.
     *
     * \~english
     * @brief Noise subtracted from each measurement.
     * @return stored noise, 0 if not calibrated.
     */
    uint16_t getRuido();

    /**
     * \~spanish
     * @brief Amplitud del sonido en la ultima ventana, sin el ruido.
     *
     * Es la diferencia entre la lectura maxima y la minima (pico a pico),
     * menos el ruido. Nunca es menor que 0. Sin calibrar, el ruido es 0.
     *
     * @return amplitud pico a pico, 0 en silencio si se calibro.
     *
     * \~english
     * @brief Amplitude of the sound in the last window, without the noise.
     *
     * It is the difference between the maximum and the minimum reading
     * (peak to peak), minus the noise. It is never below 0. Without
     * calibrating, the noise is 0.
     *
     * @return peak-to-peak amplitude, 0 in silence if calibrated.
     */
    uint16_t getAmplitud();

    /**
     * \~spanish
     * @brief Lectura minima de la ultima leer().
     * @return valor minimo, su rango depende de la placa.
     *
     * \~english
     * @brief Minimum reading of the last leer().
     * @return minimum value, its range depends on the board.
     */
    uint16_t getMinimo();

    /**
     * \~spanish
     * @brief Lectura maxima de la ultima leer().
     * @return valor maximo, su rango depende de la placa.
     *
     * \~english
     * @brief Maximum reading of the last leer().
     * @return maximum value, its range depends on the board.
     */
    uint16_t getMaximo();

private:
    uint8_t patita;
    uint16_t ventana;
    uint16_t minimo;
    uint16_t maximo;
    uint16_t ruido;
    bool ventanaAbierta;
    uint32_t inicioVentana;
    uint16_t minimoParcial;
    uint16_t maximoParcial;
};

#endif
