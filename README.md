![Robbus Kidsy overview](https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/de7d2ab2-48e1-472c-935b-4866fc14e093)
<br><br>
Gracias por tu interés en Robbus Kidsy, un robot móvil de dos ruedas, basado en el microcontrolador ESP32, capaz de moverse por si mismo, completamente programable en Arduino. Los elementos que lo componen son: <br>

![Robbus Kidsy - Parts-05](https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/f7145416-2960-4107-999f-9512fbc5b0ab)

De fábrica Robbus Kidsy viene programado con 3 divertidos juegos:

```bash
1. Kidsy Dice
2. Guia a Kidsy
3. Kidsy Kart
```

Cada uno de estos juegos se inicia al presionar los botones A, B y C. Puedes encontrar más información en el siguiente link:

```bash
https://github.com/RocketLauncherCDMX/RobbusKidsy/tree/master/documents/Spanish
```

## Primeros pasos

La biblioteca compatible para arduino, se encuentra en el library manager, buscando **RobbusKidsy** la podrás descargar directamente dentro de Arduino. de lo contrario, puedes bajar e instalar por tu cuenta la biblioteca para Arduino, para ello, da click en CODE, luego en DOWNLOAD ZIP y copia el archivo ZIP a tu carpeta libraries en Documents/Arduino.

**IMPORTANTE**
Elige la version 2.0.0 de la biblioteca para la version 4.0 o superior de tu Robbus Kidsy. Puedes consultar la versión debajo del logotipo de Rocket Launcher.

![Instalacion de biblioteca 01](https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/ec3a71b2-8fbb-4516-a508-2c1b0934a355)

![Instalacion de biblioteca 02](https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/b91858a9-d555-414a-b7b6-d24503788e34)

## Instalación del core ESP32

Como se ha comentado, el corazón de Kidsy es el ESP32, por lo que, además de la biblioteca, también necesitarás instalar este núcleo. Si ya has usado anteriormente alguna tarjeta compatible, seguramente ya lo tendrás instalado, de lo contrario, bastará con dar click en la pestaña **Herramientas/placa/Gestor de tarjetas**.

![Intalacion de biblioteca 04](https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/3cff5535-1b8b-4468-9b44-99068947c277)

Aquí deberás buscar **ESP32** y seleccionar esp32 by Espressif Systems. Una vez instalado, ya podrás elegir la tarjeta.

![Intalacion de biblioteca 05](https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/53acc892-9f31-40b2-88e9-4cab5cc82a85)

## Selección de tarjeta
La facilidad de Arduino es que dispone de la gran mayoría de tarjetas listas para programarse. Sin embargo, por el momento, Robbus Kidsy no dispone de su propia tarjeta, pero no te preocupes, podrás elegir una tarjeta compatible. La tarjeta que deberás seleccionar es la Adafruit ESP32 Feather. 

![Intalacion de biblioteca 03](https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/a30dbc35-4cc1-4295-95f1-8ff119423142)

Puedes comenzar a divertirte con tu Robbus Kidsy desde que lo sacas de su empaque, ya que trae 3 funcionalidades muy divertidas, las encontrarás explicadas en el siguiente link:

```bash
https://github.com/RocketLauncherCDMX/RobbusKidsy/tree/master/documents/Spanish
```
Si quieres programar por tu cuenta a Kidsy, puedes apoyarte de la gran variedad de ejemplos que la biblioteca tiene incluidos para Arduino, llendo a la pestaña **Archivo/Ejemplos/Robbus Kidsy**

![Intalacion de biblioteca 06](https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/9bad4976-1dda-4eb1-b8d6-46310e29f636)

## IMPORTANTE
Si estás intentando programarlo pero al subir el programa marca un error, asegurate de que el interruptor de encendido esté en la posición ON y el LED verde encendido.

Si alguno de los siguientes elementos no funciona adecuadamente, sensor de color, flechas táctiles o motores, puedes hacer una calibración de cada uno siguiendo los siguientes pasos:

## Calibración del sensor de color

Si tu Kidsy no está detectando los colores correctamente, puedes volver a calibrar el sensor. Para hacerlo, deberás mantener presionado el boton A mientras das un pulso al botón reset. Después de un par de segundos, se escuchará un pitido doble y el neopixel parpadeará en color verde, indicando que la calibración comenzó. Puedes auxiliarte de la comunicación serial (115200 baudios) para seguir las indicaciones, o hacerlo basandote en el color del neopixel.

Los colores a calibrar, en el orden que el neopixel enciende son: rojo, verde, azul, amarillo, cyan, magenta, negro y blanco. Cada que el neopixel encienda, posiciona tu Kidsy sobre el color correspondiente (el sensor de color se encuentra justo por debajo del Neopixel) y presiona el botón A, se escuchará un click y el neopixel cambiará al siguiente color.

Al terminar con el blanco, el neopixel parpadeará en verde, indicando que todos los colores han sido calibrados. Podrás comprobar el sensado de los colores de forma libre poniendo tu Kidsy sobre ellos, y cuando termines, presiona por última vez el botón A para terminar, Kidsy se reiniciará y la calibración habrá concluido.

En conclusión:

```bash
1. Manten el botón A presionado mientras pulsas Reset.
2. Alinea con el color del Neopixel.
3. Presiona el botón A para calibrar ese color.
4. Pasa al siguiente color y repite.
5. Después del blanco, comprueba el sensado de los colores libremente.
6. Presiona el botón A para concluir.
```
[![Calibración del sensor de color](https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/c23a67cf-da22-406d-af47-048f26562b0f)]([URL_DEL_VIDEO](https://www.youtube.com/watch?v=ZopndiGDuBM))

## Calibración de las flechas táctiles

Si las flechas táctiles no detectan los toques, puedes calibrarlas. Para hacerlo, deberás mantener presionado el boton B mientras das un pulso al botón reset. Después de un par de segundos, se escuchará un pitido doble y el neopixel parpadeará en color verde, indicando que la calibración comenzó. Puedes auxiliarte de la comunicación serial (115200 baudios) para seguir las indicaciones, o hacerlo basandote en los LEDs azules.

Para comenzar, los 4 LEDs azules encendidos, significa que deberás calibrar las flechas para detectar cuando no hay toque, por lo que, evitando tocar las flechas en este punto, solo vuelve a presionar el botón B y escucharás un click.

A continuación se encenderá solo un LED, y el neopixel parpadeará en un color diferente. Deberás mantener presionada la flecha correspondiente y pulsar el botón B para guardar la calibración de dicha flecha. Repite el proceso para las otras flechas. el orden de la calibración es el siguiente:

```bash
Parpadeo de neopixel en verde    - Forward (adelante) - LED1 encendido.
Parpadeo de neopixel en magenta  - Backward (atrás)   - LED2 encendido.
Parpadeo de neopixel en amarillo - Left (izquierda)   - LED3 encendido
Parpadeo de neopixel en cyan     - Right (derecha)    - LED4 encendido.
```

Al concluir con la flecha right, Kidsy se reiniciará y la calibración de las flechas táctiles estará completa.
 
En conclusión:

```bash
1. Manten el botón B presionado mientras pulsas Reset.
2. Presiona B sin presionar ninguna flecha (4 LEDs encendidos).
3. Mantén presionada la flecha rcorrecpondiente mientras presionas el botón B.
4. Repite para el resto de flechas.
```

## Calibración de los motores

****Importante****: Para calibrar los motores se utilizan las flechas táctiles, por lo que es necesario que estas se encuentren calibradas a su vez. Kidsy viene con las flechas táctiles calibradas de fábrica, pero si no lo están, regresa al apartado **Calibración de flechas táctiles** antes de continuar.

Si notas que los motores no giran, la velocidad es demasiado lenta, o rápida, o Kidsy no mantiene una linea recta al ir derecho, puedes calibrar la velocidad de cada motor de forma independiente. Para hacerlo, deberás mantener presionado el boton C mientras das un pulso al botón reset. Después de un par de segundos, se escuchará un pitido doble y el neopixel parpadeará en color azul, indicando que la calibración comenzó. Puedes auxiliarte de la comunicación serial (115200 baudios) para seguir las indicaciones, o hacerlo basandote en los LEDs azules y el movimiento de las llantas.

Si las ruedas no comienzan a girar hacia adelante, seguramente es por que la velocidad configurada por defecto es demasiado baja. Para incrementar la velocidad, pulsa la flecha Forward (adelante). Cada pulsación incrementará en 1 el valor de la velocidad. Son 255 valores diferentes, siendo 0 detenida y 255 a la máxima velocidad. Valores cercanos a 0 pueden no tener efecto.

Al pulsar las flechas, escucharás un click y el LED2 parpadeará, indicando que el valor se actualizó. Puedes mantener presionada las flechas para hacer incrementos más rápidos.

La flecha Forward (frente) incrementa la velocidad de ambas llantas, mientras que la flecha backward (atrás) decrementa. Las flechas Left (izquierda) y Right (derecha) decrementan la velocidad de la llanta correspondiente. Esto es útil cuando tu Kidsy no se mueve en línea recta, ya que el mismo valor para cada llanta, puede tener como resultado una velocidad ligeramente diferente, por medio de esto, podrás probar con las 4 flechas hasta que notes que Kidsy va completamente recto y que se mueve a la velocidad que te gusta.

Al llegar a los valores mínimos y máximos de velocidad, escucharás un pitido y el LED parpadeará en color rojo, indicando que ya no puedes incrementar o decrementar más.

Cuando quieras saber si es la calibación adecuada, bastará con que sueltes a Kidsy para que corra libremente. Si lo haces sobre una superficie elevada ten cuidado de que no caiga al llegar al borde. Al terminar, presiona de nuevo el Botón C, Kidsy se reiniciará y la calibración quedará guardada.

En conclusión:

```bash
1. Manten el botón C presionado mientras pulsas Reset.
2. Pulsa las flechas Forward (adelante) o backward (atrás) para cambiar la velocidad.
3. Los valores van de 0 a 255, al llegar a ellos, lo notarás.
3. Pulsa las flechas Left (izquierda) o Right (derecha) para corregir la dirección.
4. Presiona el botón C para concluir la colibración de los motores.
```

## Programa tu Robbus Kidsy

Cuando tengas instalada la biblioteca para Arduino, en la sección Ejemplos, tendrás acceso a cada uno de los programas que hemos escrito para que aprendas cómo funcionan las partes de tu Kidsy. No tengas miedo de experimentar, y si en algún momento quieres volver al programa de fábrica, no te preocupes, el ejemplo dentro de la carpeta RobbusKidsy/Games/ llamado **gameBoundle_01** reestablecerá a tu Kidsy. 

Para programar el gameBundle_01 también será necesario instalar la biblioteca **DabbleESP32.h** desde el library manager, la cual funciona para conectar por Bluetooth a la aplicacion Dabble.

![Intalacion de biblioteca 07](https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/c1764c1f-c265-438c-8347-d2e590c6619e)

¡Que te diviertas con tu Robbus Kidsy!

<p align="right">
  <img src="https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/574c03c0-2e4c-44d7-a36a-98085a2168a6" alt="Rocket Launcher Logo" width="200">
</p>

