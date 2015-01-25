/* Based on Web_Buzzer.pde - example sketch for Webduino library */

#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"

#include <DS1307.h>
#include <TimerOne.h>

/* CHANGE THIS TO YOUR OWN UNIQUE VALUE.  The MAC number should be
 * different from any other devices on your network or you'll have
 * problems receiving packets. */
static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/* CHANGE THIS TO MATCH YOUR HOST NETWORK.  Most home networks are in
 * the 192.168.0.XXX or 192.168.1.XXX subrange.  Pick an address
 * that's not in use and isn't going to be automatically allocated by
 * DHCP from your router. */
//static uint8_t ip[] = { 192, 168, 25, 100 };
//static uint8_t ip[] = { 192, 168, 10, 10 };
static uint8_t ip[] = { 192, 168, 0, 100 };

/* all URLs on this server will start with /buzz becaus of how we
 * define the PREFIX value.  We also will listen on port 80, the
 * standard HTTP service port */
#define PREFIX "/hcs"
WebServer webserver(PREFIX, 88);

/* the piezo speaker on the Danger Shield is on PWM output pin #3 */
int GARAGEM_PIN = 5;
int ALARME_PIN = 6;
//int ALARME_OFF_PIN = 7;

DS1307 rtc(A4, A5); // SDA, SCL

boolean alarme = false;
//boolean //desalarme = false;

char *newTime;

/* this is the number of microseconds to wait after turning the
 * speaker on before turning it off. */
int controlValue = 0;
int control = 0;

/* This command is set as the default command for the server.  It
 * handles both GET and POST requests.  For a GET, it returns a simple
 * page with some buttons.  For a POST, it saves the value posted to
 * the buzzDelay variable, affecting the output of the speaker */
void buzzCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (type == WebServer::POST)
  {
    bool repeat;
    char name[16], value[16];
    do
    {
      /* readPOSTparam returns false when there are no more parameters
       * to read from the input.  We pass in buffers for it to store
       * the name and value strings along with the length of those
       * buffers. */
      repeat = server.readPOSTparam(name, 16, value, 16);

      /* this is a standard string comparison function.  It returns 0
       * when there's an exact match.  We're looking for a parameter
       * named "hcs" here. */
      if (strcmp(name, "hcs") == 0)
      {
	/* use the STRing TO Unsigned Long function to turn the string
	 * version of the delay number into our integer buzzDelay
	 * variable */
        controlValue = strtoul(value, NULL, 10);
        control = 1;
        Serial.println(controlValue);
      }
    } while (repeat);
    
    // after procesing the POST data, tell the web browser to reload
    // the page using a GET method. 
    server.httpSeeOther(PREFIX);
    return;
  }

  /* for a GET or HEAD, send the standard "it's all OK headers" */
  server.httpSuccess();

  /* we don't output the body for a HEAD request */
  if (type == WebServer::GET)
  {
    /* store the HTML in program memory using the P macro */
    P(message) = 
      "<html><head><title>Garagem 88</title>"
      "<link rel='stylesheet' href='http://www.ricardoteix.com/labs/garagem88/style.css'>"
      "<script src='http://www.ricardoteix.com/labs/garagem88/gps.js'></script>"
      "<script>getLocation();</script></head>"
      "<body>"
      "<div id='aguarde'>Processando. Aguarde...</div>"
      "<h1>Garagem 88</h1>"
      "<form action='hcs' method='POST'  onsubmit='onSubmit()'>"
      "<p style='margin: 40px;'><button name='hcs' value='5'>Portão ( Abrir / Fechar )</button></p>"
      "<p style='margin: 40px;'><button name='hcs' value='6'>Alarme ( Ligar / Desligar )</button></p>"
      "</form></body></html>";
      
    server.printP(message);
  }
}

void setup()
{
  Serial.begin(9600);
  
  Timer1.initialize(1000000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr );
  
  //Aciona o relogio
  rtc.halt(false);
   
  //As linhas abaixo setam a data e hora do modulo
  //e podem ser comentada apos a primeira utilizacao
  //rtc.setDOW(FRIDAY);      //Define o dia da semana
  //rtc.setTime(1,30, 0);     //Define o horario
  //rtc.setDate(23, 1, 2015);   //Define o dia, mes e ano
   
  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);
   
  
  // set the PWM output for the buzzer to out
  pinMode(GARAGEM_PIN, OUTPUT);
  pinMode(ALARME_PIN, OUTPUT);
  //pinMode(ALARME_OFF_PIN, OUTPUT);
  
  digitalWrite(GARAGEM_PIN, HIGH); 
  digitalWrite(ALARME_PIN, HIGH); 
 // digitalWrite(ALARME_OFF_PIN, HIGH); 

  // setup the Ehternet library to talk to the Wiznet board
  Ethernet.begin(mac, ip);

  /* register our default command (activated with the request of
   * http://x.x.x.x/hcs */
  webserver.setDefaultCommand(&buzzCmd); 
  /* start the server to wait for connections */
  webserver.begin();
}
void loop()
{
  // process incoming connections one at a time forever
  webserver.processConnection();

  /* every other time through the loop, turn on and off the speaker if
   * our delay isn't set to 0. */
   
  if (alarme) {
    Serial.println("#### LIGA / DESLIGA ALARME ###");
    controleAlarme();
    alarme = false;
  }
  
  /*
  if (//desalarme) {
    Serial.println("#### DESLIGA ALARME ###");
    controle//desalarme();
    //desalarme = false;
  }
  */
  
  if (control == 1) {
    Serial.print("control: ");
    Serial.println(control);
    if (controlValue == GARAGEM_PIN) {
      
        controlePortao();
        Serial.println("GARAGEM_PIN");
        
    } else if (controlValue == ALARME_PIN) {
      
        controleAlarme();
        Serial.println("ALARME_PIN");
        
    }/* else if (controlValue == ALARME_OFF_PIN) {
      
        controle//desalarme();
        Serial.println("ALARME_OFF_PIN");
        
    }*/
    control = 0;
    controlValue = 0;
  }
}
//350997527
void controlePortao() {
  Serial.print("GARAGEM_PIN: ");
  Serial.println(GARAGEM_PIN);
  
  digitalWrite(GARAGEM_PIN, HIGH); 
  digitalWrite(ALARME_PIN, HIGH); 
  //digitalWrite(ALARME_OFF_PIN, HIGH); 
  
  digitalWrite(GARAGEM_PIN, LOW);
  delay(1500);
  digitalWrite(GARAGEM_PIN, HIGH); 
}

void controleAlarme() {
  Serial.print("ALARME_PIN: ");
  Serial.println(ALARME_PIN);
  
  digitalWrite(GARAGEM_PIN, HIGH); 
  digitalWrite(ALARME_PIN, HIGH); 
  //digitalWrite(ALARME_OFF_PIN, HIGH); 
  
  digitalWrite(ALARME_PIN, LOW);
  delay(3000);
  digitalWrite(ALARME_PIN, HIGH); 
}

void timerIsr() {
  //Mostra as informações no Serial Monitor
  char *timeNow = rtc.getTimeStr();
  Serial.print("Hora : ");
  Serial.println(timeNow);
  
  if (strcmp(timeNow, "22:00:00") == 0 || strcmp(timeNow, "05:30:00") == 0) {
      alarme = true;
  }
  /*
  if (strcmp(timeNow, "05:30:00") == 0) {
      //desalarme = true;
  }
  */
}


/*
void controle//desalarme() {
  Serial.print("ALARME_OFF_PIN: ");
  Serial.println(ALARME_OFF_PIN);
  
  digitalWrite(GARAGEM_PIN, HIGH); 
  digitalWrite(ALARME_PIN, HIGH); 
  //digitalWrite(ALARME_OFF_PIN, HIGH); 
  
  digitalWrite(ALARME_OFF_PIN, LOW);
  delay(3000);
  digitalWrite(ALARME_OFF_PIN, HIGH); 
}
*/
