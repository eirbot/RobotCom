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
    char header[NB_OCTET_HEADER];
    char output_data[MAX_DATA_LEN+1];
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
            read_error = slave.read(header,NB_OCTET_HEADER);
            if (!read_error)
            {
                process_data_reception(header);
            }
            
        break;
            
        case I2CSlave::WriteAddressed:
            read_error = slave.read(header,NB_OCTET_HEADER);
            if (!read_error)
            {
                process_data_reception(header);
            }
        break;
            
        case I2CSlave::NoData:
        break;
    
    
    default:
        break;
    }
}

void RobotComSlave::process_data_reception(char *header){
    struct robot_com_frame frame;
    frame = decode_header(header);
    slave.read(frame.data,frame.data_len);
    set_decoded_data(frame);

}


int RobotComSlave::get_all_data_from_output_buffer(char *data){
    size_t buffer_len = output_buffer.size();
    for (size_t i = 0; i < buffer_len; i++)
    {
        data[i] = output_buffer.front();
        output_buffer.erase(output_buffer.begin());
    }
    return buffer_len;
}

robot_com_frame RobotComSlave::decode_header(char *data){
    robot_com_frame data_frame;
    data_frame.command = data[0] >> 4;
    data_frame.object = data[0] & 0x0F;
    data_frame.object_id = data[1] >> 4;
    data_frame.property = data[1] & 0x0F;
    data_frame.data_len = data[2];
    return data_frame;
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
    output_buffer.push_back((char)data_len);
    for(char i=0;i<data_len;i++){
        output_buffer.push_back(data[i]); 
    }
}

void RobotComSlave::prepare_to_send(int data){
    prepare_to_send((const char *)&data,sizeof(data));
}

void RobotComSlave::prepare_to_send(float data){
    prepare_to_send((const char *)&data,sizeof(data));
}