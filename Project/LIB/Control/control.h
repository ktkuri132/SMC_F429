#ifndef _CONTROL_
#define _CONTROL_
#include <Project.h>
#include <stdint.h>

int8_t Project_LIB_ControlStrat();

void control_near_Init();
int8_t Data_Save_from_Camer_near();




typedef struct Control_near {
    uint8_t CamerData[2];
    uint8_t SaveDataLock;
    uint8_t VerifyDataLock;
    uint8_t SiteLock;
    uint8_t RLControl;
    uint8_t Temp_RLContrl;
    int8_t Rvalue, Lvalue;
    uint8_t old_RLControl;
    
    void (*Control_Init)();
    int8_t (*Data_Save_from_Camer)();
    int8_t (*Data_Get_from_Camer)();
    uint8_t (*Temp_Dire_select)();
    void (*Dire_select)(uint8_t Temp);
    void (*ControlTask)();
} NCtrl;


typedef struct Control_min {
    uint8_t CamerData[2];
    uint8_t SaveDataLock;
    uint8_t VerifyDataLock;
    uint8_t SiteLock;
    uint8_t RLControl;
    int8_t Rvalue, Lvalue;

    void (*Control_Init)();
    void (*Data_Save_from_Camer)();
    void (*Data_Get_from_Camer)();
    void (*Temp_Dire_select)();
    void (*ControlTask)();
} MCtrl;


typedef struct Control_far {
    uint8_t CamerData[2];
    uint8_t SaveDataLock;
    uint8_t VerifyDataLock;
    uint8_t SiteLock;
    uint8_t RLControl;
    int8_t Rvalue, Lvalue;

    void (*Control_Init)();
    void (*Data_Save_from_Camer)();
    void (*Data_Get_from_Camer)();
    void (*Temp_Dire_select)();
    void (*ControlTask)();
} FCtrl;

#endif // !_CONTROL_