/* Web_Buzzer.pde - example sketch for Webduino library */

#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"

/* CHANGE THIS TO YOUR OWN UNIQUE VALUE.  The MAC number should be
 * different from any other devices on your network or you'll have
 * problems receiving packets. */
static uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/* CHANGE THIS TO MATCH YOUR HOST NETWORK.  Most home networks are in
 * the 192.168.0.XXX or 192.168.1.XXX subrange.  Pick an address
 * that's not in use and isn't going to be automatically allocated by
 * DHCP from your router. */
//static uint8_t ip[] = { 192, 168, 25, 100 };
static uint8_t ip[] = { 192, 168, 10, 10 };

/* all URLs on this server will start with /buzz because of how we
 * define the PREFIX value.  We also will listen on port 80, the
 * standard HTTP service port */
#define PREFIX "/buzz"
WebServer webserver(PREFIX, 80);

/* the piezo speaker on the Danger Shield is on PWM output pin #3 */
int GARAGEM_PIN = 5;
int ALARME_ON_PIN = 6;
int ALARME_OFF_PIN = 7;


/* this is the number of microseconds to wait after turning the
 * speaker on before turning it off. */
int controlValue = 0;
int control = 0;

/* toggle is used to only turn on the speaker every other loop
iteration. */
char toggle = 0;

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
       * named "buzz" here. */
      if (strcmp(name, "buzz") == 0)
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
      "<html><head>"
      "<title>Garagem 88 App</title>"
      //"<style type='text/css'> #image { width: 100%; height: 120px; background-size: contain; background-repeat: no-repeat; background-position: center; background-image: url('data:image/jpg;base64,/9j/4QAYRXhpZgAASUkqAAgAAAAAAAAAAAAAAP/sABFEdWNreQABAAQAAAAeAAD/4QMraHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wLwA8P3hwYWNrZXQgYmVnaW49Iu+7vyIgaWQ9Ilc1TTBNcENlaGlIenJlU3pOVGN6a2M5ZCI/PiA8eDp4bXBtZXRhIHhtbG5zOng9ImFkb2JlOm5zOm1ldGEvIiB4OnhtcHRrPSJBZG9iZSBYTVAgQ29yZSA1LjMtYzAxMSA2Ni4xNDU2NjEsIDIwMTIvMDIvMDYtMTQ6NTY6MjcgICAgICAgICI+IDxyZGY6UkRGIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8wMi8yMi1yZGYtc3ludGF4LW5zIyI+IDxyZGY6RGVzY3JpcHRpb24gcmRmOmFib3V0PSIiIHhtbG5zOnhtcE1NPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvbW0vIiB4bWxuczpzdFJlZj0iaHR0cDovL25zLmFkb2JlLmNvbS94YXAvMS4wL3NUeXBlL1Jlc291cmNlUmVmIyIgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIiB4bXBNTTpEb2N1bWVudElEPSJ4bXAuZGlkOkY1OTFCRDc5OUMxRDExRTRBN0ZBRDJCRTlFQURBRDA3IiB4bXBNTTpJbnN0YW5jZUlEPSJ4bXAuaWlkOkY1OTFCRDc4OUMxRDExRTRBN0ZBRDJCRTlFQURBRDA3IiB4bXA6Q3JlYXRvclRvb2w9IkFkb2JlIFBob3Rvc2hvcCBDUzYgKFdpbmRvd3MpIj4gPHhtcE1NOkRlcml2ZWRGcm9tIHN0UmVmOmluc3RhbmNlSUQ9InhtcC5kaWQ6RjZGRDI1N0YxQjlDRTQxMTg5REVGNUQxQ0I1NzQwMkUiIHN0UmVmOmRvY3VtZW50SUQ9InhtcC5kaWQ6RjZGRDI1N0YxQjlDRTQxMTg5REVGNUQxQ0I1NzQwMkUiLz4gPC9yZGY6RGVzY3JpcHRpb24+IDwvcmRmOlJERj4gPC94OnhtcG1ldGE+IDw/eHBhY2tldCBlbmQ9InIiPz7/7gAOQWRvYmUAZMAAAAAB/9sAhAAQCwsLDAsQDAwQFw8NDxcbFBAQ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');}</style>"
      "<script> function onSubmit() { var aguarde = document.getElementById('aguarde'); aguarde.style.display = 'block'; } </script>"
      "</head><body>"
      "<div id='aguarde' style='width: 100%;height: 80%; back;float: left;position: absolute;border: 0px;background-color: #FFF;top: 8%;text-align: center;font-family: monospace;font-size: 26px;display: none;'>Processando. Aguarde...</div>"
      "<h1 style='font-family: monospace;font-size: 40px;text-align: center;'>Garagem 88 App</h1>"
      //"<div id='image'></div>"
      "<form action='/buzz' method='POST' onsubmit='onSubmit()'>"
      "<p style='margin: 40px;'><button name='buzz' value='5' style='width: 100%;height: 10%;font-family: monospace;font-size: 35px;'>Portão ( Abrir / Fechar )</button></p>"
      "<p style='margin: 40px;'><button name='buzz' value='6' style='width: 100%;height: 10%;font-family: monospace;font-size: 35px;'>Ligar Alarme</button></p>"
      "<p style='margin: 40px;'><button name='buzz' value='7' style='width: 100%;height: 10%;font-family: monospace;font-size: 35px;'>Desligar Alarme</button></p>"
      "</form></body></html>";
      
    server.printP(message);
  }
}

void setup()
{
  Serial.begin(9600);
  
  // set the PWM output for the buzzer to out
  pinMode(GARAGEM_PIN, OUTPUT);
  pinMode(ALARME_ON_PIN, OUTPUT);
  pinMode(ALARME_OFF_PIN, OUTPUT);
  
  digitalWrite(GARAGEM_PIN, HIGH); 
  digitalWrite(ALARME_ON_PIN, HIGH); 
  digitalWrite(ALARME_OFF_PIN, HIGH); 

  // setup the Ehternet library to talk to the Wiznet board
  Ethernet.begin(mac, ip);

  /* register our default command (activated with the request of
   * http://x.x.x.x/buzz */
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
  
  if (control == 1) {
    Serial.print("control: ");
    Serial.println(control);
    if (controlValue == GARAGEM_PIN) {
      
        controlePortao();
        Serial.println("GARAGEM_PIN");
        
    } else if (controlValue == ALARME_ON_PIN) {
      
        controleLigarAlarme();
        Serial.println("ALARME_ON_PIN");
        
    } else if (controlValue == ALARME_OFF_PIN) {
      
        controleDesligarAlarme();
        Serial.println("ALARME_OFF_PIN");
        
    }
    control = 0;
    controlValue = 0;
  }
  /*if ((++toggle & 1) && (buzzDelay > 0))
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(buzzDelay);
    digitalWrite(BUZZER_PIN, LOW);
  }*/
}
//350997527
void controlePortao() {
  Serial.print("GARAGEM_PIN: ");
  Serial.println(GARAGEM_PIN);
  
  digitalWrite(GARAGEM_PIN, HIGH); 
  digitalWrite(ALARME_ON_PIN, HIGH); 
  digitalWrite(ALARME_OFF_PIN, HIGH); 
  
  digitalWrite(GARAGEM_PIN, LOW);
  delay(1500);
  digitalWrite(GARAGEM_PIN, HIGH); 
}

void controleLigarAlarme() {
  Serial.print("ALARME_ON_PIN: ");
  Serial.println(ALARME_ON_PIN);
  
  digitalWrite(GARAGEM_PIN, HIGH); 
  digitalWrite(ALARME_ON_PIN, HIGH); 
  digitalWrite(ALARME_OFF_PIN, HIGH); 
  
  digitalWrite(ALARME_ON_PIN, LOW);
  delay(3000);
  digitalWrite(ALARME_ON_PIN, HIGH); 
}

void controleDesligarAlarme() {
  Serial.print("ALARME_OFF_PIN: ");
  Serial.println(ALARME_OFF_PIN);
  
  digitalWrite(GARAGEM_PIN, HIGH); 
  digitalWrite(ALARME_ON_PIN, HIGH); 
  digitalWrite(ALARME_OFF_PIN, HIGH); 
  
  digitalWrite(ALARME_OFF_PIN, LOW);
  delay(3000);
  digitalWrite(ALARME_OFF_PIN, HIGH); 
}
