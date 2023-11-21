#define NUMERO_DE_AMOSTRAS 30

unsigned int valor_digital_medio = 0; 
float tensao = 0;

int main() {
    // Configuração ADC
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0);

    Serial.begin(19200);

    for(;;) {
        for(int i = 0; i < NUMERO_DE_AMOSTRAS; i++) {
            ADMUX |= (ADMUX & 0xF8) | 4;
            ADCSRA |= (1 << ADSC);
            while(!(ADCSRA & (1 << ADIF))); 

            valor_digital_medio += ADC;
        }

        valor_digital_medio = valor_digital_medio / NUMERO_DE_AMOSTRAS;

        Serial.print("ADC: ");
        Serial.println(valor_digital_medio);

        tensao = (valor_digital_medio * 5.0) / 1024;
        Serial.print("Tensao: ");
        Serial.println(tensao);

        valor_digital_medio = 0;
        _delay_ms(200);
    }
}
