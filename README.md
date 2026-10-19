# Microfono

Biblioteca para leer micrófonos electret con microcontroladores, y medir la amplitud del sonido.

Funciona con placas Arduino y con placas Raspberry Pi Pico, tanto desde Arduino como desde el Pico SDK.

## Conexiones

El micrófono va con un módulo amplificador, por ejemplo MAX4466 o MAX9814. El módulo tiene tres patitas:

- VCC va a voltaje (5V o 3.3V, según la placa).
- GND va a tierra (GND).
- OUT va a una entrada análoga.

En Raspberry Pi Pico usa 3.3V (patita `3V3`, nunca 5V) y una de las patitas con conversor análogo-digital: GPIO 26, 27 o 28.

## Uso con Arduino

Instala la biblioteca copiando esta carpeta en tu carpeta de bibliotecas de Arduino (por ejemplo `~/Documents/Arduino/libraries/Microfono`), o desde Arduino IDE con *Sketch → Include Library → Add .ZIP Library...*.

```cpp
#include "Microfono.h"

// micrófono conectado a la patita A0
Microfono microfono(A0);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  // escuchar durante 50 milisegundos
  microfono.leer();

  Serial.println(microfono.getAmplitud());
}
```

Ejemplo en [examples/ej00_leerMicrofono/](./examples/ej00_leerMicrofono/).

## Calibrar el ruido

Aun en silencio la amplitud no es 0, por el ruido de la placa y del módulo. `microfono.calibrar()` escucha 1 segundo en silencio, guarda la mayor amplitud que encuentra como ruido, y desde ahí `getAmplitud()` se lo resta. Así el silencio queda en 0.

```cpp
void setup()
{
  // no hagas ruido mientras calibra
  microfono.calibrar();
}
```

`calibrar(3000)` escucha 3 segundos en vez de 1. Bloquea el programa mientras dura, así que conviene llamarla en `setup()`. Para usar un valor medido antes, sin calibrar, usa `setRuido(20)`. Con `setRuido(0)` se deja de restar. Si no llamas a ninguna de las dos, la amplitud es la lectura directa.

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

La carpeta [pico/](./pico/) tiene un proyecto CMake que compila la biblioteca y un ejemplo para Pico 2. Necesitas el [Pico SDK](https://github.com/raspberrypi/pico-sdk) instalado, o la extensión Raspberry Pi Pico de VS Code.

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

Cada ejemplo existe para Arduino, en [examples/](./examples/), y para Pico SDK, en [pico/](./pico/). En Arduino el micrófono va en A0, y en Pico en GPIO 26.

| Ejemplo | Qué hace |
| --- | --- |
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
| `void calibrar(uint16_t milisegundos)` | escucha en silencio y guarda la mayor amplitud como ruido. Sin parámetro escucha 1000 milisegundos. Bloquea el programa mientras dura |
| `void setRuido(uint16_t ruido)` | configura el ruido a mano. Con 0 se deja de restar |
| `uint16_t getRuido()` | ruido que se resta a cada medición, 0 si no se ha calibrado |
| `uint16_t getAmplitud()` | diferencia entre la lectura máxima y la mínima (pico a pico), menos el ruido. Nunca menor que 0 |
| `uint16_t getMinimo()` | lectura mínima de la última `leer()` |
| `uint16_t getMaximo()` | lectura máxima de la última `leer()` |
| `void setPatita(uint8_t patita)` | cambia la patita guardada (no la vuelve a configurar) |

## Documentación

La documentación se genera con [Doxygen](https://www.doxygen.nl/) a partir de los comentarios en [src/](./src/), en español y en inglés, y se publica en <https://piruetasxyz.github.io/Microfono/>.

Para generarla en tu computador, en español:

```bash
(cat Doxyfile; echo "OUTPUT_LANGUAGE = Spanish"; echo "HTML_OUTPUT = es") | doxygen -
```

Queda en `build/docs/es/index.html`. Para inglés, usa `English` y `en`.

Cada comentario tiene una sección `\~spanish` y una sección `\~english`. Si agregas algo público sin documentar, la generación falla.

## Cómo está organizado

El código de [src/Microfono.cpp](./src/Microfono.cpp) no depende de ninguna plataforma. Todo lo que toca el hardware pasa por [src/Hardware.h](./src/Hardware.h), que tiene una implementación por plataforma:

- [src/arduino/ArduinoHardware.cpp](./src/arduino/ArduinoHardware.cpp): usa `analogRead` y `millis`.
- [pico/PicoHardware.cpp](./pico/PicoHardware.cpp): usa las funciones `adc_*` del Pico SDK.

Para agregar otra plataforma, basta con escribir otra implementación de `Hardware.h`.

## Contenido

- [.github/](./.github): flujos de trabajo que compilan los ejemplos de Arduino y de Pico, revisan la biblioteca con Arduino Lint, y publican la documentación.
- [docs/](./docs/): página principal de la documentación y página para elegir idioma.
- [examples/](./examples/): ejemplos para Arduino.
- [Doxyfile](./Doxyfile): configuración de Doxygen.
- [pico/](./pico/): proyecto CMake, implementación y ejemplo para Pico SDK.
- [src/](./src/): código de la biblioteca.
- [keywords.txt](./keywords.txt): colores de sintaxis para Arduino IDE.
- [library.properties](./library.properties): metadatos para Arduino.
- [LICENSE](./LICENSE): licencia MIT.
- [README.md](./README.md): este documento.

## Versiones

- v0.1.0: octubre 2026, primera versión, para Arduino y Raspberry Pi Pico SDK, con lectura bloqueante y sin bloquear, y calibración del ruido.

## Licencia

MIT, ver [LICENSE](./LICENSE).
