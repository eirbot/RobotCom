#ifndef INC_ROBOTCOMSLAVE
#define INC_ROBOTCOMSLAVE

#include <vector>
#include "ObjectDef.h"
#include "mbed.h"
/*
La RobotCom est sur 24 bits
On communique par I2C
On a un MASTER (idéalement une raspberry PI que l'on nommera BRAIN)

Trame 8*HEXAS: 
|XX|XX|XX|
| COMMANDS OBJECT | OBJECT_ID PROPERTY | DATA |


*/

class RobotComSlave
{
private:
    std::vector <char> buffer;
    std::vector <char> output_buffer;
    std::vector <robot_com_frame> decoded_data;
    products product_type;
    unsigned char product_id;
    int i2c_adress;
    void set_decoded_data(robot_com_frame frame);
    char process_data_reception();
    void retrive_complete_tram_from_buffer(char *data);
    int get_all_data_from_output_buffer(char *data);
    void decode(char *data);
    I2CSlave slave;


public:
    RobotComSlave(products given_product_type, unsigned char given_product_id, PinName given_sda_pin, PinName given_scl_pin);
    ~RobotComSlave();
    void receive();
    void send(const char *data, int data_len);
    char compute_i2c_adress(products system_type, unsigned char system_id);
    char is_data_decoded_avaliable();
    robot_com_frame get_decoded_data();
    void prepare_to_send(const char *data, int data_len);
};

#endif