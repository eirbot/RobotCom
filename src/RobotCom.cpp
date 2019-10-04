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

void RobotCom::receive(){
    char data[NB_OCTET_TRAME];
    //i2c.read(i2c_adress,data,NB_OCTET_TRAME);
    for(char i=0;i<NB_OCTET_TRAME;i++){
        buffer.push_back(data[i]);
    }
    process_data_reception();
}

char RobotCom::process_data_reception(){

    if(buffer.size() >= NB_OCTET_TRAME){
        char data[NB_OCTET_TRAME];
        retrive_complete_tram_from_buffer(data);
        decode(data);
        return 1;
    }else{
        return 0;
    }
}

void RobotCom::retrive_complete_tram_from_buffer(char *data){
    for (unsigned int i = 0; i < NB_OCTET_TRAME; i++)
    {
        data[i] = buffer.front();
        buffer.erase(buffer.begin());
    }
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

void RobotCom::decode(char *data){
    robot_com_frame data_frame;
    data_frame.command = data[0] >> 4;
    data_frame.object = data[0] & 0x0F;
    data_frame.object_id = data[1] >> 4;
    data_frame.property = data[1] & 0x0F;
    data_frame.data = data[2];
    set_decoded_data(data_frame);
}

void RobotCom::send(const char *data, int data_len, int adress){
    i2c.write(adress,data,data_len);
}

char RobotCom::compute_i2c_adress(products system_type, unsigned char system_id){
    return ((unsigned char)system_id+1)<<4 | system_type<<1;
}

char RobotCom::is_data_decoded_avaliable(){
    bool answer = decoded_data.size() != 0 ;
    return answer;
}
robot_com_frame RobotCom::get_decoded_data(){
    robot_com_frame tmp;
    tmp = decoded_data.front();
    decoded_data.erase(decoded_data.begin());
    return tmp;
}

void RobotCom::set_decoded_data(robot_com_frame frame){
    decoded_data.push_back(frame);
}