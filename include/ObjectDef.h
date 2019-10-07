#ifndef DEF_OBJECTDEF
#define DEF_OBJECTDEF



#define NB_OCTET_TRAME 3
#define I2CFREQ 100000


enum commands {
    READ,
    WRITE,
    DONE,
    NOT_IMPLEMENTED,
    REBOOT,
    DESACTIVATE
};


// for reserved objects see https://os.mbed.com/users/oliverb/notebook/i2c-address-list/
enum products {
    BRAIN_MACHINE,
    ASSERV_MACHINE,
    IO_MACHINE
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
    ASSERV_POSITION,
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
    unsigned char data;
};

struct float_concat {
    union 
    {
        char byte[4];
        float value;
    };
    
};

#endif