#define ESTEIRA_DIREITA (1 << PD7)
#define ESTEIRA_ESQUERDA (1 << PD6)
#define LIGA (1 << PC0)
#define DESLIGA (1 << PC1)
#define SENSOR1 (1 << PB0)
#define SENSOR2 (1 << PB1)

short int sensor_1_ativado = 0;
short int sensor_2_ativado = 0;

int cont = 0;

int main() {
    DDRD = (ESTEIRA_DIREITA + ESTEIRA_ESQUERDA);
    PORTD &= ~(ESTEIRA_DIREITA + ESTEIRA_ESQUERDA);

    PCICR = (1 << PCIE0) + (1 << PCIE1);
    PCMSK0 = (SENSOR1 + SENSOR2);
    PCMSK1 = (LIGA + DESLIGA);

    PORTC = DESLIGA;

    // Timers
    TCCR0A = (1 << WGM01) + (0 << WGM00);
    TCCR0B = (1 << CS01) + (1 << CS00); // Pre-Scaler 64
    OCR0A = 199;
    TIMSK0 = (1 << OCIE0A);

    sei();

    for(;;) {
    }
}

ISR(TIMER0_COMPA_vect) {
    if(sensor_1_ativado) {
        PORTD &= ~(ESTEIRA_DIREITA);
        cont++;
        if(cont == 6875) {
            cont = 0;
            PORTD |= ESTEIRA_ESQUERDA;
            sensor_1_ativado = 0;
        }
    }

    if(sensor_2_ativado) {
        PORTD &= ~(ESTEIRA_ESQUERDA);
        cont++;
        if(cont == 9375) {
            cont = 0;
            PORTD |= ESTEIRA_DIREITA;
            sensor_2_ativado = 0;
        }
    }
}

ISR(PCINT0_vect) {
    short int s1, s2;
    s1 = PINB & SENSOR1;
    s2 = PINB & SENSOR2;

    if(s1 == SENSOR1) {
        sensor_1_ativado = 1;
    }

    if(s2 == SENSOR2) {
        sensor_2_ativado = 1;
    }
}

ISR(PCINT1_vect) {
    short int liga, desliga;
    liga = PINC & LIGA;
    desliga = PINC & DESLIGA;

    if(desliga == 0) {
        PORTD &= ~(ESTEIRA_DIREITA + ESTEIRA_ESQUERDA);
    }

    if(liga == LIGA) {
        PORTD |= ESTEIRA_DIREITA;
    }
}
