#include "RobotCom.h"
#include "ObjectDef.h"
#include "mbed.h"
#include <vector>



RobotCom::RobotCom(products given_product_type, unsigned char given_product_id, PinName given_sda_pin, PinName given_scl_pin) : i2c(given_sda_pin, given_scl_pin)
{
    product_type = given_product_type;
    product_id = given_product_id;
    i2c_adress = compute_i2c_adress(product_type,product_id);
    i2c.frequency(I2CFREQ);

}

RobotCom::~RobotCom()
{
}

void RobotCom::read(char *data, int len, int address){
    i2c.read(address,data,len);
}

/* Faire un cast unsigned int sur command*/
void RobotCom::encode(char *data, commands command, objects object, unsigned char object_id, unsigned char property, unsigned char data_to_send){
    data[0] = (command << 4) | object;
    data[1] =  (object_id << 4) | property;
    data[2] = data_to_send;
}

/* Faire un cast unsigned int sur command*/
void RobotCom::encode(char *data, unsigned char command){
    data[0] = command << 4;
    data[1] = NONE;
    data[2] = NONE;
}


void RobotCom::send(const char *data, int data_len, int adress){
    i2c.write(adress,data,data_len);
}

char RobotCom::compute_i2c_adress(products system_type, unsigned char system_id){
    return ((unsigned char)system_id+1)<<4 | system_type<<1;
}

