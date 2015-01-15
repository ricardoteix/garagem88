#include <SdFat.h>
#include <SdFatUtil.h>
#include <SPI.h>
#include <Ethernet.h>
//#include <EthernetDHCP.h>
#include <LiquidCrystal.h>
#include <string.h>
 
#define BUFSIZ 100
 
byte mac[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };
byte ip[] = {192, 168, 10, 10};
Server server(80);
long previousMillis = 0;
long interval = 600 * 1000;
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;
LiquidCrystal lcd(9, 8, 7, 6, 5, 3);
int redLed = 2;
int greenLed = 1;
long redCounter = 0;
long greenCounter = 0;
 
void printCounters() {
    char s1[21];
    char s2[21];
    char t[21];
    unsigned long mls = millis();
    unsigned long secs = mls / 1000;
    unsigned long mins = secs / 60;
    unsigned long hours = mins / 60;
    unsigned long days = hours / 24;
    secs = secs - (mins * 60);
    mins = mins - (hours * 60);
    hours = hours - (days * 24);
    sprintf(s1, "Code 200: %d", greenCounter);
    sprintf(s2, "Code 404: %d", redCounter);
    //sprintf(t, "%02d %02d:%02d:%02d", days, hours, mins, secs);
    sprintf(t, "Uptime: %lu ms", mls);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(s1);
    lcd.setCursor(0, 1);
    lcd.print(s2);
    lcd.setCursor(0, 2);
    lcd.print(t);
}
 
String getIPString(byte b[]){
  String ipStr = "";
  int length = 4;
  for (int i = 0; i < length; ++i)
    if (i < length-1)
      ipStr += String((long)b[i]) + ".";
    else
      ipStr += String((long)b[i]);
  return ipStr;
}
 
void ListFiles(Client cli, uint8_t flags) {
  dir_t p;
  
  root.rewind();
  cli.println("<ul>");
  while (root.readDir(p) > 0) {
    if (p.name[0] == DIR_NAME_FREE) break;
    if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') continue;
    if (!DIR_IS_FILE_OR_SUBDIR(&p)) continue;
    cli.print("<li><a href=\"");
    for (uint8_t i = 0; i < 11; i++) {
      if (p.name[i] == ' ') continue;
      if (i == 8) {
        cli.print('.');
      }
      cli.print(p.name[i]);
    }
    cli.print("\">");
    for (uint8_t i = 0; i < 11; i++) {
      if (p.name[i] == ' ') continue;
      if (i == 8) {
        cli.print('.');
      }
      cli.print(p.name[i]);
    }
    cli.print("</a>");
    if (DIR_IS_SUBDIR(&p)) {
      cli.print('/');
    }
    if (flags & LS_DATE) {
       root.printFatDate(p.lastWriteDate);
       cli.print(' ');
       root.printFatTime(p.lastWriteTime);
    }
    if (!DIR_IS_SUBDIR(&p) && (flags & LS_SIZE)) {
      cli.print(' ');
      cli.print(p.fileSize);
    }
    cli.println("</li>");
  }
  cli.println("</ul>");
}
 
char* mimetype(String filename, int index)
{
    if (index == -1) {
        return "text/plain";
    }
    String ext = filename.substring(index + 1);
    if (ext.equalsIgnoreCase("htm")) {
        return "text/html";
    }
    else if (ext.equalsIgnoreCase("css")) {
        return "text/css";
    }
    else if (ext.equalsIgnoreCase("jpg")) {
        return "image/jpeg";
    }
    else if (ext.equalsIgnoreCase("png")) {
        return "image/png";
    }
    else if (ext.equalsIgnoreCase("gif")) {
        return "image/gif";
    }
    else if (ext.equalsIgnoreCase("js")) {
        return "application/javascript";
    }
    
    return "text/plain";
}
 
void setup() {
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("Web server starting");
  lcd.setCursor(0, 1);
  lcd.print("...");
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, HIGH);
  delay(2000);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  if (!card.init(SPI_HALF_SPEED, 4)) lcd.print("card.init failed!");
  if (!volume.init(&card)) lcd.print("vol.init failed!");
  if (!root.openRoot(&volume)) lcd.print("openRoot failed");
  //EthernetDHCP.begin(mac);
  Ethernet.begin(mac, ip);
  server.begin();
}
 
void loop()
{
  char clientline[BUFSIZ];
  int index = 0, startExt = 0, startHttp = 0;
  unsigned long currentMillis = 0;
  String clientlineString;
  String filenameString;
  
  Client cli = server.available();
  if (cli) {
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print(getIPString(cli.getRemoteIP()));
    boolean current_line_is_blank = true;
    index = 0;
    while (cli.connected()) {
      if (cli.available()) {
        char c = cli.read();
        if (c != '\n' && c != '\r') {
          clientline[index] = c;
          index++;
          if (index >= BUFSIZ) 
            index = BUFSIZ -1;
          continue;
        }
        clientline[index] = 0;
        clientlineString = String(clientline);
        startHttp = clientlineString.indexOf("HTTP");
        //lcd.setCursor(0, 1);
        //lcd.print(clientlineString.substring(0, startHttp));
        //lcd.setCursor(0, 2);
        //lcd.print(clientlineString.substring(startHttp));
        if (strstr(clientline, "GET / ") != 0) {
          ++greenCounter;
          cli.println("HTTP/1.1 200 OK");
          cli.println("Content-Type: text/html");
          cli.println();
          cli.println("<h2>Files:</h2>");
          ListFiles(cli, LS_SIZE);
        }
        else if (strstr(clientline, "GET /") != 0) {
          char *filename;
          filename = clientline + 5;
          (strstr(clientline, " HTTP"))[0] = 0;
          filenameString = clientlineString.substring(5, startHttp - 1);
          startExt = filenameString.lastIndexOf('.');
          if (! file.open(&root, filename, O_READ)) {
            ++redCounter;
            digitalWrite(redLed, HIGH);
            cli.println("HTTP/1.1 404 Not Found");
            cli.println("Content-Type: text/html");
            cli.println();
            cli.println("<h2>File Not Found!</h2>");
            break;
          }
          ++greenCounter;
          digitalWrite(greenLed, HIGH);
          cli.println("HTTP/1.1 200 OK");
          cli.print("Content-Type: ");
          cli.println(mimetype(filenameString, startExt));
          cli.println();
          int16_t c;
          while ((c = file.read()) > 0) {
              cli.print((char)c);
          }
          file.close();
        }
        else {
          digitalWrite(redLed, HIGH);
          cli.println("HTTP/1.1 404 Not Found");
          cli.println("Content-Type: text/html");
          cli.println();
          cli.println("<h2>File Not Found!</h2>");
        }
        break;
      }
    }
    delay(1);
    cli.stop();
    delay(50);
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, LOW);
    if (greenCounter % 5 == 0) {
        printCounters();
    }
    /*
    currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
        previousMillis = currentMillis;
        //EthernetDHCP.maintain();
    }
    */
  }
}
