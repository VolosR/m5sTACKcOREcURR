#include <M5Core2.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <vector> 
#include <HTTPClient.h>
#include <ArduinoJson.h>   //https://github.com/bblanchon/ArduinoJson.git
#include "flags.h" 
#include "flags2.h" 

#define GREY 0x5AEB
#define orange 0xFC64
#define dark 0x2124
#define darkOrange 0x2ACA
#define red 0xFC7F
#define lightBlue 0x8FFE
#define light 0xCF3E

int posX2[4];
int n2=4;
int boxSize2=60;
int space2=3;
int start2=25;
String buttons2[3][4]={{"7","4","1","0"},{"8","5","2","."},{"9","6","3","ok"}};
int cF[3][4]={{0,1,2,3},{4,5,6,7},{8,9,10}};

const char* ssid     = "IGK20";       ///EDIIIT
const char* password = "18072019";
float amount=1.00000;
bool calcStarted=0;
bool dataStarted=0;
bool curStarted=0;
String math2="";

int chosen=2;
String payload="";
const String endpoint2 ="https://cdn.jsdelivr.net/gh/fawazahmed0/currency-api@1/latest/currencies/hrk.min.json";
String cur[11]={"eur","hrk","usd","gbp","aud","cad","chf","jpy","cny","nzd","rub"};
String big[22]={"EUR","HRK","USD","GBP","AUD","CAD","CHF","JPY","CNY","NZD","RUB"};
String curNames[11]={"Euro","Croatian kuna","US dollar","Pound sterling","Australian dollar","Canadian dollar","Swiss franc","Japanese yen","Chinese renminbi","New Zealand dollar","Russian ruble"};
StaticJsonDocument<6000> doc;

int fase=0;   //0=display data, //1=set amount //2=setCurency

void setup()
{
M5.begin();
M5.Lcd.setSwapBytes(true); 
M5.Lcd.setRotation(2);
M5.Lcd.fillScreen(BLACK);

for(int i=0;i<n2;i++)
   posX2[i]=(start2+(i*boxSize2)+(space2*i));

WiFi.begin(ssid, password);
M5.Lcd.print("connecting");

while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    M5.Lcd.print(".");
   }
M5.Lcd.print("CONECTED!!");
delay(1000);
M5.Lcd.fillScreen(BLACK);
getData(); 

}
int pres=1;
void loop(void)
{

  TouchPoint_t pos= M5.Touch.getPressPoint();
  int y=pos.y;
  int x=pos.x;


  if(fase==1)
  {
   if(calcStarted==0)
      startCalc();
    
      if (pos.x>0) {
        if(pres==1)
        {
        pres=0;
         for(int i=0;i<n2;i++)
          for(int j=0;j<n2;j++)
        if(pos.x>posX2[j] && pos.x<posX2[j]+boxSize2 && pos.y>posX2[i]+22 && pos.y<posX2[i]+boxSize2+22 )
            {
              if(buttons2[j][i]=="0" || buttons2[j][i]=="1" || buttons2[j][i]=="2" || buttons2[j][i]=="3" || buttons2[j][i]=="4" || buttons2[j][i]=="5" || buttons2[j][i]=="6" || buttons2[j][i]=="7" || buttons2[j][i]=="8" || buttons2[j][i]=="9" || buttons2[j][i]=="." )
              
              {
                math2=math2+String(buttons2[j][i]);
                M5.Lcd.drawString(math2,34,16);
              }
              if(buttons2[j][i]=="ok")
              
              {
                amount=math2.toFloat();
                fase=2;
                delay(300);
                }
            }  
         }
      }else {pres=1;}
}

 if(fase==0)
      {
        if(pos.x>2)
        {
        if(pres==1)
        {      
          pres=0;
          fase=1;
        }
        }else {pres=1;}
      }

 if(fase==2)
    {
      if(curStarted==0)
      startCur();
      
      if (pos.x>0) {
        if(pres==1)
        {
        pres=0;
         for(int i=0;i<n2;i++)
          for(int j=0;j<n2;j++)
        if(pos.x>posX2[j] && pos.x<posX2[j]+boxSize2 && pos.y>posX2[i]+22 && pos.y<posX2[i]+boxSize2+22 )
            {
          chosen=cF[j][i];
          
          getData();
            }  
         }
      }else {pres=1;}
    }   

}//end loop

void getData()
{
   M5.Lcd.setTextColor(WHITE,BLACK);
   M5.Lcd.fillScreen(BLACK);
   M5.Lcd.pushImage(0,2,48,48,flags2[chosen]);
   M5.Lcd.drawString("Currency",54,6,2);
   M5.Lcd.fillCircle(230,14,3,orange);
   M5.Lcd.fillCircle(220,14,3,ORANGE);
   M5.Lcd.fillCircle(210,14,3,ORANGE);
   
    
   M5.Lcd.drawString(big[chosen],52,24,4);
   M5.Lcd.drawString("Amount:",118,30,2);
   M5.Lcd.drawFloat(amount,5,180,30,2);
   
   if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
   HTTPClient http;
   String    endpoint="https://cdn.jsdelivr.net/gh/fawazahmed0/currency-api@1/latest/currencies/"+cur[chosen]+".min.json";
   http.begin(endpoint); //Specify the URL
   int httpCode = http.GET();  //Make the request
   if (httpCode > 0) { //Check for the returning code
   payload = http.getString();
   char inp[payload.length()];
   payload.toCharArray(inp,payload.length());
   deserializeJson(doc,inp);

   String date2=doc["date"];
   M5.Lcd.setTextColor(ORANGE,BLACK);
   M5.Lcd.drawString(date2,118,24,1);
   M5.Lcd.setTextColor(WHITE,BLACK);
   
   int j=0;
   for(int i=0;i<11;i++)
   {
    if(i!=chosen){
   String res=doc[cur[chosen]][cur[i]];
   float k=res.toFloat();
   float p=k*amount;
   M5.Lcd.pushImage(0,58+j*26,24,24,flags[i]);
   M5.Lcd.drawLine(32,80+j*26,240,80+j*26,GREY);
   M5.Lcd.drawString(curNames[i],32,62+j*26,2);
    M5.Lcd.setTextColor(lightBlue,BLACK);
   M5.Lcd.drawFloat(p,5,180,62+j*26,2);
   M5.Lcd.setTextColor(WHITE,BLACK);
  
   j++;
    }
   }
   }}

          fase=0;
          amount=1.00000;
          math2="";
          curStarted=0;
          dataStarted=0;
          calcStarted=0;
}   

   void startCalc()
  {
      calcStarted=1;
       M5.Lcd.fillScreen(BLACK);
       M5.Lcd.drawString("Set amount:",30,0,1);
       M5.Lcd.setTextColor(light);
       M5.Lcd.drawString("0",180,8,4);
       M5.Lcd.setTextFont(4);
   
    M5.Lcd.fillRoundRect(posX2[0],10,188,32,4,darkOrange);

    for(int i=0;i<3;i++)
    for(int j=0;j<4;j++){
        if(j<3 && i <3 )
        M5.Lcd.fillRoundRect(posX2[i],posX2[j]+22,boxSize2,boxSize2,4,dark);
          else
        M5.Lcd.fillRoundRect(posX2[i],posX2[j]+22,boxSize2,boxSize2,4,dark);
        M5.Lcd.drawString(String(buttons2[i][j]), posX2[i]+22, posX2[j]+42);
    
    }}

void startCur()
    { 
      curStarted=1;
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.drawString("Set currency:",30,24,2);
       int m=0;
       for(int i=0;i<3;i++)
        for(int j=0;j<4;j++)
                            {
          if(m<11)
          M5.Lcd.pushImage(posX2[i],posX2[j]+22,48,48,flags2[m]);
          M5.Lcd.drawString(big[m],posX2[i]+14,posX2[j]+70,2);
          m++;
                            }
    }
     
