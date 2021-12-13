# RobbusKidsy

Robbus Kidsy es un pequeño robot móvil de 9cm de diametro, completamente programable en Arduino, sus funciones son:

![image](https://user-images.githubusercontent.com/66702091/145869150-3855ad0f-0682-41c4-980b-14dd0e8dc90d.png)

![image](https://user-images.githubusercontent.com/66702091/145869299-53fb21c7-4e49-4c9f-98d1-e9d57f57b66f.png)

![image](https://user-images.githubusercontent.com/66702091/145869331-16988e07-13c6-4bbd-96e6-351cbe7f83e8.png)

![image](https://user-images.githubusercontent.com/66702091/145869358-d012a7b4-3752-4c62-b166-6bef9a051054.png)

![image](https://user-images.githubusercontent.com/66702091/145869386-f35299df-ad3f-4dbe-8017-a79e31a8c8d5.png)

![image](https://user-images.githubusercontent.com/66702091/145869434-906a6d9c-d229-42b6-bee7-48d747e5ef91.png)

![image](https://user-images.githubusercontent.com/66702091/145869504-96add70b-390a-4dd5-a867-73d13011e40e.png)

![image](https://user-images.githubusercontent.com/66702091/145869538-a4926910-51c1-45ce-994c-cbfac394b9f8.png)

![image](https://user-images.githubusercontent.com/66702091/145869585-83d75bd4-a8ce-484b-82f3-857966d38479.png)

Está basado en un microcontrolador ESP32 de 32 bits de doble núcleo, un cortex M4 y otro M0, corriendo a 240MHz, cuenta con una SRAM de 520KiB. Sus características y perifericos son:

Conectividad inalámbrica:
  Wi-Fi: 802.11 b/g/n
  Bluetooth: v4.2 BR/EDR y BLE
Interfaces periféricas:
  12-bit SAR ADC de hasta 18 canales
  2 × 8-bit DACs
  10 × sensores de tacto (sensores capacitivos GPIOs)
  4 × SPI
  2 × interfaces I²S
  2 × interfaces I²C
  3 × UART
  Controlador host SD/SDIO/CE-ATA/MMC/eMMC
  Controlador esclavo SDIO/SPI
  Interfaz Ethernet MAC con DMA dedicado y soporte para el protocolo IEEE 1588 Precision Time Protocol
  Bus CAN 2.0
  Controlador remoto infrarrojo (TX/RX, hasta 8 canales)
  Motor PWM
  LED PWM (hasta 16 canales)
  Sensor de efecto Hall
  Pre-amplificador analógico de ultra baja potencia
Seguridad:
  Soporta todas las características de seguridad estándar de IEEE 802.11, incluyendo WFA, WPA/WPA2 y WAPI
  Arranque seguro
  Cifrado flash
  1024-bit OTP, hasta 768-bit para clientes
  Criptografía acelerada por hardware: AES, SHA-2, RSA, criptografía de curva elíptica (ECC), generador de números aleatorios (RNG)
  Administración de energía:
  Regulador interno de baja caída
  Dominio de poder individual para RTC
  Corriente de 5μA en modo de suspensión profundo
  Despierta por interrupción de GPIO, temporizador, medidas de ADC, interrupción por sensor de tacto capacitivo
