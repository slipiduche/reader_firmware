/***********************************************************************/
//Nombre de la funcion :web_setup()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion contiene los Comandos de Servidor y comandos de
//fin de solicitud
/***********************************************************************/

void web_setup(void)
{
  DEBUG_PRINTLN("Creating Accesspoint");
  WiFi.softAP(ssid2, password2); //Crea Access Point
  DEBUG_PRINT("AP IP address:\t");
  DEBUG_PRINTLN(WiFi.softAPIP());
  /****Comandos de Servidor****/
  //server.on("/",         HomePage);
  /*
    server.on("/download", File_Download);
    server.on("/upload",   File_Upload);
    
    server.on("/delete",   File_Delete);
    server.on("/dir",      SD_dir);
  */
  server.on("/", dsetup);
  server.on("/getData", getData);
  server.on(
      "/putData", putData);///debe ser de tipo get
  /****Comandos de Fin de Solicitud****/
  server.begin();
  DEBUG_PRINTLN("HTTP server started");
}

/***********************************************************************/
//Nombre de la funcion :web_loop()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion escucha las conexiones de los clientes.
/***********************************************************************/
#ifdef AP
void apweb_loop(void)
{
  server.handleClient(); //
}
#else
void apweb_loop()
{
}
#endif

/***********************************************************************/
//Nombre de la funcion :dsetup()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion imprime en el cliente web la opcion SETUP
/***********************************************************************/
void dsetup()
{
  minutosEnApMode = 0;
  if (server.args() > 6)
  {

    if (server.hasArg(String("SSID")))
    {

      memset(ssid, '\0', sizeof(ssid)); //Inicializacion de variables para escritura y almacenamiento de parametros de configuración
      (server.arg(String("SSID"))).toCharArray(ssid, (server.arg(String("SSID"))).length() + 1);
    }
    if (server.hasArg(String("Password")))
    {

      //Inicializacion de variables para escritura y almacenamiento de parametros de configuración
      memset(password, '\0', sizeof(password)); //
      (server.arg(String("Password"))).toCharArray(password, (server.arg(String("Password"))).length() + 1);
    }
    if (server.hasArg(String("APSSID")))
    {

      //Inicializacion de variables para escritura y almacenamiento de parametros de configuración
      memset(ssid2, '\0', sizeof(ssid2)); //
      (server.arg(String("APSSID"))).toCharArray(ssid2, (server.arg(String("APSSID"))).length() + 1);
    }
    if (server.hasArg(String("AP_Password")))
    {

      //Inicializacion de variables para escritura y almacenamiento de parametros de configuración
      memset(password2, '\0', sizeof(password2)); //
      (server.arg(String("AP_Password"))).toCharArray(password2, (server.arg(String("AP_Password"))).length() + 1);
    }
    if (server.hasArg(String("WEB_Host")))
    {

      //Inicializacion de variables para escritura y almacenamiento de parametros de configuración
      memset(host, '\0', sizeof(MQTTHost)); //
      (server.arg(String("WEB_Host"))).toCharArray(MQTTHost, (server.arg(String("WEB_Host"))).length() + 1);
    }
    if (server.hasArg(String("URL1")))
    {

      memset(MQTTPort, '\0', sizeof(MQTTPort)); //
      (server.arg(String("URL1"))).toCharArray(MQTTPort, (server.arg(String("URL1"))).length() + 1);
    }
    if (server.hasArg(String("URL2")))
    {

      memset(MQTTUsername, '\0', sizeof(MQTTUsername)); //
      (server.arg(String("URL2"))).toCharArray(MQTTUsername, (server.arg(String("URL2"))).length() + 1);
    }
    if (server.hasArg(String("URL3")))
    {

      memset(MQTTPassword, '\0', sizeof(MQTTPassword)); //
      (server.arg(String("URL3"))).toCharArray(MQTTPassword, (server.arg(String("URL2"))).length() + 1);
    }

    if (modo_nowc == 1)
    { 
      String set1 = "set1," + String(ssid) + "," + String(password) + "," + String(ssid2) + "," + String(password2) + "," + String(MQTTHost) + "," + String(MQTTPort) + "," + String(MQTTUsername) + "," + String(MQTTPassword) + ",1,";
      loadsdconfig(set1);
      guardarAp = 1;
      SendHTML_Header();
      webpage += F("<script>");
      webpage += F("  function pulsar(e) {");
      webpage += F("    tecla = (document.all) ? e.keyCode :e.which;");
      webpage += F("    return (tecla!=13);");
      webpage += F("  }");
      webpage += F("  </script>");
      webpage += F("<h3>Parameters Setup</h3>");
      webpage += F("<FORM action='/' method='post' enctype='multipart/form-data'>");
      webpage += F("<p>Nompre de Red:<input type='text' name='SSID' value='");
      webpage += String(ssid);
      webpage += F("' minlength='4' maxlength='60' required onkeypress=\"return pulsar(event)\"></p>");
      webpage += F("<p>Contraseña:<input type='text' name='Password' value='");
      webpage += String(password);
      webpage += F("' minlength='8' maxlength='60' required onkeypress=\"return pulsar(event)\"></p>");
      webpage += F("<p>IP de Red: <b>");
      webpage += ipRed;
      webpage += F("</b></p>");
      webpage += F("<p>Nombre de Red AP:<input type='text' name='APSSID' value='");
      webpage += String(ssid2);
      webpage += F("' minlength='4' maxlength='60' required onkeypress=\"return pulsar(event)\"></p>");
      webpage += F("<p>Contraseña AP:<input type='text' name='AP_Password' value='");
      webpage += String(password2);
      webpage += F("' minlength='8' maxlength='60' required onkeypress=\"return pulsar(event)\"></p>");
      webpage += F("<p>IP de Red AP: <b>");
      webpage += F("192.168.4.1</b></p>");
      webpage += F("<p>MQTT_Host:<input type='text' name='WEB_Host' value='");
      webpage += String(MQTTHost);
      webpage += F("' required onkeypress=\"return pulsar(event)\"></p>");
      webpage += F("<p>MQTT_Port:<input type='text' name='URL1' value='");
      webpage += String(MQTTPort);
      webpage += F("' required onkeypress=\"return pulsar(event)\"></p>");
      webpage += F("<p>MQTT_UserName:<input type='text' name='URL2' value='");
      webpage += String(MQTTUsername);
      webpage += F("' onkeypress=\"return pulsar(event)\"></p>");
      webpage += F("<p>MQTT_Password:<input type='text' name='URL3' value='");
      webpage += String(MQTTPassword);
      webpage += F("' onkeypress=\"return pulsar(event)\"></p>");
      webpage += F("<p>ChipID: <b>");
      webpage += String(chipid);
      webpage += F("</b></p>");
      webpage += F("<br>Reiniciando... Para reingresar espere que el dispositivo reactive Modo AP.<br>");
      webpage += F("</FORM>");
      webpage += F("<a href='/'>[Back]</a><br><br> ");
      append_page_footer();
      SendHTML_Content();
      SendHTML_Stop(); // detiene el envío si es necesario
    }
  }
  else
  { SendHTML_Header();
    webpage += F("<script>");
    webpage += F("  function pulsar(e) {");
    webpage += F("    tecla = (document.all) ? e.keyCode :e.which;");
    webpage += F("    return (tecla!=13);");
    webpage += F("  }");
    webpage += F("  </script>");
    webpage += F("<h3>Parameters Setup</h3>");
    webpage += F("<FORM action='/' method='post' enctype='multipart/form-data'>");
    webpage += F("<p>Nompre de Red:<input type='text' name='SSID' value='");
    webpage += String(ssid);
    webpage += F("' minlength='4' maxlength='60' required onkeypress=\"return pulsar(event)\"></p>");
    webpage += F("<p>Contraseña:<input type='text' name='Password' value='");
    webpage += String(password);
    webpage += F("' minlength='8' maxlength='60' required onkeypress=\"return pulsar(event)\"></p>");
    webpage += F("<p>IP de Red: <b>");
    webpage += ipRed;
    webpage += F("</b></p>");
    webpage += F("<p>Nombre de Red AP:<input type='text' name='APSSID' value='");
    webpage += String(ssid2);
    webpage += F("' minlength='4' maxlength='60' required onkeypress=\"return pulsar(event)\"></p>");
    webpage += F("<p>Contraseña AP:<input type='text' name='AP_Password' value='");
    webpage += String(password2);
    webpage += F("' minlength='8' maxlength='60' required onkeypress=\"return pulsar(event)\"></p>");
    webpage += F("<p>IP de Red AP: <b>");
    webpage += F("192.168.4.1</b></p>");
    webpage += F("<p>MQTT_Host:<input type='text' name='WEB_Host' value='");
    webpage += String(MQTTHost);
    webpage += F("' required onkeypress=\"return pulsar(event)\"></p>");
    webpage += F("<p>MQTT_Port:<input type='text' name='URL1' value='");
    webpage += String(MQTTPort);
    webpage += F("' required onkeypress=\"return pulsar(event)\"></p>");
    webpage += F("<p>MQTT_UserName:<input type='text' name='URL2' value='");
    webpage += String(MQTTUsername);
    webpage += F("' onkeypress=\"return pulsar(event)\"></p>");
    webpage += F("<p>MQTT_Password:<input type='text' name='URL3' value='");
    webpage += String(MQTTPassword);
    webpage += F("' onkeypress=\"return pulsar(event)\"></p>");
    webpage += F("<p>ChipID: <b>");
    webpage += String(chipid);
    webpage += F("</b></p>");
    webpage += F("<br><button class='buttons'  type='submit'>Enviar Parametros</button><br>");
    webpage += F("</FORM>");
    webpage += F("<a href='/'>[Back]</a><br><br> ");
    append_page_footer();
    SendHTML_Content();
    SendHTML_Stop(); // detiene el envío si es necesario
  }
}
//Nombre de la funcion :SendHTML_Header()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion imprime html header
/***********************************************************************/
void SendHTML_Header()
{
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  append_page_header();
  server.sendContent(webpage);
  webpage = "";
}
void SendJson(String json)
{
  // server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  // server.sendHeader("Pragma", "no-cache");
  // server.sendHeader("Expires", "-1");
  // server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "application/json", json);
  
  
}
/***********************************************************************/
//Nombre de la funcion :SentHTML_Content()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion imprime html content
/***********************************************************************/
void SendHTML_Content()
{
  server.sendContent(webpage);
  webpage = "";
}
/***********************************************************************/
//Nombre de la funcion :SendHTML_Stop()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion detiene el cliente web
/***********************************************************************/
void SendHTML_Stop()
{
  server.sendContent("");
  server.client().stop();
}

/*
EndPoint para aplicacion Móvil

*/

void getData()
{ 
  webpage = "";
  webpage += F("{\"SSID\":\"");
  webpage += String(ssid);
  webpage += F("\",\"PASSWORD\":\"");
  webpage += String(password);
  webpage += ("\",\"CHIPID\":\"");
  webpage += String(chipid);
  webpage += F("\",\"NAME\":\"");
  webpage += String(devName);
  webpage += F("\"}");
  SendJson(webpage);
  
  SendHTML_Stop(); // detiene el envío si es necesario
}

void putData()
{ Serial.print("recbiendo datos GET....");
  Serial.print(server.args());
  Serial.print(server.arg("plain"));
  String body = server.arg("plain");
  Serial.print(body);
  if (server.hasArg(String("SSID")))
  {

    if (server.hasArg(String("SSID")))
    {

      memset(ssid, '\0', sizeof(ssid)); //Inicializacion de variables para escritura y almacenamiento de parametros de configuración
      (server.arg(String("SSID"))).toCharArray(ssid, (server.arg(String("SSID"))).length() + 1);
    }
    if (server.hasArg(String("PASSWORD")))
    {

      //Inicializacion de variables para escritura y almacenamiento de parametros de configuración
      memset(password, '\0', sizeof(password)); //
      (server.arg(String("PASSWORD"))).toCharArray(password, (server.arg(String("Password"))).length() + 1);
    }
    if (server.hasArg(String("NAME")))
    {

      //Inicializacion de variables para escritura y almacenamiento de parametros de configuración
      memset(devName, '\0', sizeof(devName)); //
      (server.arg(String("NAME"))).toCharArray(devName, (server.arg(String("NAME"))).length() + 1);
    }
    if (modo_nowc == 1)
    { Serial.println("configurando....");
      String set1 = "set1," + String(ssid) + "," + String(password) + "," + String(ssid2) + "," + String(password2) + "," + String(MQTTHost) + "," + String(MQTTPort) + "," + String(MQTTUsername) + "," + String(MQTTPassword) + ",1,";
      loadsdconfig(set1);
      guardarAp = 1;
      //SendJson_Header();
      webpage = "";
      webpage += F("{\"MESSAGE\":\"SUCCESS\"}");
      SendJson(webpage);
      //SendHTML_Content();
      SendHTML_Stop(); // detiene el envío si es necesario
    }
  }
  else
  {
    
    server.send(405, "application/json", F("{\"ERROR\":\"Method Not Allowed\"}"));
    SendHTML_Content();
    SendHTML_Stop();
  }
}
