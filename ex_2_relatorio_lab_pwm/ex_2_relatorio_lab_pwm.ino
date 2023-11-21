#define LED (1 << PD6)
#define BOTAO (1 << PD4)

float valor_dc = 0.0;

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
    }
}

ISR(PCINT2_vect) {
    short int botao;
    botao = PIND & BOTAO;

    if(botao == 0) {
        valor_dc += (0.1 * 255);
        if(valor_dc >= 255) {
            valor_dc = 0;
        }
        OCR0A = (int)valor_dc;
    }
}
