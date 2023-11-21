#define BOTAO (1 << PB4)
#define MOTOR (1 << PD6)

float dc_val = 0.0;

int main() {
    DDRD = MOTOR;

    PCICR = (1 << PCIE0);
    PCMSK0 = BOTAO;

    // Comparador A - PD6
    TCCR0A = (1 << COM0A1) + (0 << COM0A0) + (1 << WGM01) + (1 << WGM00);
    TCCR0B = (1 << CS02) + (1 << CS00);

    OCR0A = 0;

    sei();

    for(;;) {}
}

ISR(PCINT0_vect) {
    short int botao;
    botao = PINB & BOTAO;

    if(botao == BOTAO) {
        dc_val += (0.1 * 256);
    }

    if(dc_val > 255) {
        dc_val = 0;
    }
    
    OCR0A = (int)dc_val;
}
