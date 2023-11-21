#define NUMERO_AMOSTRAS 50

unsigned long int valor_medio_do_pino = 0;
unsigned long int valor_medio_total = 0;

int main() {
    // Configuração ADC
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0);

    Serial.begin(19200);

    for(;;) {
        Serial.println("Medias dos pinos (0 a 5): ");
        for(int i = 0; i < 6; i++) {
            for(int j = 0; j < NUMERO_AMOSTRAS; j++) {
                ADMUX = (ADMUX & 0xF8) | i;
                ADCSRA |= (1 << ADSC);
                while(!(ADCSRA & (1 << ADIF))); 
                
                valor_medio_do_pino += ADC;
            }

            valor_medio_do_pino /= NUMERO_AMOSTRAS;
            Serial.print(valor_medio_do_pino);
            Serial.print(" ");

            valor_medio_total += valor_medio_do_pino;
            valor_medio_do_pino = 0;
        }

        Serial.println();
        Serial.print("Valor medio global: ");
        Serial.println(valor_medio_total / 6);

        _delay_ms(200);
        valor_medio_total = 0;
    }
}
