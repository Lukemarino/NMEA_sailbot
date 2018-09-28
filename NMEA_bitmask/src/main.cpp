// Program to test the CAN bus using pins 29 and 30 on the mbed connected
// to an MCP2551 CAN transceiver bus IC
//  Note that this program will only continue to transmit while the TX message is <= 8 bytes long

#include "mbed.h"

#define THIS_CAN_ID     1   //Address of this CAN device
#define CONT_CAN_ID     3   //Address of Control CAN device

#define PGN_ONE         0b11110111111111111 //a common PGN

Serial pc(USBTX, USBRX);    //tx, and rx for tera term
DigitalOut led1(LED1);      //heartbeat
DigitalOut led2(LED2);      //CAN read activity
DigitalOut led3(LED3);      //CAN write activity

CAN can(p30,p29);      //CAN interface
Ticker pulse;

void copyCANdataStr(unsigned char* canData, char* str){
    for(int i=0;i<2;i++){
         int j=4;
         str[i] = canData[j];
         j++;
    }

}

void hexSearch(unsigned char* canData){
    bool truth=0;
    char search[3] = {1,5,'d'};
    for(int i=0;i<=2;i++){
        if(canData[i]==search[i]){
            truth=1; }
        else{
            truth=0;}
    if(truth==1){
        pc.printf("%x",canData);}
    }
}


void binary2hex(unsigned char* bin){
    unsigned char *a = bin;
    int num = 0;
    do {
        int b = *a=='1'?1:0;
        num = (num<<1)|b;
        a++;
    } while (*a);
    printf("%X\n", num);}

void alive(void){
    led1 = !led1;
    if(led1)
        pulse.attach(&alive, .2); // the address of the function to be attached (flip) and the interval (2 seconds)
    else
        pulse.attach(&alive, 1.3); // the address of the function to be attached (flip) and the interval (2 seconds)
}

int main() {


    CANMessage msg_read;
    wait(.2);
    char sample[50];


    pulse.attach(&alive, 2.0); // the address of the function to be attached (alive) and the interval (2 seconds)
    can.frequency(250000);
    pc.baud(115200);


    while(1) {

        while(can.read(msg_read)){ //if message is available, read into msg
            //pc.printf("%d\n\r", msg_read.id);
            if((msg_read.id & (256 | 512 | 1024 | 2048)) == (256 | 512 | 1024 | 2048) ){
                    //copyCANdataStr(msg_read.data, datastr);
                    pc.printf("\r\nID: %x Message Received: %x", msg_read.id, msg_read.data);
              }
                    led2 = !led2;
        }//while a CAN message has been read
    }//while(1)
}//main
