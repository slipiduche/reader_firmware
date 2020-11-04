void setup()
{
  if (!EEPROM.begin(512))
  {
    Serial.println("failed to initialise EEPROM please reset");
    return;
  }

  int boottime = EEPROM.read(1);

  Serial.begin(115200); // initialize serial for debugging

  pinMode(wifiled, OUTPUT);
  wifiLedBlink();
  DEBUG_PRINTLN("iniciando2");

  chipid = get_chipidstr();
  clientId += String(chipid);
  fun_spiff_setup(); //alocated in fun_spiff
  if (boottime == bootX)
  {
    read_spiffconfig1(); //alocated in fun_spiff

    //read_spiffmodo(); //alocated in fun_spiff
  }
  else
  {
    //save_modo_spiff();
    save_config1_spiff();
    EEPROM.write(1, bootX); //(pos,data)
    EEPROM.commit();
  }
  web_setup(); //apmode
  nfcSetup();  //nfc setup
  setup_dualcore();
  DEBUG_PRINT("inicio0:");
  DEBUG_PRINTLN(inicio);
  solicitud_web = 1;
}

TaskHandle_t Task2, Task3;

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
