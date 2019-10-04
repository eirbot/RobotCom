#include <mbed.h>
#include "ObjectDef.h"
#include "mbed.h"

//#define THEBRAIN

#ifdef THEBRAIN
    #include "RobotCom.h"
RobotCom rcom(BRAIN_MACHINE,0,D14,D15);
#else
    #include "RobotComSlave.h"
RobotComSlave rcom(IO_MACHINE,0,D14,D15);
#endif

Serial pc(USBTX,USBRX);

void write_processing(robot_com_frame data);
void traitement();

int main() {

    #ifdef THEBRAIN
    char data[NB_OCTET_TRAME];
    while(1) {
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

void traitement(){
    robot_com_frame data = rcom.get_decoded_data();
    switch (data.command)
    {
    case READ:
        
        break;
    case WRITE:
        write_processing(data);
        break;
    case ACK:
        
        break;
    case NO_ACK:
        
        break;
    case REBOOT:
        
        break;
    case DESACTIVATE:
        
        break;
    default:
        break;
    }
}

void write_processing(robot_com_frame data){
    DigitalOut led(D13);
    switch (data.object)
    {
    case DEVICE:
        
        break;
    case DIGITAL:

        led = data.data;
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