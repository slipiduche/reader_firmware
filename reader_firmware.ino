

/*
  Nombre del Proyecto:         Reader firmware

  Descripcion del Proyecto:

  Este proyecto consiste en un ESP32 como elemento central de control
  encargado de la gestion de todos los datos obtenidos, a
  traves del cual el dispositivo es controlado y programado vía web o servidor local.
  Su funcion es enviar informacion proveniente de un tag nfc a traves de protocolo mqtt.

  Author (s)
  Proyectos Orbittas:           Alejandro Camacaro


  version                       1.0.0
  date                         27/10/2020


  Nombre del archivo :         reader_register_firmware.ino



*/
#include "includes.h"

//programa principal
void loop() ///HMI LOOP
{
  wifiLedBlink();


  if (guardarAp == 1)
  {
    save_config1_spiff();
    ESP.restart();
  }
  if (cambioIp == 1)
  {
    cambioIp = 0;
  }
  DEBUG_PRINT("inicio:");
  DEBUG_PRINTLN(inicio);
  
   if (abs(millis() - nfcDelay) >= 250) 
  { 
    nfcLoop();
    nfcDelay = millis();
    
    
  }

}
void WebComm(void *parameter) ///webloop
{
  for (;;)
  {
    if ((inicio == 0) && (apMode == 0))
    {
      wifi_mqtt_setup();
    }
    if ((inicio == 1) && (apMode == 0))
    { //DEBUG_PRINT("inicio1:");
      //DEBUG_PRINTLN(inicio);
      wifi_mqtt_reconnect(MQTTTopic, MQTTTopic2); //mqtt protocol
    }
    //Serial.print("WebComm() running on core ");
    // Serial.println(xPortGetCoreID());
    //MQTT
    if ((inicio == 2) && (apMode == 0))
    { //DEBUG_PRINT("inicio2:");
      //DEBUG_PRINTLN(inicio);
      if (modo_nowc == 1) ///Si no esta siendo utilizada la pantalla HMI
      {

        
        //DEBUG_PRINT("client state:");
        //DEBUG_PRINTLN(mqttclient.state());
        if (mqttclient.state() != 0 || subscribed == 0)
        { ///0 significa conectado   si no esta conectado intnta reconnecion con broker
          if (mqttclient.state() != -1) ///-1 significa desconectado
          {
            mqttclient.disconnect();
          }
          subscribed = 0;
          wifi_mqtt_reconnect(MQTTTopic, MQTTTopic2); //mqtt protocol
          //wifi_mqtt_subscribe((clientId + "/in").c_str());
        }
        if (subscribed == 0) ///verifica si esta subscrito sino vuelve a intentar la subscripcion
        {
          wifi_mqtt_subscribe((clientId + "/in").c_str());
        }
      }
      wifi_mqtt_loop(); ///proceso en el cual se maneja el cliente de conexion mqtt

    }
  }
  vTaskDelay(10000);
}
void APmode(void *parameter) ///APmode
{
  for (;;)
  {
    //Serial.print("APmode() running on core ");
    //Serial.println(xPortGetCoreID());
    apweb_loop();
  }
  vTaskDelay(1000);
}
