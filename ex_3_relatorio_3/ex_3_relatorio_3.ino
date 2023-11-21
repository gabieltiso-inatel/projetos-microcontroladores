#define MOTOR (1 << PD1)
#define LIGA (1 << PB1)
#define DESLIGA (1 << PB2)

unsigned long int cont = 0;
int pressionado = 0;

int main() {
    PORTB = DESLIGA;

    DDRD = MOTOR;
    PORTD &= ~(MOTOR);

    PCICR = (1 << PCIE0);
    PCMSK0 = (LIGA + DESLIGA);

    // Timers
    TCCR0A = (1 << WGM01) + (0 << WGM00);
    TCCR0B = (1 << CS01) + (0 << CS00); // Cada contagem levará 500ns
    OCR0A = 199; // Uma interrupção leva 100 us para acontecer
    TIMSK0 = (1 << OCIE0A);

    sei();

    for(;;) {}
}

ISR(TIMER0_COMPA_vect) {
    if(pressionado) {
        cont++;
        if(cont == 30000) {
            cont = 0;
            PORTD |= MOTOR;
        }
    }
}

ISR(PCINT0_vect) {
    short int liga, desliga;
    liga = PINB & LIGA;
    desliga = PINB & DESLIGA;

    if(!desliga) {
        PORTD &= ~(MOTOR);
    }

    if(liga == LIGA) {
        pressionado = 1;
    } else {
        pressionado = 0;
        cont = 0;
    }
}
