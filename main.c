#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>


void GPIO_PORTF_INIT(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;  //enable clock to portf

    GPIO_PORTF_LOCK_R = 0X4C4F434B;
    GPIO_PORTF_CR_R = 0X1F;

    GPIO_PORTF_DIR_R = 0x0E;    //set correct directions

    GPIO_PORTF_DEN_R = 0x1F;    //enable digital operation at pins
    GPIO_PORTF_PUR_R = 0x11;    //enable pullups for switches



}




void send(char data){

        while((UART0_FR_R & (1<<7)) == 1);
            UART0_DR_R = data;

}



char rece(void){


        while((UART0_FR_R & (1<<4)) == 1);
            return  UART0_DR_R;



}



void UART_n_GPIO_INIT(void)
{

    GPIO_PORTA_AFSEL_R = 0x03;  // UART is enabled on PA1 and PA0
    GPIO_PORTA_DEN_R |= 0x03;  // digital fxn is enabled on PA1 and PA0
    GPIO_PORTA_PCTL_R = 0x11;  // Port mux enabled.


    UART0_CTL_R = 0x00;      // UARTEN is disabled..


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


                 UART0_LCRH_R = 0x62;


                 UART0_CC_R = 0x0;           // system clock source


      UART0_CTL_R = 0x311;      // make 0x301 to remove loopback


}


void main()
{


    SYSCTL_RCGCGPIO_R |= 0x21;  //enable clock to portA and portF
    SYSCTL_RCGCUART_R = 0x01;   // choose  UART0
    UART_n_GPIO_INIT();
    GPIO_PORTF_INIT();


   char des;

  // send('i');
   //send('\n');

    while(1){


       des = rece();
       send(des);

       switch (des)
       {
       case 'r':
           GPIO_PORTF_DATA_R = 0x02;
           break;

       case 'b':
           GPIO_PORTF_DATA_R = 0x04;
           break;

       case 'g':
           GPIO_PORTF_DATA_R = 0x08;
           break;

       default:
           GPIO_PORTF_DATA_R = 0x00;
            break;
       }




    }


}
