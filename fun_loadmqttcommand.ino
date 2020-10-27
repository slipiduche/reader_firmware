void loadMqttCommand(String datar)
{
    if (modo_nowc == 0)
    {

        DEBUG_PRINTLN("Comando recibido--Ignorado por control HMI");
    }
    else if (datar.startsWith("{\"enviar_datos_completos\":1}") && (datar.length() > 5)) //si es un comando valido (envia datos a web)
    {
        solicitud_web = 1;
    }
    else if (datar.startsWith("{\"modo_activo\":0}"))
    {
        modo_automatico = 0;
        solicitud_web = 1;
    }
    else if (datar.startsWith("{\"modo_activo\":1}"))
    {
        modo_automatico = 1;
        solicitud_web = 1;
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
    else if (datar.startsWith("{\"FechaYHora\":["))
    {
        solicitud_web = 1;
        int primeracoma = datar.indexOf('[');
        int segundacoma = datar.indexOf(',', primeracoma + 1);
        int primeracoma_aux = datar.indexOf('[');
        int segundacoma_aux = datar.indexOf(',');

        
        //DEBUG_PRINT("estado_valvulas="); //{\"estado_valvulas\":[0,0,1,1]}
        DEBUG_PRINT("{\"FechaYHora\":[");
        
            String horaMqtt = (datar.substring(primeracoma_aux + 1, segundacoma_aux));
            String Shr = horaMqtt.substring(0, horaMqtt.indexOf(':'));
            String Smin = horaMqtt.substring(horaMqtt.indexOf(':') + 1, horaMqtt.indexOf(':') + 3);
            String fechaMqtt= (datar.substring(segundacoma_aux+1, datar.indexOf(' ')));
            Syear = fechaMqtt.substring(0, fechaMqtt.indexOf('-'));
            Smonth = fechaMqtt.substring(fechaMqtt.indexOf('-') + 1, fechaMqtt.indexOf('-') + 3);
            Sday = fechaMqtt.substring(fechaMqtt.indexOf('-') + 4, fechaMqtt.indexOf('-') + 8);
            
        DEBUG_PRINT(horaMqtt);
        DEBUG_PRINT(",");
        DEBUG_PRINT(fechaMqtt);

            
        

        DEBUG_PRINTLN("]}");
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
        
        DEBUG_PRINTLN("LOAD MQTT Fecha Y Hora SUCCESS");
    }
}