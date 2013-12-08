#define F_CPU 16000000UL

#include "QueueList.h"

QueueList<unsigned char> dyn_buffer;


//For async reading
SIGNAL(USART_RX_vect){
   while(!(UCSR0A & (1<<RXC0)));
   dyn_buffer.push(UDR0);
}

//For async writing
SIGNAL(USART_UDRE_vect){
   while(!(UCSR0A & (1<<UDRE0)));
   UDR0 = dyn_buffer.pop();
}

//sincronizzare l'accesso ad una queue gia implementata

int baudrate;
int baud_prescaler;
 
void USART_init(int baud){
 
 baudrate = baud;
 baud_prescaler = ((F_CPU / (baudrate * 16UL))) - 1;

 UBRR0H = (uint8_t)(baud_prescaler >> 8);
 UBRR0L = (uint8_t)(baud_prescaler);
 UCSR0B = (1 << RXEN0)|(1 << TXEN0);
 UCSR0C = (3<<UCSZ00);
}


unsigned char USART_read(){

    return dyn_buffer.pop();
}

void USART_write(char c){

    dyn_buffer.push(c);
}

int USART_available(){
	
    return dyn_buffer.isEmpty();
}



/*
void USART_send( unsigned char data){
 
 while(!(UCSR0A & (1<<UDRE0)));
 UDR0 = data;
 
}
 
void USART_putstring(char* StringPtr){
 
while(*StringPtr != 0x00){
 USART_send(*StringPtr);
 StringPtr++;
}
 
}

unsigned char USART_receive(void){
 
 while(!(UCSR0A & (1<<RXC0)));
 return UDR0;
 
}
*/
