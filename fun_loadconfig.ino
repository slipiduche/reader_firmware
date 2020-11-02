

/***********************************************************************/
//Nombre de la funcion :loadsdconfig)
//Entrada: String datar
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de cargar la configuracion leida desde la SD
//la configuracion leida tiene la siguiente estructura:
//set1,wifistationname,wifistationpswd,wifiapname,wifiappswd,mqtthost,mqttport,mqttusername,mqttdevname,apn,apnusr,apnpwd,
//set2,xxxx,xxxx,xxxx,xxxx,xxxx,APN,USERAPN,PASSWORDAPN,
/***********************************************************************/
void loadsdconfig(String datar)
{
  if (datar.startsWith("set1") && (datar.length() > 40)) //si es un comando valido (comienza por set1)
  {                                                      //Se delimitan y organizan los datos separados por comas
    int primeracoma = datar.indexOf(',');
    int segundacoma = datar.indexOf(',', primeracoma + 1);
    int terceracoma = datar.indexOf(',', segundacoma + 1);
    int cuartacoma = datar.indexOf(',', terceracoma + 1);
    int coma5 = datar.indexOf(',', cuartacoma + 1);
    int coma6 = datar.indexOf(',', coma5 + 1);
    int coma7 = datar.indexOf(',', coma6 + 1);
    int coma8 = datar.indexOf(',', coma7 + 1);
    int coma9 = datar.indexOf(',', coma8 + 1);
    int coma10 = datar.indexOf(',', coma9 + 1);
    int coma11 = datar.indexOf(',', coma10 + 1);
    int coma12 = datar.indexOf(',', coma11 + 1);
    //Creación de variables auxiliares
    String ssid_aux = datar.substring(primeracoma + 1, segundacoma);
    String password_aux = datar.substring(segundacoma + 1, terceracoma);
    String ssid2_aux = datar.substring(terceracoma + 1, cuartacoma);
    String password2_aux = datar.substring(cuartacoma + 1, coma5);
    String host_aux = datar.substring(coma5 + 1, coma6);
    String port_aux = datar.substring(coma6 + 1, coma7);
    String usermqtt_aux = datar.substring(coma7 + 1, coma8);
    String pwdmqtt_aux = datar.substring(coma8 + 1, coma9);
    
    //String mqttdevname = datar.substring(coma9 + 1, coma10);

    //Inicializacion de variables para escritura y almacenamiento de parametros de configuración
    memset(ssid, '\0', sizeof(ssid));                 //
    memset(password, '\0', sizeof(password));         //
    memset(ssid2, '\0', sizeof(ssid2));               //
    memset(password2, '\0', sizeof(password2));       //
    memset(MQTTHost, '\0', sizeof(MQTTHost));                 //
    memset(MQTTPort, '\0', sizeof(MQTTPort));         //
    memset(MQTTUsername, '\0', sizeof(MQTTUsername)); //
    memset(MQTTPassword, '\0', sizeof(MQTTPassword)); //

    //Escritura de variables auxiliares a los parametros de configuración.
    ssid_aux.toCharArray(ssid, ssid_aux.length() + 1);
    password_aux.toCharArray(password, password_aux.length() + 1);
    ssid2_aux.toCharArray(ssid2, ssid2_aux.length() + 1);
    password2_aux.toCharArray(password2, password2_aux.length() + 1);
    host_aux.toCharArray(MQTTHost, host_aux.length() + 1);
    port_aux.toCharArray(MQTTPort, port_aux.length() + 1);
    usermqtt_aux.toCharArray(MQTTUsername, usermqtt_aux.length() + 1);
    pwdmqtt_aux.toCharArray(MQTTPassword, pwdmqtt_aux.length() + 1);
    // mqttdevname.toCharArray(MQTTClientID,mqttdevname.length()+1);

    DEBUG_PRINTLN();
    DEBUG_PRINT("ssid:");
    DEBUG_PRINTLN(String(ssid));
    DEBUG_PRINT("password:");
    DEBUG_PRINTLN(String(password));
    DEBUG_PRINT("ssid2:");
    DEBUG_PRINTLN(String(ssid2));
    DEBUG_PRINT("password2:");
    DEBUG_PRINTLN(String(password2));
    DEBUG_PRINT("host:");
    DEBUG_PRINTLN(String(MQTTHost));
    DEBUG_PRINT("port:");
    DEBUG_PRINTLN(String(MQTTPort));
    DEBUG_PRINT("username:");
    DEBUG_PRINTLN(String(MQTTUsername));
    DEBUG_PRINT("pwd:");
    DEBUG_PRINTLN(String(MQTTPassword));
    DEBUG_PRINTLN("LOAD CONFIG1 SUCCESS");
  }

  else if (datar.startsWith("set2") && (datar.length() > 40)) //Si es un comando valido (comienza por set2)
  {                                                           //Se delimitan y organizan los datos separados por comas
    int primeracoma = datar.indexOf(',');
    int segundacoma = datar.indexOf(',', primeracoma + 1);
    int terceracoma = datar.indexOf(',', segundacoma + 1);
    int cuartacoma = datar.indexOf(',', terceracoma + 1);
    int coma5 = datar.indexOf(',', cuartacoma + 1);
    int coma6 = datar.indexOf(',', coma5 + 1);
    int coma7 = datar.indexOf(',', coma6 + 1);
    int coma8 = datar.indexOf(',', coma7 + 1);
    int coma9 = datar.indexOf(',', coma8 + 1);

    //Creación de variables auxiliares
    //Lectura de intervalos para cada sensor
    String Read_intervalVL53 = datar.substring(primeracoma + 1, segundacoma);   //Intervalo VL53 4 segundos mínimo
    String Read_intervalB707C = datar.substring(segundacoma + 1, terceracoma);  //Intervalo B707C 4 segundos mínimo
    String Read_intervalDS18B20 = datar.substring(terceracoma + 1, cuartacoma); //Intervalo DS18B20 1 seg minimo
    String FREQ_intervalrdt = datar.substring(cuartacoma + 1, coma5);           //Intervalo de transmision de datos
    String FREQ_intervalsds = datar.substring(coma5 + 1, coma6);                //Frecuencia de almacenamiento en SD
    String apn_aux = datar.substring(coma6 + 1, coma7);                         //APN red movil
    String userapn_aux = datar.substring(coma7 + 1, coma8);                     //USER APN red movil
    String apnpwd_aux = datar.substring(coma8 + 1, coma9);                      //PASSWORD APN red  movil

    //Inicializacion de variables para escritura y almacenamiento de parametros de configuración
    memset(__APN, '\0', sizeof(__APN));
    memset(__usrnm, '\0', sizeof(__usrnm));
    memset(__password, '\0', sizeof(__password));
    //Escritura de variables auxiliares a los parametros de configuración.
    apn_aux.toCharArray(__APN, apn_aux.length() + 1);
    userapn_aux.toCharArray(__usrnm, userapn_aux.length() + 1);
    apnpwd_aux.toCharArray(__password, apnpwd_aux.length() + 1);

    DEBUG_PRINT("__APN:");
    DEBUG_PRINTLN(String(__APN));
    DEBUG_PRINT("__usrnm:");
    DEBUG_PRINTLN(String(__usrnm));
    DEBUG_PRINT("__password:");
    DEBUG_PRINTLN(String(__password));
    DEBUG_PRINTLN("LOAD CONFIG2 SUCCESS");
  }
  
  
}
