# SDRV2
  Descripcion del Proyecto:

  Este proyecto consiste en un ESP32 como elemento central de control
  encargado de la gestion de todos los datos obtenidos, a
  traves del cual el dispositivo es controlado y programado vía web o servidor local.

  Author (s)
  Proyectos Orbittas:           Alejandro Camacaro


  version                       2.0.1
  date                          8/12/2020


  Nombre del archivo :         SDRV2.ino



  Dependencias:Requiere de estas librerias para trabajar :

*      ArduinoJson.h       ---> Por Benoit Blanchon Version 5.13.2 <--
       Wire.h              ---> Libreria que funciona por defecto al instalar Arduino ide
       SPI.h               ---> Libreria que funciona por defecto al instalar Arduino ide
       EEPROM.h            ---> Libreria que funciona por defecto al instalar Arduino ide
       RTClib.h          ---> adafruit  version 1.11.0
       ESP32WebServer.h    ---> Por Ivan Grokhotkov Version 1.0 <-- ivan@ESP32.com IOTSHARING.COM https://github.com/nhatuan84/esp32-webserver
       Adafruit_PN532.h ---> v 1.2.0 adafruit (adafruit busio v 1.6.0)

       


*      *  Requisitos para compilar el codigo:

  -Tener instalado arduino ide-1.8.12
  - Esp32 arduino core 1.0.2 (falla con versiones superiores backtrace)
  -Descomprimir todos los archivos de la carpeta librerias en el directorio de librerías de Arduino.



  Dispositivos : HMI Nextion, 1 modulos relay x4 (total 4 reles), Ds3231

  La estructura del programa esta basada en funciones las cuales para facilidad de lectura estan almacenadas en archivos .ino que representan
  funcionalidades específicas del dispositivo, estos archivos deben estar en la carpeta de compilacion del archivo principal.

  Las funciones son las Siguientes (verificar que en la carpeta de compilacion esten todas presentes):

    fun_load_hmi_config.ino --> carga y lectura de dato provenientes de HMI
    fun_loadconfig.ino      --> carga de datos iniciales
    fun_loadmqttcommand.ino --> carga de datos provenientes del broker
    fun_spiff.ino           --> almacenamiento y lectura de datos
    fun_web.ino             --> controladores de web AP
    fun_WIFI_MQTT.ino       --> controladores para funciones de comunicación wifi y mqtt
    HMI.ino                 --> controlador de protocolo de comunicación HMI
    outputcontrol.ino       --> control de salida y funciones de temporizacion 
    SDRV2.ino               --> programa principal (nombre de carpteta de compilacion SDRV2)
    setup.ino               --> configuraciones de inicio.
    time.ino                --> configuracion y manejo de funciones del ds3231 como reloj.

**
