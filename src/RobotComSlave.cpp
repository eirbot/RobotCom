#include "RobotComSlave.h"
#include "ObjectDef.h"
#include "mbed.h"
#include <vector>



RobotComSlave::RobotComSlave(products given_product_type, unsigned char given_product_id, PinName given_sda_pin, PinName given_scl_pin) : slave(given_sda_pin, given_scl_pin)
{
    product_type = given_product_type;
    product_id = given_product_id;
    i2c_adress = compute_i2c_adress(product_type,product_id);
    slave.frequency(I2CFREQ);
    slave.address(i2c_adress);

}

RobotComSlave::~RobotComSlave()
{
}

void RobotComSlave::receive(){
    char data[NB_OCTET_TRAME];
    char output_data[30];
    int nb_output_bytes;
    int read_error;
    int recive_state = slave.receive();
    switch (recive_state)
    {
        case I2CSlave::ReadAddressed:
            nb_output_bytes = get_all_data_from_output_buffer(output_data);
            send(output_data, nb_output_bytes);
        break;
            
        case I2CSlave::WriteGeneral:
            read_error = slave.read(data,NB_OCTET_TRAME);
            if (!read_error)
            {
                for(char i=0;i<NB_OCTET_TRAME;i++){
                    buffer.push_back(data[i]); 
                    // To much de mettre ca dans un buffer mais c'est pour etre plus flexible pour la suite
                }
            }
            process_data_reception();
        break;
            
        case I2CSlave::WriteAddressed:
            read_error = slave.read(data,NB_OCTET_TRAME);
            if (!read_error)
            {
                for(char i=0;i<NB_OCTET_TRAME;i++){
                    buffer.push_back(data[i]); 
                    // To much de mettre ca dans un buffer mais c'est pour etre plus flexible pour la suite
                }
            }
            process_data_reception();
            
        break;
            
        case I2CSlave::NoData:
        break;
    
    
    default:
        break;
    }
}

char RobotComSlave::process_data_reception(){

    if(buffer.size() >= NB_OCTET_TRAME){
        char data[NB_OCTET_TRAME];
        retrive_complete_tram_from_buffer(data);
        decode(data);
        return 1;
    }else{
        return 0;
    }
}

void RobotComSlave::retrive_complete_tram_from_buffer(char *data){
    for (unsigned int i = 0; i < NB_OCTET_TRAME; i++)
    {
        data[i] = buffer.front();
        buffer.erase(buffer.begin());
    }
}

int RobotComSlave::get_all_data_from_output_buffer(char *data){
    int buffer_len = output_buffer.size();
    for (unsigned int i = 0; i < buffer_len; i++)
    {
        data[i] = output_buffer.front();
        output_buffer.erase(output_buffer.begin());
    }
    return buffer_len;
}

void RobotComSlave::decode(char *data){
    robot_com_frame data_frame;
    data_frame.command = data[0] >> 4;
    data_frame.object = data[0] & 0x0F;
    data_frame.object_id = data[1] >> 4;
    data_frame.property = data[1] & 0x0F;
    data_frame.data = data[2];
    set_decoded_data(data_frame);
}

void RobotComSlave::send(const char *data, int data_len){
    slave.write(data,data_len);
}

char RobotComSlave::compute_i2c_adress(products system_type, unsigned char system_id){
    return ((unsigned char)system_id+1)<<4 | system_type<<1;
}

char RobotComSlave::is_data_decoded_avaliable(){
    bool answer = decoded_data.size() != 0 ;
    return answer;
}
robot_com_frame RobotComSlave::get_decoded_data(){
    robot_com_frame tmp;
    tmp = decoded_data.front();
    decoded_data.erase(decoded_data.begin());
    return tmp;
}

void RobotComSlave::set_decoded_data(robot_com_frame frame){
    decoded_data.push_back(frame);
}

void RobotComSlave::prepare_to_send(const char *data, int data_len){
    for(char i=0;i<data_len;i++){
        output_buffer.push_back(data[i]); 
    }
}