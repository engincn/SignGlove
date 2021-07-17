#include "Datalib.h"

int finger_t = 20;
int gyro_t = 5;
int acc_t = 150;

Datalib::Datalib()
{

}

void Datalib::addSign(sensor* ilk,
                        float accx, float accy, float accz, float gyrox, float gyroy, float gyroz,
                        uint8_t thumb, uint8_t forefinger, uint8_t middlefinger, 
                        uint8_t ringfinger, uint8_t littlefinger, String cumlee)
{
    //sona isaret ekleme ve 1 sensor verisi ekleme
    sensor* temp;
    temp = new sensor;

    while (ilk->next != nullptr) {
        ilk = ilk->next;
    }
    if( ilk->next == nullptr )
    {
      ilk->next = temp;
      temp->AccX = accx;
      temp->AccY = accy;
      temp->AccZ = accz;
      temp->GyroX = gyrox;
      temp->GyroY = gyroy;
      temp->GyroZ = gyroz;
      temp->thumb = thumb;
      temp->foreFinger = forefinger;
      temp->middleFinger = middlefinger;
      temp->ringFinger = ringfinger;
      temp->littleFinger = littlefinger;
      temp->word = cumlee;
    }
    else {}
}

void Datalib::delSign(sensor* ilk)
{
    //sondan isaret silme
    while (ilk->next != nullptr) {
        ilk = ilk->next;
    }
    delete ilk;
}

/*
void Datalib::araelemanbul(sensor* ilk, uint8_t sayi)
{
    //aradan isaret yazdirma
    if (sayi == 1) {
        //cout << ilk->AccX << endl;
    }
    else {
        for (int i = 0; i < sayi - 1; i++) {
            ilk = ilk->next;
        }
        //cout << ilk->AccX << endl;
    }
}
*/

String Datalib::searchValue(sensor* ilk, 
                        float accx, float accy, float accz, float gyrox, float gyroy, float gyroz,
                        uint8_t thumb, uint8_t forefinger, uint8_t middlefinger, 
                        uint8_t ringfinger, uint8_t littlefinger)
{
    //girilen sensor degerini liste isinde ara ve karsilik gelen cumleyi yaz
    while (ilk->next != nullptr) {
        if ( ilk->AccX - acc_t <= accx && ilk->AccX + acc_t >= accx && 
             ilk->AccY - acc_t <= accy && ilk->AccY + acc_t >= accy && 
             ilk->AccZ - acc_t <= accz && ilk->AccZ + acc_t >= accz && 
             ilk->GyroX - gyro_t <= gyrox && ilk->GyroX + gyro_t >= gyrox && 
             ilk->GyroY - gyro_t <= gyroy && ilk->GyroY + gyro_t >= gyroy && 
             ilk->GyroZ - gyro_t <= gyroz && ilk->GyroZ + gyro_t >= gyroz && 
             ilk->thumb - finger_t <= thumb && ilk->thumb + finger_t >= thumb &&
             ilk->foreFinger - finger_t <= forefinger && ilk->foreFinger + finger_t >= forefinger &&
             ilk->middleFinger - finger_t <= middlefinger && ilk->middleFinger + finger_t >= middlefinger &&
             ilk->ringFinger - finger_t <= ringfinger && ilk->ringFinger + finger_t >= ringfinger &&
             ilk->littleFinger - finger_t <= littlefinger && ilk->littleFinger + finger_t >= littlefinger )
        {
            return ilk->word;    
        }
        else 
        {
            ilk = ilk->next;
        }
    }

    if ( ilk->next == nullptr )
    {
        if ( ilk->AccX - acc_t <= accx && ilk->AccX + acc_t >= accx && 
             ilk->AccY - acc_t <= accy && ilk->AccY + acc_t >= accy && 
             ilk->AccZ - acc_t <= accz && ilk->AccZ + acc_t >= accz && 
             ilk->GyroX - gyro_t <= gyrox && ilk->GyroX + gyro_t >= gyrox && 
             ilk->GyroY - gyro_t <= gyroy && ilk->GyroY + gyro_t >= gyroy && 
             ilk->GyroZ - gyro_t <= gyroz && ilk->GyroZ + gyro_t >= gyroz && 
             ilk->thumb - finger_t <= thumb && ilk->thumb + finger_t >= thumb &&
             ilk->foreFinger - finger_t <= forefinger && ilk->foreFinger + finger_t >= forefinger &&
             ilk->middleFinger - finger_t <= middlefinger && ilk->middleFinger + finger_t >= middlefinger &&
             ilk->ringFinger - finger_t <= ringfinger && ilk->ringFinger + finger_t >= ringfinger &&
             ilk->littleFinger - finger_t <= littlefinger && ilk->littleFinger + finger_t >= littlefinger )
        {
            return ilk->word;        
        }
        else
        {
            return "";
        }
    }

    return "";    
}
