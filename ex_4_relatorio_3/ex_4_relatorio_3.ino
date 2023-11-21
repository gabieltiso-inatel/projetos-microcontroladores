#define MOTOR (1 << PB3)
#define LIGA (1 << PB1)
#define DESLIGA (1 << PB2)

unsigned int cont = 0;
unsigned int cont_s = 0;
unsigned short int ligado = 0;
float dc_val = 0.0;

int main() {
    DDRB = MOTOR;
    PORTB &= ~(MOTOR);

    PORTB = DESLIGA;

    // Interrupção botões Liga e Desliga
    PCICR = (1 << PCIE0);
    PCMSK0 = (LIGA + DESLIGA);

    // Timer 8 segundos
    TCCR0A = (1 << WGM01) + (0 << WGM00);
    TCCR0B = (1 << CS02) + (1 << CS00);
    OCR0A = 199;
    TIMSK0 = (1 << OCIE0A);

    // PWM
    TCCR2A = (1 << COM2A1) + (0 << COM2A0) + (1 << WGM21) + (1 << WGM20);
    TCCR2B = (1 << CS22) + (1 << CS20);
    OCR2A = 0;

    sei();

    for(;;) {
    }
}

ISR(TIMER0_COMPA_vect) {
    if(ligado) {
        cont++;
        if(cont >= 78) {
            cont = 0;
            cont_s++;
            dc_val += (0.125 * 255);
            if(dc_val > 255) {
                dc_val = 0;
            }
            OCR2A = (int)dc_val;

            if(cont_s == 9) {
                OCR2A = 0;
                cont_s = 0;
                ligado = 0;
            }
        }
    }
}

ISR(PCINT0_vect) {
    short int liga, desliga;
    liga = PINB & LIGA;
    desliga = PINB & DESLIGA;

    if(desliga == 0) {
        ligado = 0;
        OCR2A = 0;
    }

    if(liga == LIGA) {
        ligado = 1;
    }
}
