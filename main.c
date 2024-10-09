#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>


void GPIO_INIT()
{

        SYSCTL_RCGCGPIO_R |= 0x01;  //enable clock to portA

       //GPIO_PORTA_LOCK_R = 0X4C4F434B;
       //GPIO_PORTA_CR_R = 0X1F;

       GPIO_PORTA_DIR_R = 0x0E;    //set correct directions

       GPIO_PORTA_DEN_R = 0x1F;    //enable digital operation at pins
       GPIO_PORTA_PUR_R = 0x11;    //enable pullups for switches

}

void GPIO_PORTF_INIT(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;  //enable clock to portf

    GPIO_PORTF_LOCK_R = 0X4C4F434B;
    GPIO_PORTF_CR_R = 0X1F;

    GPIO_PORTF_DIR_R = 0x0E;    //set correct directions

    GPIO_PORTF_DEN_R = 0x1F;    //enable digital operation at pins
    GPIO_PORTF_PUR_R = 0x11;    //enable pullups for switches



}


void UART_INIT(void)
{


    /*
     *
     *  clock Regs..............
     *  UARTCC
     *  UARTCTL....set to zero then set to 1 after configuration
     */

        /* SYSCTL_RCGCUART_R = 0x01;   // choose  UART0 ALREADY CHOOSEN IN MAIN FXN AS ITS CREATES STEP OVER PROBLEM
        UART0_CC_R = 0x0;           // system clock source // check for delayed enable....?
        UART0_CTL_R = 0x00;      // UARTEN is disabled..


    /*
     *  control Regs...................
     *  UARTRSR/ECR
        UARTFR
        UARTLCRH
        UARTCTL
        UARTILPR
        UART9BITADDR
        UART9BITAMASK
        UARTPP

        data Regs....................
        UARTDR

     *
     *
     *
     *
     * */

        /* 9600 = f/16 * divisor ----------- 9600*16/f ===  104.1667
         *
         *
                baudRate Regs..............
                UARTIBRD ===== 104
                UARTFBRD ===== 0.1667 x 64 + 0.5 ==== 11
         *
         *
         */
        UART0_IBRD_R = 104;
        UART0_FBRD_R = 11;
        UART0_LCRH_R = 0xE2;    // odd parity-one stop bit-fifo dis-8bit len-checkparity as 1


        UART0_CTL_R = 0x391;      // UARTEN is enabled, EOT is set ...loopback dis, sys_clk/16, TxRx enabled  //


    /*  Using UART0 for Transmitting operation
     *
     * PA0----->Tx
     * PA1----->Rx
     *
     *
     * */

    GPIO_PORTA_AFSEL_R = 0x03;  // UART is enabled on PA0 and PA1
    GPIO_PORTA_PCTL_R  = 0x03;  // encoding is 1 for UART0, PMC.... setting 1


}

void send(int data){

        while((UART0_FR_R & (1<<5)) != 0);
            UART0_DR_R = data;

}



int rece(void){


        while((UART0_FR_R & (1<<4)) != 0);
            return  UART0_DR_R;






}
void main()
{
    /* 0x0F ---> sw1
     *
     * 0xAA ---->sw2
     * */

    int des,data = 0x00;

    SYSCTL_RCGCGPIO_R |= 0x20;  //enable clock to portf
    SYSCTL_RCGCGPIO_R |= 0x01;  //enable clock to portA
    SYSCTL_RCGCUART_R = 0x01;   // choose  UART0

    GPIO_INIT();
    GPIO_PORTF_INIT();
    UART_INIT();


    while(1){

    /*add code for switch press condiotions : input : can also done using interrupt : try later*/

        if((GPIO_PORTF_DATA_R & (1<<0)) == 0)
        {

            data = 0xAA;    // when sw2 is pressed


        }

        if((GPIO_PORTF_DATA_R & (1<<4)) == 0)
        {

            data = 0xF0;    // when sw1 is pressed


        }


    /*add code for switch press condiotions : input*/

    send(data);     // input
    des = rece();   // output


    /* below part added for conditions after signal is received*/

    if(des == 0xAA)
        GPIO_PORTF_DATA_R = 0x08; // green color

    if(des == 0xF0)
        GPIO_PORTF_DATA_R = 0x04; // blue color

    /*if(des != 0xAA && des != 0xF0 )
        GPIO_PORTF_DATA_R = 0x02; // red color*/


    }

}
