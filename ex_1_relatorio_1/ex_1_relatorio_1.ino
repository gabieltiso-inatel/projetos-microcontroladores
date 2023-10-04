#define LIGA (1 << PC0)
#define DESLIGA (1 << PC1)
#define CHAVE (1 << PD3)
#define BAIXO (1 << PD4)
#define ALTO (1 << PD5)
#define BOMBA (1 << PB0)

int main(void) {
    DDRB = BOMBA;
    PORTC = (LIGA + DESLIGA);

    PORTB &= ~(BOMBA);

    while(1) {
        short int chave = PIND & CHAVE;

        if(chave) {
            short int baixo, alto;
            baixo = PIND & BAIXO;
            alto = PIND & ALTO;

            if(baixo && alto) {
                PORTB &= ~(BOMBA);
            }

            if (!alto && !baixo) {
                PORTB |= BOMBA;
            }
        } else {
            short int liga, desliga;
            liga = PINC & LIGA;
            desliga = PINC & DESLIGA;

            if(desliga == 0) {
                PORTB &= ~(BOMBA);
            }

            if(liga == 0) {
                PORTB |= BOMBA;
            }
        }
    }
}
