#define LED (1 << PD6)
#define BOTAO (1 << PD4)

unsigned short int pressionado = 0;

int main() {
    PORTD = BOTAO;
    
    // PWM
    DDRD = LED;
    PORTD &= ~(LED);
    TCCR0A = (1 << COM0A1) + (1 << WGM01) + (1 << WGM00);
    TCCR0B = (1 << CS00);
    OCR0A = 0;

    // Interrupção botão
    PCICR = (1 << PCIE2);
    PCMSK2 = BOTAO;

    sei();

    for(;;) {
        if(pressionado) {
            OCR0A = (int)(0.5 * 255);
        } else {
            OCR0A = 0;
        }
    }
}

ISR(PCINT2_vect) {
    short int botao;
    botao = PIND & BOTAO;

    if(botao == 0) {
        pressionado = 1;
    } else {
        pressionado = 0;
    }
}
