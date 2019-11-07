#ifndef DEF_OBJECTDEF
#define DEF_OBJECTDEF



#define NB_OCTET_HEADER 3
#define MAX_DATA_LEN 4
#define I2CFREQ 100000

// for reserved objects see https://os.mbed.com/users/oliverb/notebook/i2c-address-list/
enum products {
    BRAIN_MACHINE,
    ASSERV_MACHINE,
    IO_MACHINE
};


enum commands {
    READ,
    WRITE,
    DONE,
    NOT_IMPLEMENTED,
    REBOOT,
    DESACTIVATE
};


enum objects {
    NONE,
    DEVICE,
    DIGITAL,
    ANALOG,
    RF,
    ASSERV
};

enum DEVICE_PROPERTIES {
    DEVICE_NONE,
    DEVICE_NAME,
    DEVICE_ID,
    DEVICE_SYSTEM_TYPE
};

enum DIGITAL_PROPERTIES {
    DIGITAL_NONE,
    DIGITAL_VALUE
};

enum ANALOG_PROPERTIES {
    ANALOG_NONE,
    ANALOG_VALUE
};

enum RF_PROPERTIES {
    RF_NONE,
    RF_DATA,
    RF_SEND
};

enum ASSERV_PROPERTIES {
    ASSERV_NONE,
    ASSERV_POSITION_X,
    ASSERV_POSITION_Y,
    ASSERV_SPEED,
    ASSERV_COEFF_P,
    ASSERV_COEFF_I,
    ASSERV_COEFF_D
};

struct robot_com_frame {
    unsigned char command;
    unsigned char object;
    unsigned char object_id;
    unsigned char property;
    unsigned char data_len;
    char data[MAX_DATA_LEN];
};

struct float_concat {
    union 
    {
        char byte[4];
        float value;
    };
    
};

#endif