///Todas las configuraciones iniciales deben ser colocadas en esta función

void setup()
{ // formateando la memoria eeprom si es la primera vez o cargado los datos guardados
  
  if (!EEPROM.begin(512))
  {
    Serial.println("failed to initialise EEPROM please reset"); return;
  }
  
   int boottime=EEPROM.read(1);
  
  Serial.begin(115200);     // initialize serial for debugging
  for (int i = 1; i < max_num_valves + 1; i++)
  {
    pinMode(pinout_valvulas[i], OUTPUT);
    digitalWrite(pinout_valvulas[i], OFF); // inicializo los puertos a 1 porque 0 se activan
    valvulas[i] = OFF_bit;
  }
  pinMode(wifiled, OUTPUT);
  wifiLedBlink();

  // char bufferRtc[20];
  
  rtcSoft.begin(now);
  // now.toString(bufferRtc);
  // DEBUG_PRINTLN(bufferRtc);
  now = rtcSoft.now(); //get the current date-time
  Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
  // char bufferRtc1[20];
  
  // now.toString(bufferRtc1);
  // DEBUG_PRINTLN(bufferRtc1);
  DEBUG_PRINTLN("iniciando2");
  
  
  
                            ///leer archivos de configuracion del equipo
  chipid = get_chipidstr(); //obitene el chipid
  clientId += String(chipid);
  fun_spiff_setup();        //Funcion ubicada en fun_spiff
  if(boottime==bootX)
  {
  read_spiffconfig1();      //Funcion ubicada en fun_spiff
  //read_spiffconfig2();      //Funcion ubicada en fun_spiff
  read_spiffmodo();      //Funcion ubicada en fun_spiff
  read_spiffhorarios();      //Funcion ubicada en fun_spiff
  }
  else{
    
    save_horarios_spiff();
    save_modo_spiff();
    save_config1_spiff();
    //save_config2_spiff();
    EEPROM.write(1,bootX); //(pos,dato)
    EEPROM.commit();

  }
 
  web_setup();              //apmode
  nfcSetup();//nfc setup
  setup_dualcore();         ///configuración de elementos de rtos
  
  
    DEBUG_PRINT("inicio0:");
    DEBUG_PRINTLN(inicio);
    
    solicitud_web=1;
  
}

TaskHandle_t Task2, Task3;
//int cuenta = 0;
///configuración de elementos de rtos
void setup_dualcore()
{

  xTaskCreatePinnedToCore(
      WebComm,
      "Task_2",
      20000,
      NULL,
      1,
      &Task2,
      0);

  xTaskCreatePinnedToCore(
      APmode,
      "Task_3",
      20000,
      NULL,
      1,
      &Task3,
      0);

  //Serial.begin(115200);
}

String get_chipidstr()
{ ////////extracts the chipid from the esp32 in a character string
  String chip;

  Serial.print("chipId: ");
  ChipId = ESP.getEfuseMac();
  ChipId16 = ((uint16_t)(ChipId >> 32));
  ChipId32 = ((uint32_t)(ChipId));

  chip = String(ChipId16, HEX) + String(ChipId32, HEX);
  Serial.println(chip);
  return chip;
}
