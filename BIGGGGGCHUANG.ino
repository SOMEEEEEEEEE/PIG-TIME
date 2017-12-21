#include <Adafruit_NeoPixel.h>
/*#include <avr/sleep.h>
#include <avr/power.h>*/
#include "audio.h"
#include <SoftwareSerial.h>

/***********************光敏初始化***********************************/
#define Light_PIN A0 //初始化接口
#define Light_value 900  //初始化亮度门限值
int sensorValue;  //将读取到的光强保存到sensorValue中
/***********************光敏初始化***********************************/

/***********************计时器初始化***********************************/
long previousMillis = 0;
long interval1 = 10000;      //主计时时长;
long interval2 = 5000; //副计时时长
long interval3 = 3000;  //铃声播放时长
unsigned long currentMillis=0;
/***********************计时器初始化***********************************/

/***********************触摸开关初始化***********************************/
const int buttonPin = 2;     //接口
int buttonState = 0;  //初始化接口值为0
/***********************触摸开关初始化***********************************/

/***********************MP3初始化***********************************/
int music_vol = 26; //初始音量0~30
bool playing = false;
/***********************MP3初始化***********************************/

void(* resetFunc) (void) = 0; //定义重置函数
void setup() {
    Serial.begin(9600);
    audio_init(DEVICE_Flash,4,music_vol);  //初始化mp3
    pinMode(buttonPin, INPUT);//配置引脚的模式为输入模式
    Serial.println("Initialisation complete.");
}

void loop()
{
  sensorValue = analogRead(Light_PIN);  //读取光敏数据
  Serial.println(sensorValue);//将光敏数据传输到串口
  /**********************************当光强低于门限值**********************************/
  if(sensorValue<Light_value)
  {
    currentMillis = millis(); //开始主计时
    Serial.print("WORKING...");
    Serial.print(currentMillis - previousMillis,DEC);
    Serial.println(" seconds");
    delay(1000);
  /************************************主计时结束*************************************/
    if(currentMillis - previousMillis > interval1)
    {
      Serial.println("First Ring"); //响铃
    if (!playing)
    {
      Serial.println("play sound.....");
      audio_play();
     audio_choose(1); //播放第一首铃声
    }
    /******************************在响铃时判断触摸开关状态**********************************/
       do
      {
        previousMillis = previousMillis +500;
         buttonState = digitalRead(buttonPin); //读取开关状态
         Serial.println(buttonState);      
    Serial.print("KEEP ");
    Serial.print(previousMillis,DEC);
    Serial.println(" seconds");
             delay(500); //刷新频率
      }while((previousMillis< interval3)&&(buttonState == 1));
   /*****************************响铃结束***********************************/
    playing = false;
    audio_reset();
    Serial.println("stop sound.....");
 /****************************如果开关未被按下，开始副计时*****************************/
     previousMillis = 0; //计时清零
while(buttonState) //开关未被按下，循环执行
{
    previousMillis = previousMillis + 1000;  
    Serial.print("SECOND WORKING...");
    Serial.print(previousMillis + 1000,DEC);
    Serial.println(" seconds");
    delay(1000);
    /******************副计时结束***********************/
    if(previousMillis > interval2)
    {
       Serial.println("Second Ring"); //二次响铃
           if (!playing)
    {
      Serial.println("play sound.....");
      audio_play();
     audio_choose(2);
      playing = true;
    }
        /**************在响铃时判断触摸开关状态****************/
         previousMillis = 0; //计时清零
       do
      {
         previousMillis =  previousMillis +500;
         buttonState = digitalRead(buttonPin); //读取开关状态
         Serial.println(buttonState);      
    Serial.print("KEEP ");
    Serial.print(previousMillis,DEC);
    Serial.println(" seconds");
             delay(500); //刷新频率
      }while((previousMillis < interval3)&&(buttonState == 1));
      /************响铃结束****************/
    playing = false;
    audio_reset();
    Serial.println("stop sound.....");
    previousMillis = 0; //计时清零
    }
}  
if(!buttonState)
resetFunc();  //当开关被按下，计时器清零
    }

  }
  /****************************照明亮起时**************************/
  /*****************铃声停止，计时器清零*******************/
  else
  {
    playing = false;  
    audio_reset();
    Serial.println("stop sound...");
    resetFunc(); 
  }
}


