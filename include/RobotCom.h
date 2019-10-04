#ifndef INC_ROBOTCOM
#define INC_ROBOTCOM

#include <vector>
#include "ObjectDef.h"
#include "mbed.h"
/*
La RobotCom est sur 32 bits
On communique plustot par I2C
On a un MASTER (idéalement une raspberry PI que l'on nommera BRAIN)

Trame 8*HEXAS: 
|XX|XX|XX|
| COMMANDS OBJECT | OBJECT_ID PROPERTY | DATA |



*/

#define NB_OCTET_TRAME 3
#define I2CFREQ 100000

class RobotCom
{
private:
    std::vector <char> buffer;
    std::vector <robot_com_frame> decoded_data;
    products product_type;
    unsigned char product_id;
    int i2c_adress;
    void set_decoded_data(robot_com_frame frame);
    char process_data_reception();
    void retrive_complete_tram_from_buffer(char *data);
    void decode(char *data);


public:
    RobotCom(products given_product_type, unsigned char given_product_id, PinName given_sda_pin, PinName given_scl_pin);
    ~RobotCom();
    I2C i2c;
    void receive();
    void send(const char *data, int data_len, int adress);
    void encode(char *data, commands command, objects object, unsigned char object_id, unsigned char property, unsigned char data_to_send);
    void encode(char *data, unsigned char command);
    char compute_i2c_adress(products system_type, unsigned char system_id);
    char is_data_decoded_avaliable();
    robot_com_frame get_decoded_data();
};


#endif