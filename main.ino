#include "I2Cdev.h"
#include <Wire.h>


#include "Datalib.h"
#include <LiquidCrystal.h>

#define THUMB A0
#define FOREFINGER A1
#define MIDDLEFINGER A2
#define RINGFINGER A3
#define LITTLEFINGER A6
#define BUTTON 7
#define MOD A7
#define LED 10

int acc_error=0;                         //We use this variable to only calculate once the Acc data error
float rad_to_deg = 180/3.141592654;      //This value is for pasing from radians to degrees values
float Acc_rawX, Acc_rawY, Acc_rawZ;    //Here we store the raw data read 
float Acc_angle_x, Acc_angle_y;          //Here we store the angle value obtained with Acc data
float Acc_angle_error_x, Acc_angle_error_y; //Here we store the initial Acc data error


float accx;
float accy;
float accz;
float gyrox=0;
float gyroy=0;
float gyroz=0;                                  // MPU'dan gelen veriler kaydedilecek

int t = 0;
int z = 0;

sensor *sign;
Datalib *datalib = new Datalib();

LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
String voice = "";
char c;
int mod;
int lcd_count;
int mod_count = 0;

String foundWord;
String prevWord = "test";
String sendVoice = "";

void mpuStabilize();

void lcdprint( String kelime );
void ekle();
void sil();
void ara();
void yaz();
String listen();
uint8_t imlec;






void setup() {
 
  Wire.begin();                           //begin the wire comunication
  
  Wire.beginTransmission(0x68);           //begin, Send the slave adress (in this case 68)              
  Wire.write(0x6B);                       //make the reset (place a 0 into the 6B register)
  Wire.write(0x00);
  Wire.endTransmission(true);             //end the transmission
  
  Wire.beginTransmission(0x68);           //Start communication with the address found during search.
  Wire.write(0x1C);                       //We want to write to the ACCEL_CONFIG register
  Wire.write(0x10);                       //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true); 
  
  if(acc_error==0)
  {
    for(int a=0; a<200; a++)
    {
      Wire.beginTransmission(0x68);
      Wire.write(0x3B);                       //Ask for the 0x3B register- correspond to AcX
      Wire.endTransmission(false);
      Wire.requestFrom(0x68,6,true); 
      
      Acc_rawX=(Wire.read()<<8|Wire.read())/4096.0 ; //each value needs two registres
      Acc_rawY=(Wire.read()<<8|Wire.read())/4096.0 ;
      Acc_rawZ=(Wire.read()<<8|Wire.read())/4096.0 ;

      
      /*---X---*/
      Acc_angle_error_x = Acc_angle_error_x + ((atan((Acc_rawY)/sqrt(pow((Acc_rawX),2) + pow((Acc_rawZ),2)))*rad_to_deg));
      /*---Y---*/
      Acc_angle_error_y = Acc_angle_error_y + ((atan(-1*(Acc_rawX)/sqrt(pow((Acc_rawY),2) + pow((Acc_rawZ),2)))*rad_to_deg)); 
      
      if(a==199)
      {
        Acc_angle_error_x = Acc_angle_error_x/200;
        Acc_angle_error_y = Acc_angle_error_y/200;
        acc_error=1;
      }
    }
  }//end of error calculation   

  Serial.begin(9600);
  
  t_char();
  lcd.begin(16, 2);

  sign = new sensor;
  sign->AccX = 125;
  sign->word = "hosgeldin!!!";
  sign->next = nullptr;

  pinMode( THUMB, INPUT);
  pinMode( FOREFINGER, INPUT);
  pinMode( MIDDLEFINGER, INPUT);
  pinMode( RINGFINGER, INPUT);
  pinMode( LITTLEFINGER, INPUT);

  pinMode( BUTTON, INPUT );
  pinMode( LED, OUTPUT);

  lcd.print(sign->word);
  delay(500);
  lcd.clear();







}


void loop()
{
  
  if (z <= 4)
  {
    z++;
  }
  else
  {
    delay(200);
    mod = 0;
    mod = analogRead(MOD);

    if ( 760 < mod && mod < 780 )
    {
      delay(1000);
      lcd.clear();
      //Serial.println("ekle");
     
      if( mod_count != 1 )
      {
        Serial.println("ekle modu");
        lcd.print ("Ekle Modu");
        delay(1000);
        lcd.clear();  
        mod_count = 1;
      }
      
      ekle();
    }
    else if ( 680 < mod && mod < 700  )
    {
      //lcd.clear();
      ara();
      //Serial.println("ara");
      if( mod_count != 2 )
      {
        Serial.println("ara modu");
        lcd.print ("Ara Modu");
        delay(1000);
        lcd.clear();  
        mod_count = 2;
      }
    }
    else if ( 500 < mod && mod < 520 )
    {
      yaz();
      //Serial.println("yaz");
      if( mod_count != 3 )
      {
        Serial.println("yaz modu");
        lcd.print ("Yaz Modu");
        delay(1000);
        lcd.clear();  
        mod_count = 3;
      }
    }
    else
    {
      lcd.clear();
      sil();
      //Serial.println("sil");
      if( mod_count != 4 )
      {
        Serial.println("sil modu");
        lcd.print ("Sil Modu");
        delay(1000);
        lcd.clear();  
        mod_count = 4;
      }
    }
    z = 0;
  }

  if (lcd_count >= 10000)
  {
    lcd.clear();
    lcd_count = 0;
  }
  else
    lcd_count++;
}


void ekle()
{
  lcd_count = 0;
  lcd.print("kelimeyi gir");
  //Serial.println("kelimeyi gir:");
  delay(100);

  voice = "";
  voice = listen();

  if ( voice != "" )
  {
    lcd.clear();
    lcd.print( tcr(voice) );
    mpuStabilize();
    datalib->addSign(sign, accz, accy, accz, 100, 100, 100, map(analogRead(THUMB), 0, 1023, 0, 255), map(analogRead(FOREFINGER), 0, 1023, 0, 255),
                     map(analogRead(MIDDLEFINGER), 0, 1023, 0, 255), map(analogRead(RINGFINGER), 0, 1023, 0, 255), map(analogRead(LITTLEFINGER), 0, 1023, 0, 255), voice );
  
    delay(3000);
    lcd.clear();
    lcd.print( tcr(voice) + " eklendi");
    Serial.println(voice + " eklendi");
    
    delay(1000);
    lcd.clear();
    lcd_count = 0;
    voice = "";
  }
  else
  {
    /* code */
  }
  delay(200);
}

void sil()
{
    if (digitalRead(BUTTON) == HIGH )
    {
      datalib->delSign(sign);
      lcd.clear();
      lcd.print("silindi");
      //Serial.println("silindi");
      lcd_count = 0;
      delay(5000);
    }
    else
    {
    
    }
}

void ara()
{
  if (sendVoice == "")
  {
     lcd.clear();
  }
  
  foundWord = "";
  mpuStabilize();
  foundWord = datalib->searchValue(sign, accx, accy, accz, 100, 100, 100, map(analogRead(THUMB), 0, 1023, 0, 255), map(analogRead(FOREFINGER), 0, 1023, 0, 255),
                                   map(analogRead(MIDDLEFINGER), 0, 1023, 0, 255), map(analogRead(RINGFINGER), 0, 1023, 0, 255), map(analogRead(LITTLEFINGER), 0, 1023, 0, 255));

  //Serial.println(foundWord);
  delay(200);
  if (prevWord != foundWord && foundWord != "")
  {
    prevWord = foundWord;
    //            lcd.clear();
    //            lcd.print( foundWord );
    //lcd.print( tcr(foundWord) );
    lcd.print(foundWord);
    //Serial.print( foundWord );
    lcd_count = 0;
    //            imlec = 0;
    sendVoice += foundWord;
    sendVoice += " ";
  }
  
  if (digitalRead(BUTTON) == HIGH )
  {
      Serial.println(sendVoice);
      lcd.clear();
      imlec = 0;
      sendVoice = "";
  }

}

void yaz()
{
 
  String prevVoice = "";
  while( 500 < mod && mod < 520 )
  {
  voice = "";
  voice = listen();

  if (voice == "")
  {
    //Serial.println("cik");
    return;
  }
  if ( voice != prevVoice && voice != "" )
  {
      lcd.clear();
      prevVoice = voice;  
  }
  
  lcd.print(tcr(voice));
  Serial.println(voice + "gonderildi" );
  if (voice.length() > 16)
  {
    delay(50);
    for (int positionCounter = 0 ; positionCounter + 14 < voice.length() ; positionCounter++)
    {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(500);
    }
    delay(50);
  }
  else
  {
    lcd_count = 0;
  }
  }
}

String listen()
{
  bool receivervoice = true;
  while ( receivervoice == true )
  {
    while ( Serial.available() > 0 )
    {
      delay(10);
      lcd.clear();
      c = Serial.read();
      voice += c;
    }
    delay(100);
    if ( voice != "" || analogRead(MOD) <= 750 )
    {
      receivervoice = false;
    }
  }
  return voice;
}


void lcdprint( String kelime )
{
  if ( ( imlec + kelime.length() ) > 16 )
  {
    lcd.setCursor(0, 1);
    lcd.print( tcr(kelime) );
    lcd.print(" ");
    imlec = 0;
    imlec += kelime.length() + 1;
  }
  else
  {
    lcd.print( tcr(kelime) );
    lcd.print(" ");
    imlec += kelime.length() + 1;
  }

}

void t_char() {
  byte customChar_g[] = {
    B01110,
    B00000,
    B01111,
    B10001,
    B01111,
    B00001,
    B01110,
    B00000
  };

  byte customChar_c[] = {
    B00000,
    B00000,
    B01110,
    B10000,
    B10001,
    B01110,
    B00100,
    B00000
  };

  byte customChar_C[] = {
    B01110,
    B10001,
    B10000,
    B10000,
    B10001,
    B01110,
    B00100,
    B00000
  };

  byte customChar_s[] = {
    B00000,
    B01110,
    B10000,
    B01110,
    B00001,
    B11110,
    B00100,
    B00000
  };

  byte customChar_i[] = {
    B00000,
    B00000,
    B01100,
    B00100,
    B00100,
    B00100,
    B01110,
    B00000
  };

  byte customChar_U[] = {
    B01010,
    B00000,
    B10001,
    B10001,
    B10001,
    B10001,
    B01110,
    B00000
  };

  byte customChar_S[] = {
    B01111,
    B10000,
    B11100,
    B00111,
    B00001,
    B11110,
    B00100,
    B00000
  };

  /*
    byte customChar_whyNot[] = {
    B00000,
    B01010,
    B01010,
    B00000,
    B00000,
    B10001,
    B01110,
    B00000
    };
  */
  lcd.createChar(0, customChar_g);
  lcd.createChar(1, customChar_c);
  lcd.createChar(2, customChar_C);
  lcd.createChar(3, customChar_s);
  lcd.createChar(4, customChar_i);
  lcd.createChar(5, customChar_U);
  lcd.createChar(6, customChar_S);
  //lcd.createChar(7, customChar_whyNot);
}

String tcr(String rep) {
  rep.replace("ğ", "\x08");
  rep.replace("Ğ", "G");
 // rep.replace("ç", "\x01");
 // rep.replace("Ç", "\x02");
  rep.replace("ş", "\x03");
  rep.replace("Ş", "\x06");
  rep.replace("ü", "\xF5");
  rep.replace("Ü", "\x05");
  rep.replace("ö", "\xEF");
  rep.replace("Ö", "\xEF");
  rep.replace("ı", "\x04");
  rep.replace("İ", "\x04");
  return rep;
}

void mpuStabilize()
{
  Wire.beginTransmission(0x68);     //begin, Send the slave adress (in this case 68) 
  Wire.write(0x3B);                 //Ask for the 0x3B register- correspond to AcX
  Wire.endTransmission(false);      //keep the transmission and next
  Wire.requestFrom(0x68,6,true);    //We ask for next 6 registers starting withj the 3B  
  accx=((Wire.read()<<8|Wire.read())/4096.0)*100 ; //each value needs two registres
  accy=((Wire.read()<<8|Wire.read())/4096.0)*100 ;
  accz=((Wire.read()<<8|Wire.read())/4096.0)*100 ; 

  
}
