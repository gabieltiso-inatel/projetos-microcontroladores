unsigned int cont_s;
unsigned int cont_m;
unsigned int cont_h;

short int segundos = 0;
short int minutos = 0;
short int horas = 0;

int main() {
    Serial.begin(9600);

    TCCR0A = (1 << WGM01) + (0 << WGM00);    
    TCCR0B = (1 << CS01) + (0 << CS00); // Período de 500ns -> 1 contagem
    OCR0A = 199; // 0 a 199, 200 contagens, interrupção a cada 100us
    TIMSK0 = (1 << OCIE0A);

    sei();

    for(;;) {}
}

ISR(TIMER0_COMPA_vect) {
    cont_s++;

    if(cont_s == 10000) {
        segundos++;
        cont_s = 0;
        cont_m++;

        if(horas < 10) {
            Serial.print("0");
        } 
        Serial.print(horas);
        Serial.print(":");
        if(minutos < 10) {
            Serial.print("0");
        } 
        Serial.print(minutos);
        Serial.print(":");
        if(segundos < 10) {
            Serial.print("0");
        } 
        Serial.print(segundos);
        Serial.println();

        if(cont_m == 59) {
            segundos = 0;
            minutos++;

            cont_m = 0;
            cont_h++;

            if(cont_h == 59) {
                minutos = 0;
                horas++;

                cont_h = 0;
            }
        }
    }

}
