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
  loadMqttCommand(datarecvd); ///ubicada en fun_loadmqttcommand.ino
}
/***********************************************************************/
//Nombre de la funcion :wifi_mqtt_setup()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de configurar los parametros de BROKER mqtt a traves de WIFI
/***********************************************************************/
void wifi_mqtt_setup()
{
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
      apMode=1;
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
    
    ipRed=String(WiFi.localIP()[0])+"."+String(WiFi.localIP()[1])+"."+String(WiFi.localIP()[2])+"."+String(WiFi.localIP()[3]);
    DEBUG_PRINTLN(ipRed);
    cambioIp=1;
    count = 0;
  }
  String MQTTPORT = String(MQTTPort);
  DEBUG_PRINTLN(MQTTHost);
  DEBUG_PRINTLN(MQTTPORT.toInt());

  mqttclient.setServer(MQTTHost, MQTTPORT.toInt());
  mqttclient.setCallback(callback);

  wifi_mqtt_reconnect_setup(MQTTTopic, MQTTTopic2);
}
/***********************************************************************/
//Nombre de la funcion :wifi_mqtt_reconnect()
//Entrada:char mqtttopic[120],mqtttopic2[120]
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada establecer conexion y reconexion con broker MQTT via WIFI
/***********************************************************************/

void wifi_mqtt_reconnect(char mqtttopic[120], char mqtttopic2[120])
{
  char topicCh[120];

  String topic_s = "";

  if ((WiFi.status() == WL_CONNECTED)&&(apMode==0))
  {
    while (!mqttclient.connected())
    {
      DEBUG_PRINT("Attempting MQTT connection...");
      
      clientId += String(chipid);
      String topic_s = clientId + "/out";

      DEBUG_PRINTLN(MQTTUsername);
      DEBUG_PRINTLN(MQTTPassword);
      if (mqttclient.connect(clientId.c_str(), MQTTUsername, MQTTPassword))
      {
        DEBUG_PRINTLN("WCONNECT PACKET SUCCESS");
        inicio = 2;
        wifi_mqtt_publish(topic_s.c_str(), "{\"mqtt\": \"RECONNECTED\"}");
        topic_s = "";
        
        clientId += String(chipid);
        topic_s = clientId + "/in";

        wifi_mqtt_subscribe(topic_s.c_str());
        reconnect = 0;
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

        if ((reconnect > 4)&&(apMode==0)) //Realiza 3 intentos de reconexion y sale del bucle.
        { apMode=1;
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

  if ((WiFi.status() == WL_CONNECTED)&&(apMode==0))
  {
    while (!mqttclient.connected())
    {
      DEBUG_PRINT("Attempting MQTT connection...");
      
      clientId += String(chipid);
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
       
        clientId += String(chipid);
        topic_s = clientId + "/in";

        wifi_mqtt_subscribe(topic_s.c_str());
        reconnect = 0;
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

        if ((reconnect > 4)&&(apMode==0)) //Realiza 3 intentos de reconexion y sale del bucle.
        { apMode=1;
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
  DEBUG_PRINTLN("mqtt_connected:");
  bool mqtt_connected = mqttclient.connected();
  DEBUG_PRINTLN(mqtt_connected);
  if (mqttclient.connected())
  {
    mqttclient.publish(mqtttopic.c_str(), msg.c_str());
    DEBUG_PRINTLN("WPUBLISH PACKET SENT");
    return 1;
  }
  else
    return 0;
}
/***********************************************************************/
//Nombre de la funcion :wifi_mqtt_subscribe()
//Entrada: char mqtttopic[120]
//Salida :int
//Descripcion:Esta funcion es la encargada de subscribirse a un tema en BROKER MQTT WIFI
/***********************************************************************/
int wifi_mqtt_subscribe(String mqtttopic)
{
  DEBUG_PRINTLN("mqtt_connected:");
  bool mqtt_connected = mqttclient.connected();
  DEBUG_PRINTLN(mqtt_connected);
  if (mqtt_connected)
  {
    if(mqttclient.subscribe((clientId + "/in").c_str()))
    {
    wifi_mqtt_publish((clientId + "/out"), "{\"mqtt\": \"SUBSCRIBED\"}");
    DEBUG_PRINTLN("WSUBSCRIBE PACKET SENT");
    subscribed=1;
    apMode=0;
    minutosEnApMode=0;
    return 1;
    }
  }
  else
    return 0;
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
    mqtt_send_changes();
    mqttdelay = millis();
  }
}
void mqtt_send_changes()
{
  int counter = 0;
  String mensaje = "", min_aux = "", hora_aux = "";

  //verifica si ocurrió cambio en valvulas
  for (int i = 0; i < 4; i++)
  {
    if (val_aux1[i] != valvulas[i + 1])
    {
      val_aux1[i] = valvulas[i + 1];
      counter++;
    }
  }
  if (counter > 0)
  {
    solicitud_web = 1; //envía todo
  }
  if (envia_horarios==1)
  { mensaje = "";
    clientId = "";
    //si ocurrió cambio envía a broker
    
    clientId += String(chipid);
    String topic_valve = clientId + "/out";
    DEBUG_PRINTLN(topic_valve);
    mensaje = "{\"horarios\":["; //1,1,1,1]}"";
    mensaje +=  String(numero_horarios) + ",";
    for (int i = 0; i < numero_horarios * 6; i++)
    {
      mensaje += String(horario[i]);
      if (i == (6 * numero_horarios - 1))
      {
        mensaje += "]}";
      }
      else
      {
        mensaje += ",";
      }
    }
    DEBUG_PRINTLN(mensaje);
    wifi_mqtt_publish(topic_valve.c_str(), mensaje.c_str());
    envia_horarios = 0;
    solicitud_web = 0;
  }
  if (solicitud_web == 1)
  {
    mensaje = "";
    clientId = "";
    //si ocurrió cambio envía a broker
    
    clientId += String(chipid);

    String topic_valve = clientId + "/out";
    DEBUG_PRINTLN(topic_valve);
    mensaje = "{\"estado_valvulas\":["; //1,1,1,1]}"";
    mensaje = mensaje + String(!valvulas[1]) + "," + String(!valvulas[2]) + "," + String(!valvulas[3]) + "," + String(!valvulas[4]) + "],\"modo_activo\":";

    mensaje = mensaje + String(modo_automatico) + ",\"proximo_ciclo\":\"";

    min_aux = String(horario[6 * (proximo_ciclo - 1) + 1]);
    if (horario[6 * (proximo_ciclo - 1) + 1] < 10)
    {
      min_aux = "0" + String(horario[6 * (proximo_ciclo - 1) + 1]);
    }
    hora_aux = String(horario[6 * (proximo_ciclo - 1)]);
    if (horario[6 * (proximo_ciclo - 1)] < 10)
    {
      hora_aux = "0" + String(horario[6 * (proximo_ciclo - 1)]);
    }
    String Shora=String(hora);
    if (hora<10)
    {
      Shora="0"+Shora;
    }
    String Smin=String(minuto);
    if (minuto<10)
    {
      Smin="0"+Smin;
    }
    String Sdia=String(dia);
    if (dia<10)
    {
      Sdia="0"+Sdia;
    }
    String Smes=String(mes);
    if (mes<10)
    {
      Smes="0"+Smes;
    }
    mensaje = mensaje + hora_aux + ":" + min_aux + " " + String(Dia_semana[dia_prox_ciclo])+ "\",\"hora\":\"";// + "\"}";
    mensaje = mensaje+ String(Shora)+":" +String(Smin)+"\",\"fecha\":\"";
    mensaje = mensaje+ String(ano)+"-" +String(Smes)+"-"+String(Sdia) + "\"}";
    DEBUG_PRINTLN(mensaje);
    DEBUG_PRINTLN(mensaje.c_str());
    wifi_mqtt_publish(topic_valve.c_str(), mensaje.c_str());
    solicitud_web = 0;
    envia_horarios=1;
  }
  else if (counter > 0)
  {
    mensaje = "";
    clientId = "";
    //si ocurrió cambio envía a broker
    
    clientId += String(chipid);

    String topic_valve = clientId + "/out";
    DEBUG_PRINTLN(topic_valve);
    mensaje = "{\"estado_valvulas\":["; //1,1,1,1]}"";
    mensaje = mensaje + String(!valvulas[1]) + "," + String(!valvulas[2]) + "," + String(!valvulas[3]) + "," + String(!valvulas[4]) + "]}";

    wifi_mqtt_publish(topic_valve.c_str(), mensaje.c_str());
  }
  else if (modo_automatico_aux1 != modo_automatico) ///envía cambio de modos
  {
    mensaje = "";
    clientId = "";
    modo_automatico_aux1 = modo_automatico;
    
    
    clientId += String(chipid);

    String topic_valve = clientId + "/out";
    DEBUG_PRINTLN(topic_valve);
    mensaje = "{\"modo_activo\":"; //1}";
    mensaje = mensaje + String(modo_automatico) + "}";

    wifi_mqtt_publish(topic_valve.c_str(), mensaje.c_str());

    env_prox = 1;
  }
  else if (modo_nowc_aux1 != modo_nowc) ///envía cambio de modos
  {
    modo_nowc_aux1 = modo_nowc;
    
  }
  else if ((proximo_ciclo_aux1 != proximo_ciclo) || env_prox == 1) ///envía cambios de proximo ciclo a web
  {
    mensaje = "";
    clientId = "";
    proximo_ciclo_aux1 = proximo_ciclo;
    env_prox = 0;
    if (modo_automatico == 1)
    {

     
      clientId += String(chipid);
      String topic_valve = clientId + "/out";
      DEBUG_PRINTLN(topic_valve);
      mensaje = "{\"proximo_ciclo\":\""; //]}";
      min_aux = String(horario[6 * (proximo_ciclo - 1) + 1]);
      if (horario[6 * (proximo_ciclo - 1) + 1] < 10)
      {
        min_aux = "0" + String(horario[6 * (proximo_ciclo - 1) + 1]);
      }
      hora_aux = String(horario[6 * (proximo_ciclo - 1)]);
      if (horario[6 * (proximo_ciclo - 1)] < 10)
      {
        hora_aux = "0" + String(horario[6 * (proximo_ciclo - 1)]);
      }
      mensaje = mensaje + hora_aux + ":" + min_aux + " " + String(Dia_semana[dia_prox_ciclo]) + "\"}";
      wifi_mqtt_publish(topic_valve.c_str(), mensaje.c_str());
    }
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
