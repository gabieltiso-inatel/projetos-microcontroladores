#define MOTOR         (1 << PB0)
#define BOTAO1        (1 << PC0)
#define BOTAO2        (1 << PC1)
#define MOTOR_FURACAO (1 << PB3)
#define DESLIGA       (1 << PC2)

#define TEMPO_PADRAO_PARA_PERFURACAO 4000

#define TEMPO_MOVIMENTO_0_1 4000
#define TEMPO_MOVIMENTO_1_2 3000
#define TEMPO_MOVIMENTO_2_3 2000
#define TEMPO_MOVIMENTO_3_4 6000
#define TEMPO_MOVIMENTO_4_5 5000

#define POTENCIA_BERCO_1 0.40
#define POTENCIA_BERCO_2 0.80
#define POTENCIA_BERCO_3 0.35
#define POTENCIA_BERCO_4 0.10
#define POTENCIA_BERCO_5 0.75

unsigned int cont = 0;
unsigned short int movimentando = 0;
unsigned short int perfurando = 0;

unsigned short int tempo_movimento_atual = 0;
float potencia_perfuracao_atual = 0.0;

enum Estado {
    ESPERA,
    MOVIMENTACAO_0_1,
    FURACAO_1,
    MOVIMENTACAO_1_2,
    FURACAO_2,
    MOVIMENTACAO_2_3,
    FURACAO_3,
    MOVIMENTACAO_3_4,
    FURACAO_4,
    MOVIMENTACAO_4_5,
    FURACAO_5,
};

enum Estado estado = ESPERA;

int main() {
    // Saidas
    DDRB = (MOTOR + MOTOR_FURACAO);

    // Interrupções dos botões
    PORTC = DESLIGA;
    PCICR = (1 << PCIE1);
    PCMSK1 = (BOTAO1 + BOTAO2 + DESLIGA);

    // PWM Furação
    TCCR2A = (1 << COM2A1) + (0 << COM2A0) + (1 << WGM21) + (1 << WGM20);
    TCCR2B = (1 << CS22) + (1 << CS20);
    OCR2A = 0;

    // Timer Movimentação
    TCCR0A = (1 << WGM01) + (0 << WGM00);
    TCCR0B = (1 << CS01) + (1 << CS00); // Pre Scaler de 64
    OCR0A = 249;
    TIMSK0 = (1 << OCIE0A);

    sei();

    for(;;) {
    }
}

ISR(TIMER0_COMPA_vect) {
    if(movimentando) {
        PORTB |= MOTOR;
        cont++;
        if(cont == tempo_movimento_atual) {
            PORTB &= ~(MOTOR);
            movimentando = 0;
            cont = 0;

            perfurando = 1;

            switch(estado) {
                case MOVIMENTACAO_0_1:
                    potencia_perfuracao_atual = POTENCIA_BERCO_1;
                    estado = FURACAO_1;
                    break;
                case MOVIMENTACAO_1_2:
                    potencia_perfuracao_atual = POTENCIA_BERCO_2;
                    estado = FURACAO_2;
                    break;
                case MOVIMENTACAO_2_3:
                    potencia_perfuracao_atual = POTENCIA_BERCO_3;
                    estado = FURACAO_3;
                    break;
                case MOVIMENTACAO_3_4:
                    potencia_perfuracao_atual = POTENCIA_BERCO_4;
                    estado = FURACAO_4;
                    break;
                case MOVIMENTACAO_4_5:
                    potencia_perfuracao_atual = POTENCIA_BERCO_5;
                    estado = FURACAO_5;
                    break;
            }
        }
    }

    if(perfurando) {
        OCR2A = (int)(potencia_perfuracao_atual * 255);
        cont++;

        if(cont == TEMPO_PADRAO_PARA_PERFURACAO) {
            OCR2A = 0;
            cont = 0;
            perfurando = 0;
            movimentando = 1;

            switch(estado) {
                case FURACAO_1:
                    tempo_movimento_atual = TEMPO_MOVIMENTO_1_2;
                    estado = MOVIMENTACAO_1_2;
                    break;
                case FURACAO_2:
                    tempo_movimento_atual = TEMPO_MOVIMENTO_2_3;
                    estado = MOVIMENTACAO_2_3;
                    break;
                case FURACAO_3:
                    tempo_movimento_atual = TEMPO_MOVIMENTO_3_4;
                    estado = MOVIMENTACAO_3_4;
                    break;
                case FURACAO_4:
                    tempo_movimento_atual = TEMPO_MOVIMENTO_4_5;
                    estado = MOVIMENTACAO_4_5;
                    break;
                case FURACAO_5:
                    movimentando = 0;
                    perfurando = 0;

                    OCR2A = 0;
                    PORTB &= ~(MOTOR);

                    estado = ESPERA;
                    break;
            }
        }
    }
}

ISR(PCINT1_vect) {
    short int b1, b2, desliga;
    b1 = PINC & BOTAO1;
    b2 = PINC & BOTAO2;
    desliga = PINC & DESLIGA;

    if(desliga == 0) {
        movimentando = 0;
        perfurando = 0;
        OCR2A = 0;
        PORTB &= ~(MOTOR);
        estado = ESPERA;
    }

    if(b1 == BOTAO1 && b2 == BOTAO2 && estado == ESPERA) {
        movimentando = 1;
        estado = MOVIMENTACAO_0_1;
        tempo_movimento_atual = TEMPO_MOVIMENTO_0_1;
    }
}
