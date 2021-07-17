#include <stdint.h>
#include <WString.h>

using namespace std;

struct sensor
{
    float AccX, AccY, AccZ, GyroX, GyroY, GyroZ;
    uint8_t thumb, foreFinger, middleFinger, ringFinger, littleFinger;
    double error;
    String word;
    struct sensor* next = nullptr;
};

class Datalib
{
public:
    Datalib();
    void addSign(sensor* ilk, float accx, float accy, float accz, float gyrox, float gyroy, float gyroz,
                         uint8_t thumb, uint8_t forefinger, uint8_t middlefinger,
                         uint8_t ringfinger, uint8_t littlefinger, String cumlee);
    void delSign(sensor* ilk);
    //void araelemanbul(sensor* ilk, uint8_t sayi);
    String searchValue(sensor* ilk,
                        float accx, float accy, float accz, float gyrox, float gyroy, float gyroz,
                        uint8_t thumb, uint8_t forefinger, uint8_t middlefinger, 
                        uint8_t ringfinger, uint8_t littlefinger);

private:

};

