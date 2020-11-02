#ifdef fun_spiff

// agrega las funciones recuerda que arduino trae dos por defecto loop y setup
// nosotros las convertimos en fun_XXX_setup y fun_XXX_Loop (abreviatura de funciones) para que las puedas agregar
// em el setup y el loop de tu programa principal (main)

/***********************************************************************/
//Nombre de la funcion :autolog()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada cargar los parametros de modo
//para almacenarlos en la spiff
/***********************************************************************/
void autolog() //
{
  logFile.print("automode");
  logFile.print(",");
  logFile.print(modo_automatico);
  logFile.print(",");
  logFile.print(Sistema_apagado);
  logFile.print(",");
  logFile.print("\r\n");
  logFile.close();
}

/***********************************************************************/
//Nombre de la funcion :logValue2()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada cargar los parametros de CONFIG1
//para almacenarlos en la Spiff
/***********************************************************************/
void logValue2() //
{
  logFile.print("set1");
  logFile.print(",");
  logFile.print(ssid);
  logFile.print(",");
  logFile.print(password);
  logFile.print(",");
  logFile.print(ssid2);
  logFile.print(",");
  logFile.print(password2);
  logFile.print(",");
  logFile.print(String(MQTTHost));
  logFile.print(",");
  logFile.print(String(MQTTPort));
  logFile.print(",");
  logFile.print(String(MQTTUsername));
  logFile.print(",");
  logFile.print(String(MQTTPassword));
  logFile.print(",");
  logFile.print("1");
  logFile.print(",");
  logFile.print("\r\n");
  logFile.close();
}
/***********************************************************************/
//Nombre de la funcion :logValue3()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de cargar los parametros de CONFIG2
//para almacenarlos en la Spiff
/***********************************************************************/
void logValue3() //
{
  logFile.print("set2");
  logFile.print(",");
  logFile.print("1");
  logFile.print(",");
  logFile.print("1");
  logFile.print(",");
  logFile.print("1");
  logFile.print(",");
  logFile.print("1");
  logFile.print(",");
  logFile.print("1");
  logFile.print(",");
  logFile.print(__APN);
  logFile.print(",");
  logFile.print(__usrnm);
  logFile.print(",");
  logFile.print(__password);
  logFile.print(",");
  logFile.print("\r\n");
  logFile.close();
}


/***********************************************************************/
//Nombre de la funcion :save_modo_spiff()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de almacenar parametros modo en Spiff
/***********************************************************************/

void save_modo_spiff()
{
  if (SPIFFS.exists("/automode.txt")) //verifica si existe un archivo con el nombre config1.txt en el directorio raíz  del sistema spiff
  {
    SPIFFS.remove("/automode.txt"); //si existe lo elimina para la escritura de uno nuevo

    DEBUG_PRINTLN(F("ERRASED"));
  }
  DEBUG_PRINTLN("saving in SPIFFS:");
  fun_spiff_setup();
  logFile = SPIFFS.open("/automode.txt", FILE_APPEND); //abre archivo con el nombre config1.txt si no existe lo crea
  DEBUG_PRINTLN("automode.txt");

  if (logFile)
  {
    autolog(); //almacena los parametros de configuración 1 separados por coma','
    logFile.close();
    //ESP.restart();
  }
  else
  {
    logFile.close();
    DEBUG_PRINTLN("ERROR OPENNIG FILE");
    fun_spiff_setup();
  }
  logFile.close();
}

/***********************************************************************/
//Nombre de la funcion :save_config1_spiff()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de almacenar parametros CONFIG1 en Spiff
/***********************************************************************/

void save_config1_spiff()
{
  if (SPIFFS.exists("/config1.txt")) //verifica si existe un archivo con el nombre config1.txt en el directorio raíz  del sistema spiff
  {
    SPIFFS.remove("/config1.txt"); //si existe lo elimina para la escritura de uno nuevo

    DEBUG_PRINTLN(F("ERRASED"));
  }
  DEBUG_PRINTLN("saving in SPIFFS:");
  fun_spiff_setup();
  logFile = SPIFFS.open("/config1.txt", FILE_APPEND); //abre archivo con el nombre config1.txt si no existe lo crea
  DEBUG_PRINTLN("config1.txt");

  if (logFile)
  {
    logValue2(); //almacena los parametros de configuración 1 separados por coma','
    logFile.close();
    //ESP.restart();
  }
  else
  {
    logFile.close();
    DEBUG_PRINTLN("ERROR OPENNIG FILE");
    fun_spiff_setup();
  }
  logFile.close();
}
/***********************************************************************/
//Nombre de la funcion :save_config2_spiff()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de almacenar parametros CONFIG1 en Spiff
/***********************************************************************/

void save_config2_spiff()
{
  if (SPIFFS.exists("/config2.txt")) //verifica si existe un archivo con el nombre config2.txt en el directorio raíz  spiff
  {
    SPIFFS.remove("/config2.txt"); //si existe lo elimina para la escritura de uno nuevo
    DEBUG_PRINTLN(F("ERRASED"));
  }
  DEBUG_PRINTLN("saving in SPIFFS:");
  fun_spiff_setup();
  logFile = SPIFFS.open("/config2.txt", FILE_APPEND); //abre archivo con el nombre config2.txt si no existe lo crea
  DEBUG_PRINTLN("config2.txt");

  if (logFile)
  {
    logValue3(); //almacena los parametros de configuración 2 separados por coma','
    logFile.close();
    //ESP.restart();
  }
  else
  {
    logFile.close();
    DEBUG_PRINTLN("ERROR OPENNIG FILE");
    fun_spiff_setup();
  }
  logFile.close();
}
/***********************************************************************/
//Nombre de la funcion :read_spiffmodo()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de leer el archivo horarios en Spiff
/***********************************************************************/
void read_spiffmodo()
{
  fun_spiff_setup();
  logFile = SPIFFS.open("/automode.txt"); //abre archivo con el nombre automode.txt
  DEBUG_PRINT(F("OPENNING FILE"));
  int totalBytes = logFile.size();
  String cadena = "";
  if (logFile)
  {
    if (lastposition4 >= totalBytes)
      lastposition4 = 0;
    logFile.seek(lastposition4);
    while (logFile.available()) //lee el archivo linea por linea en este caso una sola línea de configuración
    {
      char caracter = logFile.read();
      cadena += String(caracter);
      lastposition4 = logFile.position();
      if (caracter == 10)
      { 
        break;
      }
    }

    logFile.close(); //cierra el archivo
    DEBUG_PRINT("length:");
    DEBUG_PRINTLN(cadena.length());
    DEBUG_PRINT("position:");
    DEBUG_PRINTLN(lastposition4);
    lastposition4=0;
    DEBUG_PRINT("position:");
    DEBUG_PRINTLN(lastposition4);
    DEBUG_PRINT("String readed:");
    DEBUG_PRINT(cadena);
    loadsdconfig(cadena); //ubicada en fun_loadconfig.ino --> verifica si el comando es valido y carga la configuracion en el dispositivo
  }
  else
  {
    logFile.close();
    DEBUG_PRINTLN(F("OPENNIG FILE ERROR"));
    fun_spiff_setup();
  }
}

/***********************************************************************/
//Nombre de la funcion :read_spiffconfig1()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de leer el archivo config1 en Spiff
/***********************************************************************/
void read_spiffconfig1()
{
  fun_spiff_setup();
  logFile = SPIFFS.open("/config1.txt"); //abre archivo con el nombre config1.txt
  DEBUG_PRINT(F("OPENNING FILE"));
  int totalBytes = logFile.size();
  String cadena = "";
  if (logFile)
  {
    if (lastposition >= totalBytes)
      lastposition = 0;
    logFile.seek(lastposition);
    while (logFile.available()) //lee el archivo linea por linea en este caso una sola línea de configuración
    {
      char caracter = logFile.read();
      cadena += String(caracter);
      lastposition = logFile.position();
      if (caracter == 10)
      {
        break;
      }
    }

    logFile.close(); //cierra el archivo
    DEBUG_PRINT("length:");
    DEBUG_PRINTLN(cadena.length());
    DEBUG_PRINT("position:");
    DEBUG_PRINTLN(lastposition);
    
    
    DEBUG_PRINT("String readed:");
    DEBUG_PRINT(cadena);
    loadsdconfig(cadena); //ubicada en fun_loadconfig.ino --> verifica si el comando es valido y carga la configuracion en el dispositivo
  }
  else
  {
    logFile.close();
    DEBUG_PRINTLN(F("OPENNIG FILE ERROR"));
    fun_spiff_setup();
  }
}
/***********************************************************************/
//Nombre de la funcion :read_spiffconfig2()
//Entrada: Ninguna
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de leer el archivo config2 en spiff
/***********************************************************************/
void read_spiffconfig2()
{
  fun_spiff_setup();
  logFile = SPIFFS.open("/config2.txt"); //abre archivo con el nombre config1.txt
  DEBUG_PRINT(F("OPENNING FILE"));

  int totalBytes = logFile.size();
  String cadena = "";
  if (logFile) //lee el archivo linea por linea en este caso una sola línea de configuración
  {
    if (lastposition2 >= totalBytes)
      lastposition2 = 0;
    logFile.seek(lastposition2);

    while (logFile.available())
    {
      char caracter = logFile.read();
      cadena += String(caracter);
      lastposition2 = logFile.position();

      if (caracter == 10) //caracter ASCII  para nueva linea
      {
        break;
      }
    }
    logFile.close(); //cierra el archivo
    DEBUG_PRINT("length:");
    DEBUG_PRINTLN(cadena.length());
    DEBUG_PRINT("position:");
    DEBUG_PRINTLN(lastposition2);
    DEBUG_PRINT("String readed:");
    DEBUG_PRINT(cadena);

    loadsdconfig(cadena); //ubicada en fun_loadconfig.ino --> verifica si el comando es valido y carga la configuracion en el dispositivo
  }
  else
  {
    logFile.close();
    DEBUG_PRINTLN(F("OPENNIG FILE ERROR"));
    fun_spiff_setup();
  }
}

void fun_spiff_setup() //escriba aqui las funciones que se ejecutaran una vez en su codigo cuando la funcion Fun_XXX_setup() sea llamada
{
  // coloque lo que necesite que se ejecute para iniciar su funcion ej:"activar dispositivos y modulos"
  if (!SPIFFS.begin())
  {
    DEBUG_PRINTLN("SPIFFS Mount Failed");
    return;
  }
}

void fun_spiff_loop()
{
  // coloque lo que quieras que se ejecute una y otra vez, recuerda incluir este
  //loop en tu main, si no jamas sera llamado, como ya no es una funcion loop
  //recuerda que al final se saldra de esta funcion y podra entrar a otras
  //tip: puedes usar un DEBUG.println("sali de fun_XXX_Loop()") para observar
  //como se comporta con otras funciones
}

//// puedes agregar tantas funciones como quieras
#else
void save_config1_spiff() {}
void save_config2_spiff() {}
void save_horarios_spiff() {}
void read_spiffconfig1() {}
void read_spiffconfig2() {}
void read_spiffhorarios() {}
void save_modo_spiff(){}
void read_spiffmodo() {}

void fun_spiff_setup() {} // agrega tu setup agregalo en el setup de tu main
void fun_spiff_loop() {}  // agrega tu loop, debes agregarlo en el loop de tu main
                          //FUNCIONES EJEMPLOS
                          //int fun_XXX_ReturnInt(int x, int y, char[120]){}
                          //void fun_XXX_ConEntradas(char x, char y, int z){}
                          //cualquier otra funcion puede agregarse si la funcion agregara tiene retornos debe agregarse
                          //como muestran los ejemplos
#endif
