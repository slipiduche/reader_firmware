String Shora = "", Sfecha = "", Sday = "", Smonth = "", Syear = "", Shr = "", Smin = "";

char buf[20], buf1[20], buf2[20], buf3[20];
/***********************************************************************/
//Nombre de la funcion :load_hmi_config()
//Entrada: String datar
//Salida :Ninguna
//Descripcion:Esta funcion es la encargada de cargar la configuracion leida desde la SD
//la configuracion leida tiene la siguiente estructura:

/***********************************************************************/
void load_hmi_config(String datar)
{
  String hora_inicial = "";
  String min_inicial = "";
  String hora_final = "";
  String min_final = "";
  if (datar.startsWith("set2") && (datar.length() > 40)) //si es un comando valido (comienza por set1)
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
    String port_aux = datar.substring(coma8 + 1, coma9);
    String usermqtt_aux = datar.substring(coma9 + 1, coma10);
    String pwdmqtt_aux = datar.substring(coma10 + 1, coma11);

    //String mqttdevname = datar.substring(coma9 + 1, coma10);

    //Inicializacion de variables para escritura y almacenamiento de parametros de configuración
    memset(ssid, '\0', sizeof(ssid));                 //
    memset(password, '\0', sizeof(password));         //
    memset(ssid2, '\0', sizeof(ssid2));               //
    memset(password2, '\0', sizeof(password2));       //
    memset(MQTTHost, '\0', sizeof(MQTTHost));         //
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
    save_config1_spiff();
    ESP.restart();
  }

  else if (datar.startsWith("set4") && (datar.length() > 5)) //si es un comando valido (comienza por set1)
  {                                                          //Se delimitan y organizan los datos separados por comas
    int primeracoma = datar.indexOf(',');
    int segundacoma = datar.indexOf(',', primeracoma + 1);
    int terceracoma = datar.indexOf(',', segundacoma + 1);
    int cuartacoma = datar.indexOf(',', terceracoma + 1);
    int quintacoma = datar.indexOf(',', cuartacoma + 1);
    int sextacoma = datar.indexOf(',', quintacoma + 1);
    int septimacoma = datar.indexOf(',', sextacoma + 1);
    int octavacoma = datar.indexOf(',', septimacoma + 1);
    int novenacoma = datar.indexOf(',', octavacoma + 1);

    
    modo_automatico=1;
    //Creación de variables auxiliares
    String numero_horario_aux = datar.substring(primeracoma + 1, segundacoma);
    int horario_act_aux = (datar.substring(segundacoma + 1, terceracoma)).toInt();
    numero_horarios = numero_horario_aux.toInt();
    if (numero_horarios > 0)
    {
      DEBUG_PRINTLN();
      DEBUG_PRINT("horarios,");
      DEBUG_PRINT(numero_horarios);
      DEBUG_PRINT(",");
      DEBUG_PRINT(horario_act_aux);
      DEBUG_PRINT(",");
         
        
        horario[(6*(horario_act_aux-1))] = (datar.substring(terceracoma + 1, cuartacoma)).toInt();//hora inicial
        DEBUG_PRINT(String(horario[(6*(horario_act_aux-1))]));
        DEBUG_PRINT(",");
        horario[(6*(horario_act_aux-1))+1] = (datar.substring(cuartacoma + 1, quintacoma)).toInt();//minuto inicial
        DEBUG_PRINT(String(horario[(6*(horario_act_aux-1))+1]));
        DEBUG_PRINT(",");
        horario[(6*(horario_act_aux-1))+2] = (datar.substring(quintacoma + 1, sextacoma)).toInt();//hora final
        DEBUG_PRINT(String(horario[(6*(horario_act_aux-1))+2]));
        DEBUG_PRINT(",");
        horario[(6*(horario_act_aux-1))+3] = (datar.substring(sextacoma + 1, septimacoma)).toInt();//minuto final
        DEBUG_PRINT(String(horario[(6*(horario_act_aux-1))+3]));
        DEBUG_PRINT(",");
        horario[(6*(horario_act_aux-1))+4] = (datar.substring(septimacoma + 1, octavacoma)).toInt();//dias
        DEBUG_PRINT(String(horario[(6*(horario_act_aux-1))+4]));
        DEBUG_PRINT(",");
        horario[(6*(horario_act_aux-1))+5] = (datar.substring(octavacoma + 1, novenacoma)).toInt();//valvulas
        DEBUG_PRINT(String(horario[(6*(horario_act_aux-1))+5]));
        DEBUG_PRINTLN(",");
        
        
      if (horario_act_aux==numero_horarios)
      { Sistema_apagado=0;
        DEBUG_PRINTLN('guardando horarios');
        save_horarios_spiff();
        save_modo_spiff();
        solicitud_web = 1;
      }
      DEBUG_PRINTLN();
      DEBUG_PRINTLN("LOAD horarios SUCCESS");
    }
  }
  else if (datar.startsWith("set3") && (datar.length() > 5))
  {
    int primeracoma = datar.indexOf(',');
    int segundacoma = datar.indexOf(',', primeracoma + 1);
    int terceracoma = datar.indexOf(',', segundacoma + 1);
    // int cuartacoma = datar.indexOf(',', terceracoma + 1);
    //lectura de hora hmi
    Shora = datar.substring(primeracoma + 1, segundacoma);
    Shr = Shora.substring(0, Shora.indexOf(':'));
    Smin = Shora.substring(Shora.indexOf(':') + 1, Shora.indexOf(':') + 3);
    //lectura de fecha hmi
    Sfecha = datar.substring(segundacoma + 1, terceracoma);
    Sday = Sfecha.substring(Sfecha.indexOf('p') + 1, Sfecha.indexOf('/'));
    Smonth = Sfecha.substring(Sfecha.indexOf('/') + 1, Sfecha.indexOf('/') + 3);
    Syear = Sfecha.substring(Sfecha.indexOf('/') + 4, Sfecha.indexOf('/') + 8);
    DEBUG_PRINT("Fecha:");
    DEBUG_PRINT(Sday);
    DEBUG_PRINT("/");
    DEBUG_PRINT(Smonth);
    DEBUG_PRINT("/");
    DEBUG_PRINTLN(Syear);
    DEBUG_PRINT("hora:");
    DEBUG_PRINT(Shr);
    DEBUG_PRINT(":");
    DEBUG_PRINTLN(Smin);

    ano1 = Syear.toInt();
    mes1 = Smonth.toInt();
    dia1 = Sday.toInt();
    hora1 = Shr.toInt();
    minuto1 = Smin.toInt();
    DEBUG_PRINT("Fecha:");
    DEBUG_PRINT(dia1);
    DEBUG_PRINT("/");
    DEBUG_PRINT(mes1);
    DEBUG_PRINT("/");
    DEBUG_PRINTLN(ano1);
    DEBUG_PRINT("hora:");
    DEBUG_PRINT(hora1);
    DEBUG_PRINT(":");
    DEBUG_PRINTLN(minuto1);
    sprintf(buf2, "%02d:%02d", hora1, minuto1);
    sprintf(buf3, "%02d/%02d/%04d", dia1, mes1, ano1);

    

    DateTime dt(ano1, mes1, dia1, hora1, minuto1, segundo1); // seteo el tiempo en 12:00:00-23:11:2017 [HH:MM:SS-dia/mes/año]
    rtc.adjust(dt);                                           //Adjust date-time as defined 'dt' above
    rtcSoft.begin(dt);
    cambioFechaHora=true;
    
  }
  else if (datar.startsWith("set1") && (datar.length() > 5)) // if it is a valid command (starts with set1)
  {                                                          // Comma separated data is delimited and organized
    int primeracoma = datar.indexOf(',');
    int segundacoma = datar.indexOf(',', primeracoma + 1);
    int terceracoma = datar.indexOf(',', segundacoma + 1);
    int cuartacoma = datar.indexOf(',', terceracoma + 1);
    //int quintacoma = datar.indexOf(',', cuartacoma + 1);
    // Creation of auxiliary variables
    int pageid_aux = (datar.substring(primeracoma + 1, segundacoma)).toInt();
    int page_aux = (datar.substring(segundacoma + 1, terceracoma)).toInt();
    int modo_aux = (datar.substring(terceracoma + 1, cuartacoma)).toInt();
    char prox[30];
    //int nowc_aux = (datar.substring(cuartacoma + 1, quintacoma)).toInt();
    if ((modo_automatico!=modo_aux))
    {
      modo_automatico=modo_aux;
      if(modo_automatico==1)
      {
        
      }
      solicitud_web = 1;
    }

    Serial.println();
    Serial.print(F("pageid:"));
    Serial.print(pageid_aux);
    Serial.print(F(" page:"));
    Serial.println(page_aux);
    
    if (pageid_aux == 1) ///home
    { 
      
      
      if(modo_automatico==1)
      { 
        sprintf(prox, "%02d:%02d %s", horario[6*(proximo_ciclo-1)], horario[6*(proximo_ciclo-1)+1],Dia_semana[dia_prox_ciclo]);
        

      }
      modo_nowc=1;
      
    }
    else if (pageid_aux == 2) //configuración
    {
      
      modo_nowc=0;
    }
    //  else if (pageid_aux==3)//manual
    // {
    //   /* code */
    // }
    else if (pageid_aux == 4) //Auto
    {
      if (page_aux == 11)
      {

        if (numero_horarios > 4)
        {
          String bx = "b" + String(numero_horarios - 4) + ".pic";
          String click = "click b5,1";
          String release = "click b5,0";
         
        }
        else if (numero_horarios > 0)
        {
          String bx = "b" + String(numero_horarios) + ".pic";
         
          /* code */
        }
      }
      else{
        if ((numero_horarios > 4)&&(page_aux == 2))
        {
          String bx = "b" + String(numero_horarios - 4) + ".pic";
          
          
        }
        else if ((numero_horarios > 0)&&(numero_horarios < 5)&&(page_aux != 2))
        {
          String bx = "b" + String(numero_horarios) + ".pic";
          
          /* code */
        }
      }

    }
    else if (pageid_aux == 5) //Auto1 hora inicial
    {  min_final ="";
       hora_final ="";
      if (horario[(6 * (page_aux - 1) + 1)] > 9)
      {
        min_inicial = String(horario[(6 * (page_aux - 1) + 1)]);
      }
      else if (horario[(6 * (page_aux - 1) + 1)] < 10)
      {
        min_inicial = "0" + String(horario[(6 * (page_aux - 1) + 1)]);
      }
      if (horario[(6 * (page_aux - 1))] > 9)
      {
        hora_inicial = String(horario[(6 * (page_aux - 1))]);
      }
      else if(horario[(6 * (page_aux - 1))] < 10)
      {
        hora_inicial = "0" + String(horario[(6 * (page_aux - 1))]);
      }

      

      /* code */
    }
    else if (pageid_aux == 6) //Auto2 hora final
    {  min_final ="";
       hora_final ="";
      if (horario[(6 * (page_aux - 1) + 3)] > 9)
      {
        min_final = String(horario[(6 * (page_aux - 1) + 3)]);
      }
      else if (horario[(6 * (page_aux - 1) + 3)] <10)
      {
        min_final = "0" + String(horario[(6 * (page_aux - 1) + 3)]);
      }
      if (horario[(6 * (page_aux - 1)+2)] > 9)
      {
        hora_final = String(horario[(6 * (page_aux - 1)+2)]);
      }
      else if (horario[(6 * (page_aux - 1)+2)] < 10)
      {
        hora_final = "0" + String(horario[(6 * (page_aux - 1)+2)]);
      }
      
      /* code */
    }
    else if (pageid_aux == 7) //Auto3 dias semana
    {
      for (int diabit = 0; diabit < 7; diabit++)
      { //chequea cadabit del arreglo dias_semana para cada horario

        if (bitRead(horario[(6 * (page_aux - 1)) + 4], diabit) == 1) //si algun dia está activo
        {
          String bx = "b" + String(diabit + 1) + ".pic";
          
        }
      }
    }
    else if (pageid_aux == 8) //Auto4  valvulas por horario
    {
      for (int i = 0; i < max_num_valves; i++) ///verifica cada valvula
      {

        if (bitRead(horario[((page_aux - 1) * 6) + 5], i) == 1) //si la valvula  esta programada para ese horario
        {
          String bx = "b" + String(i + 1) + ".pic";
          
        }
      }
    }
    // else if (pageid_aux==9)//Fecha y hora
    // {
    //   /* code */
    // }
    // else if (pageid_aux==10)//Fecha y hora 1
    // {
    //   /* code */
    // }
    // else if (pageid_aux==11)//WiFi
    // {
    //   /* code */
    // }
    // else if (pageid_aux==12)//WiFi1
    // {
    //   /* code */
    // }
    // else if (pageid_aux==13)//WiFi2
    // {
    //   /* code */
    // }
  
  }
}
