# Robbus Kidsy V4.1

![Render_008](https://github.com/RocketLauncherCDMX/RobbusKidsy/assets/66702091/cb009d29-da85-4506-91f8-fa9c1f5d87fb)

La versión de la fotografía es la 3.2 y es solo para fines ilustrativos.

La biblioteca compatible para arduino, se encuentra en el library manager, buscando **RobbusKidsy** la podrás descargar directamente dentro de Arduino. de lo contrario, puedes bajar e instalar por tu cuenta la biblioteca para Arduino, para ello, da click en CODE, luego en DOWNLOAD ZIP y copia el archivo ZIP a tu carpeta libraries en Documents/Arduino.

La tarjeta que deberás seleccionar es la Adafruit ESP32 Feather. 

Puedes comenzar a divertirte con tu Robbus Kidsy desde que lo sacas de su empaque, ya que trae 3 funcionalidades muy divertidas, estas las encontrarás explicadas en la carpeta **Documents**

## IMPORTANTE
Si estás intentando programarlo pero al subir el programa marca un error, asegurate de que el interruptor de encendido esté en la posición ON y el LED verde encendido.

## Descripción

Robbus Kidsy es un pequeño robot móvil capaz de moverse por si mismo. Es completamente programable en Arduino. Los elementos que lo componen son:

![image](https://user-images.githubusercontent.com/66702091/145869150-3855ad0f-0682-41c4-980b-14dd0e8dc90d.png)![image](https://user-images.githubusercontent.com/66702091/145869299-53fb21c7-4e49-4c9f-98d1-e9d57f57b66f.png)

![image](https://user-images.githubusercontent.com/66702091/145869331-16988e07-13c6-4bbd-96e6-351cbe7f83e8.png)![image](https://user-images.githubusercontent.com/66702091/145869358-d012a7b4-3752-4c62-b166-6bef9a051054.png)

![image](https://user-images.githubusercontent.com/66702091/145869386-f35299df-ad3f-4dbe-8017-a79e31a8c8d5.png)![image](https://user-images.githubusercontent.com/66702091/145869434-906a6d9c-d229-42b6-bee7-48d747e5ef91.png)

![image](https://user-images.githubusercontent.com/66702091/145869504-96add70b-390a-4dd5-a867-73d13011e40e.png)![image](https://user-images.githubusercontent.com/66702091/145869538-a4926910-51c1-45ce-994c-cbfac394b9f8.png)

![image](https://user-images.githubusercontent.com/66702091/145869585-83d75bd4-a8ce-484b-82f3-857966d38479.png)

Si alguna función de tu Kidsy no funciona adecuadamente (sensor de color, flechas táctiles o motores), puedes hacer una calibración de cada uno siguiendo los siguientes pasos:

## Calibración del sensor de color

Si tu Kidsy no está detectando los colores correctamente, puedes volver a calibrar el sensor. Para hacerlo, deberás hacer es mantener presionado el boton A mientras das un pulso al botón reset. Después de un par de segundos, se escuchará un pitido doble y el neopixel parpadeará en color verde, indicando que la calibración comenzó. Puedes auxiliarte de la comunicación serial (115200 baudios) para seguir las indicaciones, o hacerlo basandote en el color del neopixel.

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

Cuando tengas instalada la biblioteca para Arduino, en la sección Ejemplos, tendrás acceso a cada uno de los programas que hemos escrito para que aprendas cómo funcionan las partes de tu Kidsy. No tengas miedo de experimentar, y si en algún momento quieres volver al programa de fábrica, no te preocupes, el ejemplo dentro de la carpeta RobbusKidsy/Games/ llamado **gameBoundle_01** reestablecerá a tu Kidsy. Además de instalar la biblioteca **DabbleESP32.h** desde el library manager.

¡Que te diviertas con tu Robbus Kidsy!

