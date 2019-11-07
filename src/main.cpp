#include <mbed.h>
#include "ObjectDef.h"
#include "mbed.h"

// #define THEBRAIN

#ifdef THEBRAIN
    #include "RobotCom.h"
RobotCom rcom(BRAIN_MACHINE,0,D14,D15);
#else
    #include "RobotComSlave.h"
RobotComSlave rcom(IO_MACHINE,0,D14,D15);
#endif

Serial pc(USBTX,USBRX);

void write_processing(robot_com_frame recived_data);
void read_processing(robot_com_frame recived_data);
void traitement();

int main() {

    #ifdef THEBRAIN
    char data[NB_OCTET_TRAME];
    char read_data[30];
    while(1) {
        rcom.encode(data,READ,ANALOG,0,ANALOG_VALUE,ANALOG_NONE);
        rcom.send(data,NB_OCTET_TRAME,rcom.compute_i2c_adress(IO_MACHINE,0));
        rcom.read(read_data,rcom.compute_i2c_adress(IO_MACHINE,0));

        pc.printf("Data recue : %d \n",read_data[0]);
        wait_ms(100);
        rcom.encode(data,WRITE,DIGITAL,0,DIGITAL_VALUE,1);
        rcom.send(data,NB_OCTET_TRAME,rcom.compute_i2c_adress(IO_MACHINE,0));
        wait(1);
        rcom.encode(data,WRITE,DIGITAL,0,DIGITAL_VALUE,0);
        rcom.send(data,NB_OCTET_TRAME,rcom.compute_i2c_adress(IO_MACHINE,0));
        wait(1);
    }

    #else
    while(1) {
        rcom.receive();
        if(rcom.is_data_decoded_avaliable()){
            traitement();
        }
    }
    #endif
}

#ifndef THEBRAIN
void traitement(){
    robot_com_frame recived_data = rcom.get_decoded_data();
    switch (recived_data.command)
    {
    case READ:
        read_processing(recived_data);
        break;
    case WRITE:
        write_processing(recived_data);
        break;
    case DONE:
        
        break;
    case NOT_IMPLEMENTED:
        
        break;
    case REBOOT:
        
        break;
    case DESACTIVATE:
        
        break;
    default:
        break;
    }
}

void write_processing(robot_com_frame recived_data){
    DigitalOut led(D13);
    switch (recived_data.object)
    {
    case DEVICE:
        
        break;
    case DIGITAL:

        led = recived_data.data[1];
        break;
    case ANALOG:
        
        break;
    case RF:
    
        break;
    case ASSERV:
    
        break;
    
    default:
        break;
    }
}

void read_processing(robot_com_frame recived_data){
    int data = 246;
    switch (recived_data.object)
    {
    case DEVICE:
        
        break;
    case DIGITAL:

        break;
    case ANALOG:
        rcom.prepare_to_send(data);
        break;
    case RF:
    
        break;
    case ASSERV:
    
        break;
    
    default:
        break;
    }
}

#endif