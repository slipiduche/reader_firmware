/***Puerto de Monitoreo***/
#define DEBUG
#ifdef DEBUG
#define DEBUG_PORT Serial //al usar la clase Serial se esta usando el puerto uart 0 ubicado por defecto en GPIO1/RX y GPIO3/TX del esp32

#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_WRITE(x) Serial.write(x)
#define DEBUG_PRINTx(x, y) Serial.print(x, y)
#define DEBUG_PRINTDEC(x) Serial.print(x, DEC)
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINTLNx(x, y) Serial.println(x, y)
#else
#define DEBUG_PORT
#define DEBUG_WRITE(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTx(x, y)
#define DEBUG_PRINTDEC(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTLNx(x, y)
#endif

////////////////
#include <ArduinoJson.h>
#include <Wire.h> // libreria para usar i2c para el RTCC y la LCD

#include "EEPROM.h"       // libreria para guardar y leer en la eeprom del esp32
// #include <Sodaq_DS3231.h> // libreria para el RTC
#include <RTClib.h>

RTC_DS3231 rtc;
RTC_Millis rtcSoft;
#include <SPI.h>
#include "FS.h"
#include "SD.h"
#include "SPIFFS.h"
#define ssl
#ifdef ssl

#include <WiFiClientSecure.h>
//#include <credentials.h>
WiFiClientSecure client1;
//WiFiClientSecure client_1;
#else
#include <WiFi.h>
WiFiClient client1;
//WiFiClient client_1;
#endif
#include <ESP32WebServer.h>
#define AP
ESP32WebServer server(80); //utilizado en la implementacion de un servidor local
/**almacenamiento**/
#define fun_spiff
/****DEFINICION DE ALMACENAMIENTO - CONTROL****/
File logFile; //utilizado para el fujo de datos con Spiff

/*
  const char* root_ca = \
                      "-----BEGIN CERTIFICATE-----\n" \
                      "MIIFxTCCBK2gAwIBAgIQQRtJ4xBr6EiECpiBU9M+VTANBgkqhkiG9w0BAQsFADCB\n" \
                      "jzELMAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4G\n" \
                      "A1UEBxMHU2FsZm9yZDEYMBYGA1UEChMPU2VjdGlnbyBMaW1pdGVkMTcwNQYDVQQD\n" \
                      "Ey5TZWN0aWdvIFJTQSBEb21haW4gVmFsaWRhdGlvbiBTZWN1cmUgU2VydmVyIENB\n" \
                      "MB4XDTIwMDYyNjAwMDAwMFoXDTIxMDYyNjIzNTk1OVowGzEZMBcGA1UEAxMQc2Ry\n" \
                      "Lm9yYml0dGFzLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAM7q\n" \
                      "7lHBm9Rm5xqUtNr0dbim7BR9oayfnuKt8PpEC97Id45m6883v9EYbp7Qi9/QmeIU\n" \
                      "Dg352bi0Qmn4ekxb/PwB+t14xerAaaZiIeOtsd5O+wq2aPigX1gSd5ngv/q+KmN2\n" \
                      "14FXOak48IfrpOolLWGR10w++OHDpI6/gt0l2uuBni1EejxjZunTwgdsgM4O6FxE\n" \
                      "GST0qX9pujv+9InzVI32FJgizj/YQ8CF7M83dfnUKUX94aor+Bfr0Y33fWUqYeCO\n" \
                      "d6LQw7JNG5MNATlG/bHT1s5TNAnH6uB3K6sh9smW1JQXZ9eRjllTKIX+AFS4sjV6\n" \
                      "yIFWZCkkkFBY8D6wEwkCAwEAAaOCAo4wggKKMB8GA1UdIwQYMBaAFI2MXsRUrYrh\n" \
                      "d+mb+ZsF4bgBjWHhMB0GA1UdDgQWBBQTJvzP4LqMz07i+5/j2ZtUrJ3RxzAOBgNV\n" \
                      "HQ8BAf8EBAMCBaAwDAYDVR0TAQH/BAIwADAdBgNVHSUEFjAUBggrBgEFBQcDAQYI\n" \
                      "KwYBBQUHAwIwSQYDVR0gBEIwQDA0BgsrBgEEAbIxAQICBzAlMCMGCCsGAQUFBwIB\n" \
                      "FhdodHRwczovL3NlY3RpZ28uY29tL0NQUzAIBgZngQwBAgEwgYQGCCsGAQUFBwEB\n" \
                      "BHgwdjBPBggrBgEFBQcwAoZDaHR0cDovL2NydC5zZWN0aWdvLmNvbS9TZWN0aWdv\n" \
                      "UlNBRG9tYWluVmFsaWRhdGlvblNlY3VyZVNlcnZlckNBLmNydDAjBggrBgEFBQcw\n" \
                      "AYYXaHR0cDovL29jc3Auc2VjdGlnby5jb20wMQYDVR0RBCowKIIQc2RyLm9yYml0\n" \
                      "dGFzLmNvbYIUd3d3LnNkci5vcmJpdHRhcy5jb20wggEEBgorBgEEAdZ5AgQCBIH1\n" \
                      "BIHyAPAAdwB9PvL4j/+IVWgkwsDKnlKJeSvFDngJfy5ql2iZfiLw1wAAAXLxRciW\n" \
                      "AAAEAwBIMEYCIQCb6H/nTV5ib61CyL/XewhSBQYW/VbcT4fBShFgKmzebwIhAIE9\n" \
                      "ppm5ooQLnhgyVmoxHfG5RoVLbbzwU9SbX+PfdCMTAHUAlCC8Ho7VjWyIcx+CiyIs\n" \
                      "DdHaTV5sT5Q9YdtOL1hNosIAAAFy8UXIvwAABAMARjBEAiAbkkFXsNDTdw/Jzehm\n" \
                      "zVK7Um+h0DJUu4rH7+rpbls8vAIgJWzjWAtYXGSJXU5Qq/j5X6C/yp0Sz+R9hNa4\n" \
                      "9wQXiewwDQYJKoZIhvcNAQELBQADggEBAJ1lHnhL+ODd1JQF8luEcTryn5YL4W40\n" \
                      "ym5Bfh2Sy3QeuRF47GyE9xY3lgwLgzt/yUIXYc5cKTLfRJ7bd92Bh1fFvLK7CIvn\n" \
                      "yXi5qGPpPxjaGFsXQ2WnNRs9aCXVTYAYCIOWO5YG08PbkepY0KHQL8Ne4hIAmIbg\n" \
                      "l2J3UWyMOcvYjatLMDuoyq3W1a5YlZjSQuWuQ6r7t2AXB4hi8VXtJQQwSpIIqeMG\n" \
                      "b5ep0+QV6s70H4NKthW8G3RBDJzdR8XW34nzcFx/2rAtNbpvGnsA0PxEhVlN768I\n" \
                      "si+ok66/RnV1nqOhh17oe5O4DGupbyznjqms+B9683iEQC42Zq+HeVk=\n" \
                      "-----END CERTIFICATE-----\n";
  const char* test_client_cert = \
                               "-----BEGIN CERTIFICATE-----\n" \
                               "MIIFxTCCBK2gAwIBAgIQQRtJ4xBr6EiECpiBU9M+VTANBgkqhkiG9w0BAQsFADCB\n" \
                               "jzELMAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4G\n" \
                               "A1UEBxMHU2FsZm9yZDEYMBYGA1UEChMPU2VjdGlnbyBMaW1pdGVkMTcwNQYDVQQD\n" \
                               "Ey5TZWN0aWdvIFJTQSBEb21haW4gVmFsaWRhdGlvbiBTZWN1cmUgU2VydmVyIENB\n" \
                               "MB4XDTIwMDYyNjAwMDAwMFoXDTIxMDYyNjIzNTk1OVowGzEZMBcGA1UEAxMQc2Ry\n" \
                               "Lm9yYml0dGFzLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAM7q\n" \
                               "7lHBm9Rm5xqUtNr0dbim7BR9oayfnuKt8PpEC97Id45m6883v9EYbp7Qi9/QmeIU\n" \
                               "Dg352bi0Qmn4ekxb/PwB+t14xerAaaZiIeOtsd5O+wq2aPigX1gSd5ngv/q+KmN2\n" \
                               "14FXOak48IfrpOolLWGR10w++OHDpI6/gt0l2uuBni1EejxjZunTwgdsgM4O6FxE\n" \
                               "GST0qX9pujv+9InzVI32FJgizj/YQ8CF7M83dfnUKUX94aor+Bfr0Y33fWUqYeCO\n" \
                               "d6LQw7JNG5MNATlG/bHT1s5TNAnH6uB3K6sh9smW1JQXZ9eRjllTKIX+AFS4sjV6\n" \
                               "yIFWZCkkkFBY8D6wEwkCAwEAAaOCAo4wggKKMB8GA1UdIwQYMBaAFI2MXsRUrYrh\n" \
                               "d+mb+ZsF4bgBjWHhMB0GA1UdDgQWBBQTJvzP4LqMz07i+5/j2ZtUrJ3RxzAOBgNV\n" \
                               "HQ8BAf8EBAMCBaAwDAYDVR0TAQH/BAIwADAdBgNVHSUEFjAUBggrBgEFBQcDAQYI\n" \
                               "KwYBBQUHAwIwSQYDVR0gBEIwQDA0BgsrBgEEAbIxAQICBzAlMCMGCCsGAQUFBwIB\n" \
                               "FhdodHRwczovL3NlY3RpZ28uY29tL0NQUzAIBgZngQwBAgEwgYQGCCsGAQUFBwEB\n" \
                               "BHgwdjBPBggrBgEFBQcwAoZDaHR0cDovL2NydC5zZWN0aWdvLmNvbS9TZWN0aWdv\n" \
                               "UlNBRG9tYWluVmFsaWRhdGlvblNlY3VyZVNlcnZlckNBLmNydDAjBggrBgEFBQcw\n" \
                               "AYYXaHR0cDovL29jc3Auc2VjdGlnby5jb20wMQYDVR0RBCowKIIQc2RyLm9yYml0\n" \
                               "dGFzLmNvbYIUd3d3LnNkci5vcmJpdHRhcy5jb20wggEEBgorBgEEAdZ5AgQCBIH1\n" \
                               "BIHyAPAAdwB9PvL4j/+IVWgkwsDKnlKJeSvFDngJfy5ql2iZfiLw1wAAAXLxRciW\n" \
                               "AAAEAwBIMEYCIQCb6H/nTV5ib61CyL/XewhSBQYW/VbcT4fBShFgKmzebwIhAIE9\n" \
                               "ppm5ooQLnhgyVmoxHfG5RoVLbbzwU9SbX+PfdCMTAHUAlCC8Ho7VjWyIcx+CiyIs\n" \
                               "DdHaTV5sT5Q9YdtOL1hNosIAAAFy8UXIvwAABAMARjBEAiAbkkFXsNDTdw/Jzehm\n" \
                               "zVK7Um+h0DJUu4rH7+rpbls8vAIgJWzjWAtYXGSJXU5Qq/j5X6C/yp0Sz+R9hNa4\n" \
                               "9wQXiewwDQYJKoZIhvcNAQELBQADggEBAJ1lHnhL+ODd1JQF8luEcTryn5YL4W40\n" \
                               "ym5Bfh2Sy3QeuRF47GyE9xY3lgwLgzt/yUIXYc5cKTLfRJ7bd92Bh1fFvLK7CIvn\n" \
                               "yXi5qGPpPxjaGFsXQ2WnNRs9aCXVTYAYCIOWO5YG08PbkepY0KHQL8Ne4hIAmIbg\n" \
                               "l2J3UWyMOcvYjatLMDuoyq3W1a5YlZjSQuWuQ6r7t2AXB4hi8VXtJQQwSpIIqeMG\n" \
                               "b5ep0+QV6s70H4NKthW8G3RBDJzdR8XW34nzcFx/2rAtNbpvGnsA0PxEhVlN768I\n" \
                               "si+ok66/RnV1nqOhh17oe5O4DGupbyznjqms+B9683iEQC42Zq+HeVk=\n" \
                               "-----END CERTIFICATE-----\n" ;
*/
/*
  const char* test_client_key = \
                              "-----BEGIN RSA PRIVATE KEY-----\n" \
                              "MIIEpAIBAAKCAQEAzuruUcGb1GbnGpS02vR1uKbsFH2hrJ+e4q3w+kQL3sh3jmbr\n" \
                              "zze/0RhuntCL39CZ4hQODfnZuLRCafh6TFv8/AH63XjF6sBppmIh462x3k77CrZo\n" \
                              "+KBfWBJ3meC/+r4qY3bXgVc5qTjwh+uk6iUtYZHXTD744cOkjr+C3SXa64GeLUR6\n" \
                              "PGNm6dPCB2yAzg7oXEQZJPSpf2m6O/70ifNUjfYUmCLOP9hDwIXszzd1+dQpRf3h\n" \
                              "qiv4F+vRjfd9ZSph4I53otDDsk0bkw0BOUb9sdPWzlM0Ccfq4HcrqyH2yZbUlBdn\n" \
                              "15GOWVMohf4AVLiyNXrIgVZkKSSQUFjwPrATCQIDAQABAoIBABkD1+n0YRVtl1Tk\n" \
                              "U3UBBtZa/8F+arRG29uFsdNftJurWqDRmoKDt+HcdUsn5TEas+/cAtL3uLWWvUy4\n" \
                              "aHigylXhG2X4+txlhKBDztsKna1FmR3zbX9ZTA2GtgTZYfw7Tz6VGHqfg+4MXy/f\n" \
                              "ciGIexMHoaKrHkWU9Gj5iCWaNPwxiIJ/CQ2ttMRQy9Atl3SMZaUA9v4huMRa79CR\n" \
                              "DnQ6OPo5e+u3bf49sTircilx0K5QUyYzD+qTtz8rz5W7TYyhXAhn2GZ8uOlJ7fQp\n" \
                              "gG4PlLH1ENMDnxL6BhxpfVkh9Pg2CiYHYZcg//wzxXb7dJ6AQPb3pGEIIzMY8xlY\n" \
                              "/E6RVDECgYEA+4h5XVWxGfc6Pyfuhcc+Mp4h3oHTnpPBsvW8uN3wvB2J3RynpbnW\n" \
                              "QzIUlgYHcwf3ebwc6lSq/pOcmDD+j72L7V70vwz+6VxUeXxCm9br7JPdazYfsqBp\n" \
                              "V8RESmf5eSrgSK4qYqsSe+nyK66d6wJ7DuNuOUwOdFNt9ZFoiI3uOi0CgYEA0peg\n" \
                              "DpB3WdbLRBf1afIb06ZVR9EmLvHeoOWe5n2vxpjcdd5PILcAXya8NYJUjRo34nng\n" \
                              "xL8j7LmvdD+jOwJqcf5bboSHhlOuqluQ5UkGCD8WzY8TOVa5Cue9FPCX/h2IjmqL\n" \
                              "nWgflt8Ns1/DOe5W25lfQ6zxuUEDZr90DRrYkc0CgYEAzhBeLQKY1kbd5wOVuu8h\n" \
                              "m2JFKP5/Gbi+xRfghir8n4Arg4OILJL0X5RVnRpPXAO40Hg10BuRAoJ3lY793VBR\n" \
                              "pCNYqnPBGfjUE7qi7vzBxszUZpKuoS4dXxOhRrure4KOv7w/aRnxSKa+CN9C7FbZ\n" \
                              "7baQhEw0w0PeH4ICe2ybL/kCgYBIy3zaS/nVpjfxO87rZnNUAAUupA9s3tK0D2uU\n" \
                              "BRJDtgdO/x+fEWMhgQRCAuEzPULgGH0nxWKnq7AIoQ7NkRI2gVQdqdYnCrovzSMS\n" \
                              "/uEE7Ln3KGPeyCmCqYRkvp3ri03u3MmSCbIRttzHimyzk72Wfze87wVHoj+Ivtlg\n" \
                              "xkziaQKBgQD1omEQhDZxVt1GZfofYr7b7AYKR/M96e7u9zR8CZvjQZd3hLY5W6Pz\n" \
                              "kMWgE3KZYpMy8XdmK6q6BxJpdxBeVtBh9ZVrLagsPknDaZyuAlEiQtJ78GCaKUFv\n" \
                              "Ej8NHi6Ws7DSCpCTgRzP18RQBvj5EuG5W+VlDeRcVk9j6PdjBrGV8Q==\n" \
                              "-----END RSA PRIVATE KEY-----\n" ;
*/

/**HMI SERIAL**/
HardwareSerial HMI(2); //Se define el puerto hardware uart 2 del esp32 para uso de HMI nextion

/**** REGISTROS ****/

#define salidabajoactiva // Determina si las salidas seran bajo activas o alto activas
//#define salidaaltoactiva
#define max_time_duracion 60 // maxima duracion de los ciclos de encendido de las valvulas
#define max_horarios 4       // maxima cantidad de horarios a selecteccionar (OJO tambien hay q añadir codigo al modificar esto)
#define max_num_valves 4     // cantidad maxima de valvulas
#define max_numero_zonas 4   // numero maxima de zonas de riego

#define bomba_de_agua 32 // pinout para posible bomba, se activa si por lo menos una valvula esta activa
/* Defincion del pinout de las 16 valvulas*/
#define val_1 2
#define val_2 4
#define val_3 12
#define val_4 13
#define val_5 14
#define val_6 15
#define val_7 25
#define val_8 26
#define val_9 27
//#define val_10 32
#define wifiled 5 //5 para placa de pruebas lolin32  32 para placa final

/* Definicion del pinout de los pulsadores select, arriba, abajo, izquierda y derecha*/
#define izq 34 // pinout pulsador izquierda
#define der 35 // pinout pulsador derecha
#define aba 36 // pinout pulsador abajo
#define arr 39 // pinout pulsador arriba
#define sel 33 // pinout pulsador seleccionar

#define LCD_backlight 32 // LCD blpin

#define filasLCD 4     // Numero de filas del LCD
#define columnasLCD 20 // Numero de columnas del LCD

#define RetardoAntirebotePulsador 25 // antirebote de los pulsadores por software
#define tiempo_auto_off_LCD 20000    // timepo para que la LCD se apage automaticament y salga al menu inicial

#ifdef salidabajoactiva // si se define la salida bajo activa defino ciertas palabras
#define OFF_bit 1
#define ON_bit 0
#define OFF HIGH
#define ON LOW
#endif
#ifdef salidaaltoactiva // si se define la salida alto activa defino ciertas palabras
#define OFF_bit 0
#define ON_bit 1
#define OFF LOW
#define ON HIGH
#endif

#define state_val "estado_val" // definicion usada para reducir el espacio de la memoria del programa

/****VARIABLES****/
/****Variables fun_web****/
bool noAP = 1;
int tnoAP = 0;
String webpage = "";     //contiene el string necesario para imprimir la webpage en el cliente
bool SD_present = false; //se usa para indicar la presencia de la sd en las funciones web SD
/*************/
unsigned long actualizar_hora = 0;

unsigned long t_ultima_accion = 0;
unsigned long cuentaSegundos = 0;

uint8_t count = 0;
uint8_t countRtc = 0;

bool connected = 0;
//----------------------------------------------//
//  Define todo lo relacionado con los menus    //
// las string que se utilizan en los menus      //
//----------------------------------------------//
boolean Salir_del_menu = false;
boolean update = true;  //variable utilizada para controlar la actuaalizacion desde pagina y hmi basica
boolean update2 = true; //variable utilizada para controlar la actuaalizacion desde pagina y hmi avanzada
byte menuPosition = 0;

#define TYPE_SMENU1 1
#define TYPE_SMENU2 2
#define TYPE_BOOL 3
#define TYPE_INT 4


//----------------------------------------------//
// Define todas las variables de los pulsadores //
// define tiempos de rebote y estado del pul    //
//----------------------------------------------//
byte pulsadores;
int estadopulsadores;       // bit(0)=sel_estado bit(1)=izq_estado  bit(2)=der_estado  bit(3)=arr_estado  bit(4)=aba_estado
int ultimoestadopulsadores; // bit(0)=sel_ultimo bit(1)=izq_ultimo  bit(2)=der_ultimo  bit(3)=arr_ultimo  bit(4)=aba_ultimo





//----------------------------------------------//
//   Define las variables de tiempos y horarios //
//----------------------------------------------//
char Dia_semana[][10] = {"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"}; /*Su=1 Mo=2 Tu=3 We=4 Th=5 Fr=6 Sa=7 */
int dia_semana_int = 0;
uint8_t segundo; /* seconds */
uint8_t minuto;  /* minutes */
uint8_t hora;    /* hours */
uint8_t dia;     /* day of the week */
uint8_t mes;     /* month */
int16_t ano;     /* year */

uint8_t prevmes;     /* month */
int16_t prevano;     /* year */
/*Auxiliares*/
uint8_t segundo1; /* seconds */
uint8_t minuto1;  /* minutes */
uint8_t hora1;    /* hours */
uint8_t dia1;     /* day of the week */
uint8_t mes1;     /* month */
int16_t ano1;     /* year */






uint8_t horario[48] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // hora_ini, minuto_ini,hora_fin,minuto_fin,dias,valvulas

//----------------------------------------------//
//   Define los array usados en el sistema      //
//----------------------------------------------//
int pinout_valvulas[max_num_valves + 1] = {0, val_1, val_2, val_3, val_4};
boolean valvulas[max_num_valves + 1] = {0, OFF_bit, OFF_bit, OFF_bit, OFF_bit};
//uint8_t Act_valvulas[8 + 1] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; ///valor int que representa cuales valvulas seran encendidas (16 estados) el puntero representa para cual horario esel estado pasa a horario[6*j+4]
uint8_t sistema[100];
int ID_sistema = 0;
//----------------------------------------------//
//             VARIABLES DE USO GENERAL         //
//----------------------------------------------//

int prevmin = 0, prevday = 0;
int rtcrestart = 0; //
int hmidate = 0;    //controla el envio de la fecha a la hmi
int hmienvia = 0;   //controla cuando enviar los datos a la hmi
int hmichange = 0;  //controla si hubo cambios por hmi
unsigned long timeout1 = 0, timeout2 = 0;
bool hmiwork = 1;     //haciendo lectura de hmi
bool webwork = 0;     //haciendo lectura de web
bool valvulasoff = 0; //controla apagado de valvulas cuando se envia de hmi modo auto
bool envia = 0;
int change = 0;          //controla el envio de datos al servidor (solo si algo cambió)
int modo_automatico = 0; // variable que controla si el modo automatico esta encendido o apagado
int numero_horarios = 0;
bool sal = 0;
bool tipo_de_automatico = true; // si en modo_automatico automatico se activan todas las zonas o una por una
int ciclo_actual = 0;
int encender_por_automatico = 0;
int encender_por_automatico_aux = -1;
int algun_horario_activo = 0;
int siguiente_zona = 1; // debe comenzar en 1 xq la valvula 0 no exite
unsigned long retardo_x_activacion = 0;
unsigned long ultima_activacion_automatico = 0;
bool automatico_incompleto = false;
bool Esperando_finalizar_Horario = false;
int Sistema_apagado = 0; //
bool encender_bomba = false;
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
//char server[] = "ec2-35-156-183-231.eu-central-1.compute.amazonaws.com";    ///cityvisions-master/ name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
//IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):

//Host configuration  (HTTP request)
char host[120] = "broker.mqttdashboard.com";
String ch, ch1, ch2, ch3;
int chlength = 0;

String formdata, vacio = "";
/****Variables SIM900****/

char aux_str[100]; //se utiliza como auxiliar en la formacion de cadenas para enviar comandos AT al sim900

char __APN[120] = "";     //APN de la red celular
char __usrnm[60] = "";    //User Name de la red celular
char __password[60] = ""; //Password APN de la red celular
int intentosg = 0;        //utilizada para controlar el numero de intentos en acceder a la red celular
char sim800lreset = 12;   //utilizado para encendido y apagado el modulo sim900 con el pin GPIO12 del esp32
int start = 0;            //utilizada para no repetir configuraciones del modulo sim900
int sim900 = 0;           //utilizada para indicar si el mosulo sim900 esta activo o inactivo

///-------------Set wifi network parameters---------------------
/****Parametros WIFI****/
//Parametros para conexion con router externo
char ssid[60] = "orbittas";     //WIFI SSID
char password[60] = "20075194"; //WIFI PASSWORD
//Parametros para conexion con wless lan local en esp32
char ssid2[60] = "Reader";         //AP SSID
char password2[60] = "12345678"; //AP PASSWORD
String WRSSI;
/****Variables SPIFF****/
long int lastposition = 0;  //contador de la posicion indice utilizado para la lectura de archivos en SPIFF en especifico el archivo config1.txt
long int lastposition2 = 0; //contador de la posicion indice utilizada para la lectura de archivos en SPIFF en especifico el archivo config2.txt
long int lastposition3 = 0;  //contador de la posicion indice utilizado para la lectura de archivos en SPIFF en especifico el archivo config1.txt
long int lastposition4 = 0; //contador de la posicion indice utilizada para la lectura de archivos en SPIFF en especifico el archivo config2.txt

///funciones de tiempo y control output
long int horainicio[8] = {0, 0, 0, 0, 0, 0, 0, 0};
long int horafin[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int proximo_ciclo = 0;
int dia_prox_ciclo = 0;
int minuto_aux = 60;
long int horaux = 0;
long int horaux2 = 0;
//int dias_semana[4] = {0, 0, 0, 0}; /// va de 0 a 127 posibilidasdes por cada elemento pasa a horario[6*j+5]
DateTime now;
//////
#define USEWIFIMQTT ///usar funciiones de transmision mqtt
#include "PubSubClient.h"//<PubSubClient.h>
WiFiClient espClient; //utilizado para las opciones de cliente WIFI requiere wifi.h

PubSubClient mqttclient(espClient); //utilizado para las funciones de ciente MQTT
/****Variables WIFI MQTT****/
char datarecvd[512]; //utilizada para la recepción de comandos mqtt vía wifi
int reconnect = 0;   //utilizada para controlar los intentos de reconexion como cliente mqtt wifi
/****parametros mqtt ****/

char MQTTHost[120] = "broker.mqttdashboard.com";  //"broker.mqttdashboard.com"; //                        //"mqtt01.ejemplo.com" Nombre del Host del broker MQTT
char MQTTPort[6] = "1883";                     //"1883" Puerto Broker MQTT
char MQTTClientID[60] = "";                    //ClientID MQTT utilizado para identificarse como cliente en la subscripcion a un topico, tambien usado como nombre de dispositivo
char MQTTTopic[60] = "";                       //Topico Salida donde se envia la data al broker
char MQTTTopic2[60] = "";                      //Topico entrada donde se recibe data  comandos de configuración

char MQTTUsername[60] = "*";  //"NOMBRE" username MQTT dato proporcionado por el broker para la autenticacion de la conexion
char MQTTPassword[120] = "*"; //"Rne3pIQPYO" password MQTT dato proporcionado por el broker para la autenticacion de la conexion

uint32_t ChipId32, wait = 0, wait2 = 0, wait3 = 0;
uint16_t ChipId16;
uint64_t ChipId;
String chipid = "";
String clientId = "reader/";

///changes
bool val_aux[max_num_valves];///se usa para saber si hubo un cambio en valvulas
int modo_automatico_aux;
int modo_nowc = 1;
int modo_nowc_aux = 0;
///changes mqtt
bool val_aux1[max_num_valves];///se usa para saber si hubo un cambio en valvulas
int modo_automatico_aux1;
int modo_nowc1 = 1;
int modo_nowc_aux1 = 0;

char ssid_aux[60] = "";     //WIFI SSID
char password_aux[60] = ""; //WIFI PASSWORD
char ssid2_aux[60] = "";         //AP SSID
char password2_aux[60] = ""; //AP PASSWORD
char host_aux[120] = "";
char MQTTUsername_aux[60] = "";
char MQTTPassword_aux[120] = "";
char MQTTHost_aux[120] = ""; //"sdrorbittas.sytes.net";                          //"mqtt01.ejemplo.com" Nombre del Host del broker MQTT
char MQTTPort_aux[6] = "";

int changev = 0; //cambio en valvulas
int changev2 = 0; //cambio en valvulas
int inicio = 0;

int proximo_ciclo_aux = 0;
int proximo_ciclo_aux1 = 0;
long int mqttdelay = 0, blikDelay = 0;
int env_prox = 0; //enviar mqtt proximo encendido
bool solicitud_web = 0;
bool envia_horarios = 0;
bool guardarHorarios = 0;

int subscribed = 0; ///verifica si esta subscrito a topic

bool rtcFalla = 0;
bool wifiLedState = false;
bool apMode = 0;

bool guardarAp = 0;
String ipRed = "0.0.0.0";
bool cambioIp = 0;
long int minutosEnApMode = 0;
int minutoAuxAp = 0;
bool cambioFechaHora = false;


///////////pn532 declarations

// If using the breakout with SPI, define the pins for SPI communication.
#include <Adafruit_PN532.h>
#define PN532_SCK  (18)
#define PN532_MISO (19)
#define PN532_MOSI (23)
#define PN532_SS   (15)

#define PN532DEBUG

#define bootX 4
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
long int nfcDelay = 0;

uint8_t tag;
uint32_t tagId=0;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)


bool bussyMqtt=0;
// Uncomment just _one_ line below depending on how your breakout or shield
// is connected to the Arduino:

// Use this line for a breakout with a SPI connection:

// Use this line for a breakout with a hardware SPI connection.  Note that
// the PN532 SCK, MOSI, and MISO pins need to be connected to the Arduino's
// hardware SPI SCK, MOSI, and MISO pins.  On an Arduino Uno these are
// SCK = 13, MOSI = 11, MISO = 12.  The SS line can be any digital IO pin.
//Adafruit_PN532 nfc(PN532_SS);

// Or use this line for a breakout or shield with an I2C connection:
//Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);


/////
String Shora = "", Sfecha = "", Sday = "", Smonth = "", Syear = "", Shr = "", Smin = "";

char buf[20], buf1[20], buf2[20], buf3[20];
/////