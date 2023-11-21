unsigned int valor_digital_atual = 0;
unsigned long int valor_digital_medio = 0;

int main() {
    // Configuração ADC
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) + (1 << ADPS2) + (1 << ADPS1) + (1 << ADPS0);

    Serial.begin(19200);

    for(;;) {
        for(int i = 0; i < 6; i++) {
            ADMUX = (ADMUX & 0xF8) | i;
            ADCSRA |= (1 << ADSC);
            while(!(ADCSRA & (1 << ADIF))); 
            
            // valor atual contido no conversor
            valor_digital_atual = ADC;
            valor_digital_medio += valor_digital_atual;
        }

        Serial.print("Valor medio: ");
        Serial.println(valor_digital_medio / 6);

        _delay_ms(200);
        valor_digital_medio = 0;
    }
}
