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
    server.on("/fupload",  HTTP_POST,[](){ server.send(200);}, handleFileUpload);
    server.on("/delete",   File_Delete);
    server.on("/dir",      SD_dir);
  */
  server.on("/", dsetup);
  //server.on("/asetup", asetup);
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
{ minutosEnApMode=0;
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
    if(modo_nowc==1){
    String set1 = "set1," + String(ssid) + "," + String(password) + "," + String(ssid2) + "," + String(password2) + "," + String(MQTTHost) + "," + String(MQTTPort) + "," + String(MQTTUsername) + "," + String(MQTTPassword) + ",1,";
    loadsdconfig(set1);
    guardarAp=1;
    append_page_header();
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
  { 
    append_page_header();
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

/***********************************************************************/
//Nombre de la funcion :File_Download()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion: Esta funcion gestiona la descarga del archivo en la sd desde la webpage
/***********************************************************************/

void File_Download()
{

  if (server.args() > 0)
  {

    if (server.hasArg("download") && (server.arg(0) != ""))
      SD_file_download(server.arg(0));

    else if (server.hasArg("download") && (server.arg(1) != ""))
      SD_file_download(server.arg(1));
  }
  else
    SelectInput1("Enter filename to download", "download", "download");
  noAP = 0;
  tnoAP = 0;
}
/***********************************************************************/
//Nombre de la funcion :SD_file_download()
//Entrada: String filename
//Salida :Ninguna
//Descripcion: Esta funcion imprime en el cliente web la opcion DOWNLOAD
/***********************************************************************/
void SD_file_download(String filename)
{

  if (SD_present)
  {
    File download = SD.open("/" + filename);
    if (download)
    {
      server.sendHeader("Content-Type", "text/text");
      server.sendHeader("Content-Disposition", "attachment; filename=" + filename);
      server.sendHeader("Connection", "close");
      server.streamFile(download, "application/octet-stream");
      download.close();
    }
    download = SD.open("/" + filename + ".txt");
    if (download)
    {
      server.sendHeader("Content-Type", "text/text");
      server.sendHeader("Content-Disposition", "attachment; filename=" + filename + ".txt");
      server.sendHeader("Connection", "close");
      server.streamFile(download, "application/octet-stream");
      download.close();
    }
    else
      ReportFileNotPresent("download");
  }
  else
    ReportSDNotPresent();
  noAP = 0;
  tnoAP = 0;
}
/***********************************************************************/
//Nombre de la funcion :File_Upload()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion imprime en el cliente web la opcion UPLOAD
/***********************************************************************/
void File_Upload()
{

  DEBUG_PRINTLN("File upload stage-1");
  append_page_header();
  webpage += F("<h3>Select File to Upload</h3>");
  webpage += F("<FORM action='/fupload' method='post' enctype='multipart/form-data'>");
  webpage += F("<input class='buttons'  type='file' name='fupload' id = 'fupload' value=''><br>");
  webpage += F("<br><button class='buttons'  type='submit'>Upload File</button><br>");
  webpage += F("<a href='/'>[Back]</a><br><br>");
  append_page_footer();
  DEBUG_PRINTLN("File upload stage-2");
  server.send(200, "text/html", webpage);
  noAP = 0;
  tnoAP = 0;
}
/***********************************************************************/
//Nombre de la funcion :handleFileUpload()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de subir un nuevo archivo a la memoria SD
/***********************************************************************/
File UploadFile;
void handleFileUpload()
{
  DEBUG_PRINTLN("File upload stage-3");
  HTTPUpload &uploadfile = server.upload();

  if (uploadfile.status == UPLOAD_FILE_START)
  {
    DEBUG_PRINTLN("File upload stage-4");
    String filename = uploadfile.filename;
    if (!filename.startsWith("/"))
      filename = "/" + filename;
    DEBUG_PRINT("Upload File Name: ");
    DEBUG_PRINTLN(filename);
    SD.remove(filename);                        //Elimina la versión anterior del archivo
    UploadFile = SD.open(filename, FILE_WRITE); // Abre el archivo para escribir en el SPIFFS (Si no existe ,lo crea)
    filename = String();
  }
  else if (uploadfile.status == UPLOAD_FILE_WRITE)
  {
    DEBUG_PRINTLN("File upload stage-5");
    if (UploadFile)
      UploadFile.write(uploadfile.buf, uploadfile.currentSize); // Escribe los bytes recibidos en el archivo
  }
  else if (uploadfile.status == UPLOAD_FILE_END)
  {
    if (UploadFile) // Si el archivo fue creado exitosamente
    {
      UploadFile.close(); // Cierra el archivo nuevamente
      DEBUG_PRINT("Upload Size: ");
      DEBUG_PRINTLN(uploadfile.totalSize);
      webpage = "";
      append_page_header();
      webpage += F("<h3>File was successfully uploaded</h3>");
      webpage += F("<h2>Uploaded File Name: ");
      webpage += uploadfile.filename + "</h2>";
      webpage += F("<h2>File Size: ");
      webpage += file_size(uploadfile.totalSize) + "</h2><br>";
      append_page_footer();
      server.send(200, "text/html", webpage);
      noAP = 0;
      tnoAP = 0;
    }
    else
    {
      ReportCouldNotCreateFile("upload");
    }
  }
}
/***********************************************************************/
//Nombre de la funcion :SD_dir()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion añade en el cliente web la opcion de ver el directorio raiz de la SD
/***********************************************************************/
void SD_dir()
{
  if (SD_present)
  { //ini_almacenamiento();
    File root = SD.open("/");
    if (root)
    {
      root.rewindDirectory();
      SendHTML_Header();
      webpage += F("<h3 class='rcorners_m'>SD Card Contents</h3><br>");
      webpage += F("<table align='center'>");
      webpage += F("<tr><th>Name/Type</th><th>Type File/Dir</th><th>File Size</th></tr>");
      printDirectory("/", 0);
      webpage += F("</table>");
      SendHTML_Content();
      root.close();
    }
    else
    {
      SendHTML_Header();
      webpage += F("<h3>No Files Found</h3>");
    }
    append_page_footer();
    SendHTML_Content();
    SendHTML_Stop(); //detiene el envío si es necesario
    noAP = 0;
    tnoAP = 0;
  }
  else
    ReportSDNotPresent();
}
/***********************************************************************/
//Nombre de la funcion :printDirectory()
//Entrada: const char *dirname, uint8_t levels
//Salida :Ninguna
//Descripcion:Esta funcion imprime en el cliente web el directorio RAIZ de la SD
/***********************************************************************/
void printDirectory(const char *dirname, uint8_t levels)
{
  File root = SD.open(dirname);
#ifdef ESP8266
  root.rewindDirectory();
#endif
  if (!root)
  {
    return;
  }
  if (!root.isDirectory())
  {
    return;
  }
  File file = root.openNextFile();
  while (file)
  {
    if (webpage.length() > 1000)
    {
      SendHTML_Content();
    }
    if (file.isDirectory())
    {
      DEBUG_PRINTLN(String(file.isDirectory() ? "Dir " : "File ") + String(file.name()));
      webpage += "<tr><td>" + String(file.isDirectory() ? "Dir" : "File") + "</td><td>" + String(file.name()) + "</td><td></td></tr>";
      printDirectory(file.name(), levels - 1);
    }
    else
    {
      webpage += "<tr><td>" + String(file.name()) + "</td>";
      DEBUG_PRINT(String(file.isDirectory() ? "Dir " : "File ") + String(file.name()) + "\t");
      webpage += "<td>" + String(file.isDirectory() ? "Dir" : "File") + "</td>";
      int bytes = file.size();
      String fsize = "";
      if (bytes < 1024)
        fsize = String(bytes) + " B";
      else if (bytes < (1024 * 1024))
        fsize = String(bytes / 1024.0, 3) + " KB";
      else if (bytes < (1024 * 1024 * 1024))
        fsize = String(bytes / 1024.0 / 1024.0, 3) + " MB";
      else
        fsize = String(bytes / 1024.0 / 1024.0 / 1024.0, 3) + " GB";
      webpage += "<td>" + fsize + "</td></tr>";
      DEBUG_PRINTLN(String(fsize));
    }
    file = root.openNextFile();
  }
  file.close();
  noAP = 0;
  tnoAP = 0;
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
/***********************************************************************/
//Nombre de la funcion :SelecInput()
//Entrada: String heading1,String command , String arg_calling_name
//Salida :Ninguna
//Descripcion:Esta funcion gestiona entradas del webpage
/***********************************************************************/
void SelectInput(String heading1, String command, String arg_calling_name)
{
  SendHTML_Header();
  webpage += F("<h3>");
  webpage += heading1 + "</h3>";
  webpage += F("<FORM action='/");
  webpage += command + "' method='post'>";
  webpage += F("<input type='text' name='");
  webpage += arg_calling_name;
  webpage += F("' value=''><br>");
  webpage += F("<type='submit' name='");
  webpage += arg_calling_name;
  webpage += F("' value=''><br><br>");
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
  noAP = 0;
  tnoAP = 0;
}
/***********************************************************************/
//Nombre de la funcion :SelecInput1()
//Entrada: String heading1,String command , String arg_calling_name
//Salida :Ninguna
//Descripcion:Esta funcion gestiona entradas del webpage en menu download
/***********************************************************************/
void SelectInput1(String heading1, String command, String arg_calling_name)
{
  SendHTML_Header();
  webpage += F("<h3>");
  webpage += heading1 + "</h3>";
  webpage += F("<FORM action='/");
  webpage += command + "' method='post'>";
  webpage += F("<input type='text' name='");
  webpage += arg_calling_name;
  webpage += F("' value=''><br>");
  webpage += F("<type='submit' name='");
  webpage += arg_calling_name;
  webpage += F("' value=''><br>");
  webpage += F("<p>Register Date:<input type='date' name='");
  webpage += arg_calling_name;
  webpage += F("' value=''><button class='buttons' type='submit'>Download</button></p>");

  webpage += F("</FORM>");
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
  noAP = 0;
  tnoAP = 0;
}
/***********************************************************************/
//Nombre de la funcion :ReportSDNotPresent()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion reporta si la sd no esta presente en el dispositivo
/***********************************************************************/
void ReportSDNotPresent()
{
  SendHTML_Header();
  webpage += F("<h3>No SD Card present</h3>");
  webpage += F("<a href='/'>[Back]</a><br><br>");
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
  noAP = 0;
  tnoAP = 0;
}
/***********************************************************************/
//Nombre de la funcion :ReportFileNotPresent()
//Entrada: String target
//Salida :Ninguna
//Descripcion:Esta funcion reporta si el archivo solicitado no se encuentra en la SD
/***********************************************************************/
void ReportFileNotPresent(String target)
{
  SendHTML_Header();
  webpage += F("<h3>File does not exist</h3>");
  webpage += F("<a href='/");
  webpage += target + "'>[Back]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
  noAP = 0;
  tnoAP = 0;
}
/***********************************************************************/
//Nombre de la funcion :ReportCouldNotCreateFile()
//Entrada: String target
//Salida :Ninguna
//Descripcion:Esta funcion reporta si no es posible crear el archivo en la SD
/***********************************************************************/
void ReportCouldNotCreateFile(String target)
{
  SendHTML_Header();
  webpage += F("<h3>Could Not Create Uploaded File (write-protected?)</h3>");
  webpage += F("<a href='/");
  webpage += target + "'>[Back]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
  noAP = 0;
  tnoAP = 0;
}
/***********************************************************************/
//Nombre de la funcion :file_size()
//Entrada: int bytes
//Salida :String
//Descripcion:Esta funcion es la encargada de mostrar el tamaño de los archivos en la SD
/***********************************************************************/
String file_size(int bytes)
{
  String fsize = "";
  if (bytes < 1024)
    fsize = String(bytes) + " B";
  else if (bytes < (1024 * 1024))
    fsize = String(bytes / 1024.0, 3) + " KB";
  else if (bytes < (1024 * 1024 * 1024))
    fsize = String(bytes / 1024.0 / 1024.0, 3) + " MB";
  else
    fsize = String(bytes / 1024.0 / 1024.0 / 1024.0, 3) + " GB";
  return fsize;
}

/*
EndPoint para aplicacion Móvil

*/

void getData()
{ minutosEnApMode=0;
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
    if(modo_nowc==1){
    String set1 = "set1," + String(ssid) + "," + String(password) + "," + String(ssid2) + "," + String(password2) + "," + String(MQTTHost) + "," + String(MQTTPort) + "," + String(MQTTUsername) + "," + String(MQTTPassword) + ",1,";
    loadsdconfig(set1);
    guardarAp=1;
    append_page_header();
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
  { 
    append_page_header();
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

