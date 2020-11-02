#ifdef USEWIFIMQTT
/***********************************************************************/
//Nombre de la funcion :callback()
//Entrada: char* topic ,byte* payload, unsigned int length
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de recibir las tramas enviadas desde el broker por el tema subscrito
//ademas graba carga la configuracion al equipo si recibe un comando valido y lo guarda en la memoria micro SD
/***********************************************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
  memset(datarecvd, 0, 512);
  DEBUG_PRINT("Message arrived in topic: ");
  DEBUG_PRINTLN(topic);

  DEBUG_PRINT("Message:"); //Recepción de trama MQTT
  for (int i = 0; i < length; i++)
  {
    DEBUG_PRINT((char)payload[i]);
    datarecvd[i] = payload[i];
  }

  //loadmqttconfig(datarecvd);                                  //Ubicada en fun_loadconfig.ino--> Verifica si la trama es un comando de configuración vàlido
  DEBUG_PRINTLN();
  DEBUG_PRINTLN("-----------------------");
  //loadMqttCommand(datarecvd); ///ubicada en fun_loadmqttcommand.ino
}
/***********************************************************************/
//Nombre de la funcion :wifi_mqtt_setup()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de configurar los parametros de BROKER mqtt a traves de WIFI
/***********************************************************************/
void wifi_mqtt_setup()
{
  bussyMqtt = 1;
  count = 1;
  WiFi.begin(ssid, password); //Inicia la configuracion wifi local
  DEBUG_PRINTLN("conectando wifi");
  while (WiFi.status() != WL_CONNECTED)
  { //Realiza intentos de conexion a wifi local si no esta conectado
    delay(500);
    DEBUG_PRINT(".");
    count++;
    if (count > 50)
    {
      count = 0;
      apMode = 1;
      break;
    }
    wifiLedBlink();
  }
  if (count > 0)
  {
    DEBUG_PRINTLN("");
    DEBUG_PRINT(ssid);
    DEBUG_PRINT(" RSSI:");
    WRSSI = String(WiFi.RSSI());
    DEBUG_PRINTLN(WRSSI);
    DEBUG_PRINTLN("WiFi connected");
    DEBUG_PRINTLN("STATION IP address: ");

    ipRed = String(WiFi.localIP()[0]) + "." + String(WiFi.localIP()[1]) + "." + String(WiFi.localIP()[2]) + "." + String(WiFi.localIP()[3]);
    DEBUG_PRINTLN(ipRed);
    cambioIp = 1;
    count = 0;
  }
  String MQTTPORT = String(MQTTPort);
  DEBUG_PRINTLN(MQTTHost);
  DEBUG_PRINTLN(MQTTPORT.toInt());

  mqttclient.setServer(MQTTHost, MQTTPORT.toInt());
  mqttclient.setCallback(callback);

  wifi_mqtt_reconnect_setup(MQTTTopic, MQTTTopic2);
  bussyMqtt = 0;
}
/***********************************************************************/
//Nombre de la funcion :wifi_mqtt_reconnect()
//Entrada:char mqtttopic[120],mqtttopic2[120]
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada establecer conexion y reconexion con broker MQTT via WIFI
/***********************************************************************/

void wifi_mqtt_reconnect(char mqtttopic[120], char mqtttopic2[120])
{
  bussyMqtt = 1;
  char topicCh[120];

  String topic_s = "";

  if ((WiFi.status() == WL_CONNECTED) && (apMode == 0))
  {
    while (!mqttclient.connected())
    {
      DEBUG_PRINT("Attempting MQTT connection...");

      String topic_s = clientId + "/out";

      DEBUG_PRINTLN(MQTTUsername);
      DEBUG_PRINTLN(MQTTPassword);
      if (mqttclient.connect(clientId.c_str(), MQTTUsername, MQTTPassword))
      {
        DEBUG_PRINTLN("WCONNECT PACKET SUCCESS");
        inicio = 2;
        wifi_mqtt_publish(topic_s.c_str(), "{\"mqtt\": \"RECONNECTED\"}");
        topic_s = "";

        topic_s = "SERVER/POLL";
        DEBUG_PRINTLN(topic_s);
        wifi_mqtt_subscribe(topic_s.c_str());
        topic_s = "";
        topic_s = "SERVER/" + chipid;
        DEBUG_PRINTLN(topic_s);
        wifi_mqtt_subscribe(topic_s.c_str());
        reconnect = 0;
        bussyMqtt = 0;
      }
      else
      {
        DEBUG_PRINT("failed, rc=");
        DEBUG_PRINT(mqttclient.state());
        DEBUG_PRINTLN(" try again in 5 seconds");
#ifdef AP

        //webloopmodo AP
#endif
        reconnect++;

        if ((reconnect > 4) && (apMode == 0)) //Realiza 3 intentos de reconexion y sale del bucle.
        {
          apMode = 1;
          reconnect = 0;
          WiFi.reconnect();
          inicio = 1;
          //  break;
        }
      }
      wifiLedBlink();
    }
  }
}

/***********************************************************************/
//Nombre de la funcion :wifi_mqtt_reconnect()
//Entrada:char mqtttopic[120],mqtttopic2[120]
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada establecer conexion y reconexion con broker MQTT via WIFI
/***********************************************************************/

void wifi_mqtt_reconnect_setup(char mqtttopic[120], char mqtttopic2[120])
{
  String topic_s = "";
  bussyMqtt = 1;
  if ((WiFi.status() == WL_CONNECTED) && (apMode == 0))
  {
    while (!mqttclient.connected())
    {
      DEBUG_PRINT("Attempting MQTT connection...");

      topic_s = clientId + "/out";
      DEBUG_PRINTLN(topic_s);

      DEBUG_PRINTLN(MQTTUsername);
      DEBUG_PRINTLN(MQTTPassword);

      if (mqttclient.connect(clientId.c_str(), MQTTUsername, MQTTPassword))
      {
        DEBUG_PRINTLN("WCONNECT PACKET SUCCESS");
        inicio = 2;
        wifi_mqtt_publish(topic_s.c_str(), "{\"mqtt\": \"RECONNECTED\"}");
        topic_s = "";

        topic_s = "SERVER/POLL";
        DEBUG_PRINTLN(topic_s);
        wifi_mqtt_subscribe(topic_s.c_str());
        topic_s = "";
        topic_s = "SERVER/" + chipid;
        DEBUG_PRINTLN(topic_s);
        wifi_mqtt_subscribe(topic_s.c_str());
        reconnect = 0;
        bussyMqtt = 0;
      }
      else
      {
        DEBUG_PRINT("failed, rc=");
        DEBUG_PRINT(mqttclient.state());
        DEBUG_PRINTLN(" try again in 5 seconds");
#ifdef AP

        //webloopmodo AP
#endif
        reconnect++;

        if ((reconnect > 4) && (apMode == 0)) //Realiza 3 intentos de reconexion y sale del bucle.
        {
          apMode = 1;
          reconnect = 0;
          WiFi.reconnect();
          inicio = 1;
          break;
        }
      }
      wifiLedBlink();
    }
  }
}
/***********************************************************************/
//Nombre de la funcion :wifi_mqtt_publish()
//Entrada: char mqtttopic,char msg[512]
//Salida :int
//Descripcion:Esta funcion es la encargada de enviar PUBLISH packet MQTT via WIFI
/***********************************************************************/
int wifi_mqtt_publish(String mqtttopic, String msg)
{
  bussyMqtt = 1;
  DEBUG_PRINTLN("mqtt_connected:");
  bool mqtt_connected = mqttclient.connected();
  DEBUG_PRINTLN(mqtt_connected);
  if (mqttclient.connected())
  {
    mqttclient.publish(mqtttopic.c_str(), msg.c_str());
    DEBUG_PRINTLN("WPUBLISH PACKET SENT");
    bussyMqtt = 0;
    return 1;
  }
  else
  {
    bussyMqtt = 0;
    return 0;
  }
}
/***********************************************************************/
//Nombre de la funcion :wifi_mqtt_subscribe()
//Entrada: char mqtttopic[120]
//Salida :int
//Descripcion:Esta funcion es la encargada de subscribirse a un tema en BROKER MQTT WIFI
/***********************************************************************/
int wifi_mqtt_subscribe(String mqtttopic)
{
  bussyMqtt = 1;
  DEBUG_PRINTLN("mqtt_connected:");
  bool mqtt_connected = mqttclient.connected();
  DEBUG_PRINTLN(mqtt_connected);
  if (mqtt_connected)
  {
    if (mqttclient.subscribe((mqtttopic).c_str()))
    {
      wifi_mqtt_publish((clientId + "/out"), "{\"mqtt\": \"SUBSCRIBED\"}");
      DEBUG_PRINTLN("WSUBSCRIBE PACKET SENT");
      subscribed = 1;
      apMode = 0;
      minutosEnApMode = 0;
      bussyMqtt = 0;
      return 1;
    }
  }
  else
  {
    bussyMqtt = 0;
    return 0;
  }
}
/***********************************************************************/
//Nombre de la funcion :wifi_mqtt_loop()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de manejar el cliente MQTT WIFI
/***********************************************************************/
void wifi_mqtt_loop()
{

  mqttclient.loop();
  if (abs(millis() - mqttdelay) >= 500)
  {
    Serial.println(tagId);
    // mqtt_send_changes();
    if (tagId > 0)
    {
      wifi_mqtt_publish((clientId + "/out"), "{\"TAG\":" + String(tagId) + ",\"ACTION\":\"START\"}");
      tagId = 0;
    }
    if (tagId == 0)
    {
      wifi_mqtt_publish((clientId + "/out"), "{\"TAG\":" + String(tagId) + ",\"ACTION\":\"STOP\"}");
      tagId = 0;
    }
    if (serverPoll){

      if (wifi_mqtt_publish(("READER/INFO"), "{ \"NAME\": \"READER\",\"CHIP_ID\": \"SERIAL NUMBER\"}"))
      {
        serverPoll=0;
      }
    }
    mqttdelay = millis();
  }
}

#else
void callback(char *topic, byte *payload, unsigned int length) {}
void wifi_mqtt_setup() {}
void wifi_mqtt_reconnect(char mqtttopic[120], char mqtttopic2[120]) {}
int wifi_mqtt_publish(char mqtttopic[120], char msg[512]) {}
int wifi_mqtt_subscribe(char mqtttopic[120]) {}
void wifi_mqtt_loop() {}
void mqtt_send_changes() {}
#endif
