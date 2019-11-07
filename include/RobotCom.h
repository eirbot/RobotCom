#ifndef INC_ROBOTCOM
#define INC_ROBOTCOM

#include <vector>
#include "ObjectDef.h"
#include "mbed.h"
/*
La RobotCom est sur X bits
On communique par I2C
On a un MASTER (idéalement une raspberry PI que l'on nommera BRAIN)

Trame 8*HEXAS: 
|XX|XX|XX|
| COMMANDS OBJECT | OBJECT_ID PROPERTY | DATA_LEN | X


*/


class RobotCom
{
private:
    std::vector <char> buffer;
    products product_type;
    unsigned char product_id;
    int i2c_adress;
    I2C i2c;


public:
    RobotCom(products given_product_type, unsigned char given_product_id, PinName given_sda_pin, PinName given_scl_pin);
    ~RobotCom();
    void send(const char *data, int data_len, int adress);
    void encode(char *data, commands command, objects object, unsigned char object_id, unsigned char property, unsigned char data_to_send);
    void encode(char *data, unsigned char command);
    char compute_i2c_adress(products system_type, unsigned char system_id);
    char read(char *data, int address);
};


#endif