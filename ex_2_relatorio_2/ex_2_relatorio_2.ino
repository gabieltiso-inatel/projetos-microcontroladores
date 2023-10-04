#define LIGA (1 << PC0)
#define DESLIGA (1 << PC1)
#define S1 (1 << PB0)
#define S2 (1 << PB1)
#define AVANCO (1 << PD1)
#define RECUO (1 << PD2)
#define M1 (1 << PD3)

int main(void) {
    DDRD = (AVANCO + RECUO + M1);
    PORTD &= ~(AVANCO + RECUO + M1);

    PORTC = (LIGA + DESLIGA);

    PCICR = (1 << PCIE0);
    PCMSK0 = (S1 + S2);

    sei();

    for(;;) {
        short int liga, desliga;
        liga = PINC & LIGA;
        desliga = PINC & DESLIGA;

        if(desliga == 0) {
            PORTD &= ~(AVANCO + RECUO + M1);
        }

        if(liga == 0) {
            PORTD |= M1;
        }
    }
}

ISR(PCINT0_vect) {
    short int sensor1, sensor2;
    sensor1 = PINB & S1;
    sensor2 = PINB & S2;

    if(sensor1 == S1 && sensor2 == 0 && (PIND & RECUO) == 0) {
        PORTD |= AVANCO;
    }

    if(sensor1 == S1 && sensor2 == S2) {
        PORTD &= ~(AVANCO + M1);
        PORTD |= RECUO;
    }

    if(sensor1 == 0 && (PIND & RECUO) == RECUO) {
        PORTD &= ~(RECUO + AVANCO); 
    }
}

