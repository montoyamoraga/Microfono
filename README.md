# Microfono

Biblioteca para leer micrófonos electret con microcontroladores, y medir la amplitud del sonido.

Programada en C++, diseñada para uso con placas de desarrollo de las marcas Arduino y Raspberry Pi Pico.

Está disponible en el registro de bibliotecas de Arduino IDE.

## Conexiones

Si estás usando un módulo, tiene 3 patitas:

- VCC: voltaje de alimentación.
- GND: tierra.
- OUT: salida de señal.

La salida de señal se conecta a una patita con ADC, por ejemplo en Raspberry Pi Pico usar las patitas GPIO 26, 27 o 28.

VCC va al mismo voltaje con que funciona la placa: 5V en Arduino Uno, y 3.3V en placas de 3.3V, como Raspberry Pi Pico, Arduino Nano 33 o ESP32. Con el módulo a 5V, la salida puede pasar de 3.3V y dañar la patita de esas placas.

## Uso

Para importar la biblioteca

```cpp
#include "Microfono.h"
```

Para declarar una instancia llamada microfono

```cpp
// micrófono conectado a la patita A0
Microfono microfono(A0);
```

Para leer y actualizar variables internas del objeto.

```cpp
microfono.leer();
```

Para obtener la amplitud

```cpp
microfono.getAmplitud();
```

## Calibrar el ruido

El silencio es una decisión, y como tal, debemos definir un umbral.

Para eso está la función `calibrar()`, que escucha 1 segundo y guarda el promedio de las amplitudes como umbral. Desde ahí, ese umbral se resta a cada medición, y todo lo menor a él es considerado 0. Como es un promedio, en silencio la amplitud queda cerca de 0, con saltos pequeños.

Si quieres calibrar usando una muestra distinta de tiempo, puedes usar un parámetro medido en milisegundos `calibrar(uint16_t milisegundos)`.

Esta función bloquea el funcionamiento del programa, considerarlo durante el diseño de dispositivos.

Si quieres definir el umbral, puedes usar `setRuido(uint16_t ruido)`.

Ejemplo en [examples/ej02_calibrar/](./examples/ej02_calibrar/).

## Sin bloquear

`leer()` detiene el programa durante toda la ventana. Si necesitas hacer otras cosas a la vez, como leer botones o hacer parpadear un led, usa `escuchar()`: toma una sola muestra en cada llamada y devuelve `true` cuando termina una ventana.

```cpp
void loop()
{
  if (microfono.escuchar())
  {
    Serial.println(microfono.getAmplitud());
  }

  // aquí puedes hacer otras cosas
}
```

Hay que llamarla en cada vuelta del loop: mientras más seguido, más muestras por ventana. Si el loop se demora mucho en otras cosas, la medición pierde precisión.

Ejemplo en [examples/ej01_sinBloquear/](./examples/ej01_sinBloquear/).

## Uso con Raspberry Pi Pico SDK

La carpeta [pico/](./pico/) tiene un proyecto CMake que compila la biblioteca y un ejemplo para Pico 2. Necesitas la [Pico SDK](https://github.com/raspberrypi/pico-sdk) instalada, o la extensión Raspberry Pi Pico de VS Code.

```bash
cd pico
mkdir build
cd build
PICO_SDK_PATH=/ruta/a/pico-sdk cmake ..
cmake --build .
```

Esto genera un archivo `.uf2` por ejemplo, como `ej00_leerMicrofono.uf2`. Para cargarlo, conecta la Pico manteniendo presionado el botón BOOTSEL y copia el archivo a la unidad que aparece.

En Pico, el micrófono se lee de 0 a 4095 (12 bits), no de 0 a 1023 como en Arduino Uno. Además, al crear el primer micrófono la biblioteca pone el regulador de la placa en modo PWM (GPIO 23), que hace menos ruido en la lectura.

Para otra placa, cambia `PICO_BOARD` en [pico/CMakeLists.txt](./pico/CMakeLists.txt) (por ejemplo `pico` para la Pico original).

Ejemplo completo en [pico/ej00_leerMicrofono/main.cpp](./pico/ej00_leerMicrofono/main.cpp).

## Ejemplos

Cada ejemplo existe para Arduino, en [examples/](./examples/), y para Pico SDK, en [pico/](./pico/). En Arduino el micrófono está conectado a A0, y en Pico a GPIO 26.

| Ejemplo | Qué hace |
| -------------------- | ----------------------------------------------------------------- |
| `ej00_leerMicrofono` | imprime la amplitud del sonido, en formato para el plotter serial |
| `ej01_sinBloquear` | lee el micrófono sin bloquear, mientras el led de la placa parpadea |
| `ej02_calibrar` | mide el ruido en silencio al partir y lo resta de la amplitud |

## Referencia

| Método | Descripción |
| --- | --- |
| `Microfono(uint8_t patita)` | crea un micrófono y configura la patita como entrada análoga |
| `void setVentana(uint16_t milisegundos)` | cuánto tiempo escucha cada `leer()`. El valor inicial es 50. Un 0 se toma como 1 |
| `void leer()` | escucha el micrófono durante la ventana. Bloquea el programa mientras dura |
| `bool escuchar()` | toma una muestra sin bloquear. Devuelve `true` cuando termina una ventana. Hay que llamarla en cada vuelta del loop |
| `void calibrar(uint16_t milisegundos)` | escucha en silencio y guarda el promedio de las amplitudes como ruido. Sin parámetro escucha 1000 milisegundos. Bloquea el programa mientras dura |
| `void setRuido(uint16_t ruido)` | configura el ruido a mano. Con 0 se deja de restar |
| `uint16_t getRuido()` | ruido que se resta a cada medición, 0 si no se ha calibrado |
| `uint16_t getAmplitud()` | diferencia entre la lectura máxima y la mínima (pico a pico), menos el ruido. Nunca menor que 0 |
| `uint16_t getMinimo()` | lectura mínima de la última `leer()` |
| `uint16_t getMaximo()` | lectura máxima de la última `leer()` |
| `void setPatita(uint8_t patita)` | cambia la patita y la configura como entrada análoga. En Pico, una patita sin ADC se lee como 0 |

## Documentación

La documentación se genera con [Doxygen](https://www.doxygen.nl/) a partir de los comentarios en [src/](./src/), en español y en inglés, y se publica en <https://piruetasxyz.github.io/Microfono/>.

## Cómo está organizado

El código de [src/Microfono.cpp](./src/Microfono.cpp) no tiene dependencias.

El código en [src/Hardware.h](./src/Hardware.h) contiene una implementación por placa:

- [src/arduino/ArduinoHardware.cpp](./src/arduino/ArduinoHardware.cpp): usa `analogRead` y `millis`.
- [pico/PicoHardware.cpp](./pico/PicoHardware.cpp): usa las funciones `adc_*` del Pico SDK.

Para agregar otra plataforma, basta con escribir otra implementación de `Hardware.h`.

## Contenido

- [.github/](./.github): flujos de trabajo que compilan los ejemplos de distintas placas, revisan la biblioteca con Arduino Lint, y publican la documentación.
- [docs/](./docs/): página principal de la documentación y página para elegir idioma.
- [examples/](./examples/): ejemplos para Arduino.
- [Doxyfile](./Doxyfile): configuración de Doxygen.
- [pico/](./pico/): proyecto CMake, implementación y ejemplo para Pico SDK.
- [src/](./src/): código de la biblioteca.
- [keywords.txt](./keywords.txt): indicadores de sintaxis para Arduino IDE.
- [library.properties](./library.properties): metadatos para Arduino IDE.
- [LICENSE](./LICENSE): licencia MIT.
- [README.md](./README.md): este documento.

## Versiones

- v0.0.1: octubre 2026, primera versión, para Arduino y Raspberry Pi Pico SDK, con lectura bloqueante y sin bloquear, y calibración del ruido.

## Licencia

MIT, ver [LICENSE](./LICENSE).
