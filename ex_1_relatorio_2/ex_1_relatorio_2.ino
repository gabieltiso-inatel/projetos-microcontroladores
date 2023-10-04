#define BUZZER (1 << PB4)
#define B1 (1 << PB2)
#define B2 (1 << PC4)
#define B3 (1 << PD2)
#define B4 (1 << PD6)

int main(void) {
    DDRB |= BUZZER;

    PORTB = B1;
    PORTC = B2;
    PORTD = (B3 + B4);

    PORTB &= ~(BUZZER);

    PCICR = (1 << PCIE2) + (1 << PCIE1) + (1 << PCIE0);
    EICRA = (1 << ISC01) + (0 << ISC00);

    PCMSK0 = B1;
    PCMSK1 = B2;
    PCMSK2 = B4;
    EIMSK = (1 << INT0);

    sei();

    for(;;) {
    }
}

ISR(PCINT0_vect) {
    short int b1;
    b1 = PINB & B1;

    if(!b1) {
        PORTB |= BUZZER;
        _delay_ms(3000);
        PORTB &= ~(BUZZER);
    }
}

ISR(PCINT1_vect) {
    short int b2;
    b2 = PINC & B2;

    if(!b2) {
        PORTB |= BUZZER;
        _delay_ms(3000);
        PORTB &= ~(BUZZER);
    }
}

ISR(PCINT2_vect) {
    short int b4;
    b4 = PIND & B4;

    if(!b4) {
        PORTB |= BUZZER;
        _delay_ms(3000);
        PORTB &= ~(BUZZER);
    }
}

ISR(INT0_vect) {
    short int b3;
    b3 = PIND & B3;

    if(!b3) {
        PORTB |= BUZZER;
        _delay_ms(3000);
        PORTB &= ~(BUZZER);
    }
}
