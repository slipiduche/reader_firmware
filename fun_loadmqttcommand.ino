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
    else if (datar.startsWith("{\"estado_valvula\":["))
    {
        solicitud_web = 1;
        int primeracoma = datar.indexOf('[');
        int segundacoma = datar.indexOf(',', primeracoma + 1);
        int primeracoma_aux = datar.indexOf('[');
        int segundacoma_aux = datar.indexOf(',');

        String estado_valvula_aux = datar.substring(datar.indexOf('{'));
        //DEBUG_PRINT("estado_valvulas="); //{\"estado_valvulas\":[0,0,1,1]}
        DEBUG_PRINT("{\"estado_valvula\":[");
        // for (int i = 1; i < (max_num_valves+1); i++)
        // {   int estado_valvula = (datar.substring(primeracoma_aux + 1, segundacoma_aux)).toInt();
        //     valvulas[i]=!(bitRead(estado_valvula, 0));
        //     primeracoma_aux = segundacoma_aux;
        //     segundacoma_aux = datar.indexOf(',', primeracoma_aux + 1);
        //     DEBUG_PRINT(String(!valvulas[i]));
        //     DEBUG_PRINT(",");
        // }
        int estado_valvula = (datar.substring(segundacoma_aux + 1, datar.indexOf(']'))).toInt();
        int valvula = (datar.substring(primeracoma_aux + 1, segundacoma_aux)).toInt();
        valvulas[valvula] = (bitRead(estado_valvula, 0));
        DEBUG_PRINT(valvula);
        DEBUG_PRINT(",");
        DEBUG_PRINT(estado_valvula);
        DEBUG_PRINTLN("]}");
        
        DEBUG_PRINT("activarla:");
        DEBUG_PRINTLN(bitRead(estado_valvula, 0));
        DEBUG_PRINTLN("LOAD MQTT Estado_valvulas SUCCESS");
    }
    else if (datar.startsWith("{\"estados_valvulas\":["))
    {
        solicitud_web = 1;
        int primeracoma = datar.indexOf('[');
        int segundacoma = datar.indexOf(',', primeracoma + 1);
        int primeracoma_aux = datar.indexOf('[');
        int segundacoma_aux = datar.indexOf(',');

        String estado_valvula_aux = datar.substring(datar.indexOf('{'));
        //DEBUG_PRINT("estado_valvulas="); //{\"estado_valvulas\":[0,0,1,1]}
        DEBUG_PRINT("{\"estados_valvulas\":[");
        for (int i = 1; i < (max_num_valves + 1); i++)
        {
            int estado_valvula = (datar.substring(primeracoma_aux + 1, segundacoma_aux)).toInt();
            valvulas[i] = (bitRead(estado_valvula, 0));
            primeracoma_aux = segundacoma_aux;
            segundacoma_aux = datar.indexOf(',', primeracoma_aux + 1);

            DEBUG_PRINT(String(!valvulas[i]));
            if (i != max_num_valves)
            {
                DEBUG_PRINT(",");
            }
        }

        DEBUG_PRINTLN("]}");
        
        
        DEBUG_PRINTLN("LOAD MQTT Estado_valvulas SUCCESS");
    }
    
}
