void loadMqttCommand(String datar)
{
    if (modo_nowc == 0)
    {

        DEBUG_PRINTLN("Comando recibido--Ignorado por control HMI");
    }
    else if (datar.startsWith("horarios") && (datar.length() > 5)) //si es un comando valido (comienza por horarios)
    {
        solicitud_web = 1;
        //Se delimitan y organizan los datos separados por comas
        int primeracoma = datar.indexOf(',');
        int segundacoma = datar.indexOf(',', primeracoma + 1);
        int primeracoma_aux = segundacoma;
        int segundacoma_aux = datar.indexOf(',', primeracoma_aux + 1);
        //Creación de variables auxiliares
        String numero_horario_aux = datar.substring(primeracoma + 1, segundacoma);
        modo_automatico = 1;
        numero_horarios = numero_horario_aux.toInt();
        if (numero_horarios > 0)
        {
            DEBUG_PRINTLN();
            DEBUG_PRINT("horarios,");
            DEBUG_PRINT(numero_horarios);
            DEBUG_PRINT(",");
            for (int i = 0; i < (numero_horarios * 6); i++)
            {
                //horario_aux='';

                String horario_aux = datar.substring(primeracoma_aux + 1, segundacoma_aux);
                horario[i] = horario_aux.toInt();
                primeracoma_aux = segundacoma_aux;
                segundacoma_aux = datar.indexOf(',', primeracoma_aux + 1);

                DEBUG_PRINT(String(horario[i]));
                DEBUG_PRINT(",");
            }
            guardarHorarios = 1;
            DEBUG_PRINTLN();
        }

        DEBUG_PRINTLN("LOAD MQTT Horarios SUCCESS");
    }
    
    
    
}
