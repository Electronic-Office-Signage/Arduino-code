#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "GxEPD2_display_selection_new_style.h"
#include <SPI.h>
#include <WiFiNINA.h>
#include "ParserLib.h"
#include <String.h>
#include <ArduinoLowPower.h>

//#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "IT Wireless";        // your network SSID (name)
char password[] = "deathpunch";
int status = WL_IDLE_STATUS;     // the Wifi radio's status


//Global Vars
// these need to change 
String title = " ";
String lastupdate = " ";
int form = 1;
String text1 = " ";



int titlecolor = GxEPD_BLACK;
int text1Color = GxEPD_BLACK;
int text2Color =GxEPD_BLACK;

void setup(){
  Serial.begin(9600);
  delay(5000);
  //Wifi first
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to open SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, password);
    // wait 10 seconds for connection:
    delay(10000);
  }
  
  //Get Json From server
  char server[] = "eos-services.onu.edu";    // name address for Google (using DNS)

  // Initialize the Ethernet client library
  // with the IP address and port of the server
  // that you want to connect to (port 80 is default for HTTP):
  WiFiClient client;
  char input[384];
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 5000)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /api/device?dev=12345 HTTP/1.1");
    client.println("Host: eos-services.onu.edu");
    client.println("Connection: close");
    client.println();
    delay(75);
    int i = 0;
    while (client.available()){
      char c = client.read();
      input[i]=c;
      i++;
    }
      client.stop();
      Serial.println(input);
  }
  else{
    Serial.println("failed to connect");
  }
  
  //[{"name": "t-huff", "uid": 12345, "time": "2022-04-04 22:20:56", "template_id": 0, "title_text": "yo mama", "title_color": 0, "box1_text": "yo mama house", "box1_color": 1}]
  Serial.println(input);
  int inputlength = strlen(input);
  Parser parser((byte*)input, inputlength);
  String junk = " ";
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String(' ');
  parser.Skip(1);
  Serial.println(junk);
  lastupdate = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(lastupdate);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String(' ');
  parser.Skip(1);
  Serial.println(junk);
  form = parser.Read_Int16();
  Serial.println(form);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  title = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(title);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  junk = parser.Read_String('"');
  parser.Skip(1);
  Serial.println(junk);
  text1 = parser.Read_String('"');
  Serial.println(text1);
  
  //start display connection
  display.init();
  //write to display
  create();
  //power screen off
  display.hibernate();
}

// sets up and wites to the screen
void create(){
  int titlex = 5;
  int titlewidth = 590;
  int radius = 5;
  int titleheight = 60;
  int titley = 5;
  int titleTexty = 45;
  if (form == 2){
    titley = 383;
    titleTexty = 423;
  }
 
  int text1X = 2;
  int text1Y = 70;
  int text1width = 593;
  int text1height = 373;
  int text1Texty = 100;
  if(form==2){
    text1Y = 5;
    text1Texty = 45;
  }
    display.setFullWindow();
    display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    
    display.drawRoundRect(titlex,titley,titlewidth,titleheight,radius,GxEPD_BLACK);
    display.drawRoundRect(text1X,text1Y,text1width,text1height,radius,GxEPD_BLACK);
    
    display.setRotation(0);
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(titlecolor);
    display.setTextSize(2);
    
    
    display.setCursor(15,titleTexty);
    display.print(title);
    display.setCursor(420,titleTexty);
    display.print(lastupdate);

    display.setTextSize(3);
    display.setTextColor(text1Color);
    display.setCursor(3,text1Texty);
    display.print(text1);
    }
      while (display.nextPage());
}
void loop(){
    Serial.println("about to snore");
    LowPower.sleep(900000);
    setup();
    Serial.println("Im Awake");
}
