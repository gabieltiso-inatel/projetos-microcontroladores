#define LIGA (1 << PB0)
#define DESLIGA (1 << PD3)
#define S1 (1 << PB1)
#define S2 (1 << PB2)
#define S3 (1 << PD2)
#define M1 (1 << PD1)
#define ALARME (1 << PD4)
#define BUZZER (1 << PD5)

#define CH1 (1 << PB4)
#define CH2 (1 << PB5)

void alarme() {
    PORTD &= ~(M1);
    PORTD |= BUZZER;
    for(int i = 0; i < 5; i++) {
        PORTD |= ALARME;
        _delay_ms(100);
        PORTD &= ~(ALARME);
        _delay_ms(100);
    }
}

int main(void) {
   DDRD |= (M1 + ALARME + BUZZER);

   PORTB |= (LIGA + CH1 + CH2);
   PORTD |= DESLIGA;

   EICRA = (1 << ISC11) + (0 << ISC10);
   EIMSK = (1 << INT1);

   PORTD &= ~(M1 + ALARME + BUZZER);
   sei();

   for(;;) {
        short int liga;
        liga = PINB & LIGA;

        if(liga == 0) {
            PORTD &= ~(BUZZER + ALARME);
            PORTD |= M1;
        }

        short int ch1, ch2;
        ch1 = PINB & CH1;
        ch2 = PINB & CH2;

        if(ch1 == CH1 && ch2 == CH2) {
            short int s2, s3;
            s2 = PINB & S2;
            s3 = PIND & S3;

            if(s2 == S2 || s3 == S3) {
                alarme();
            }
        } else if(ch1 == CH1 && ch2 == 0) {
            short int s3;
            s3 = PIND & S3;

            if(s3 == S3) {
                alarme();
            }
        } else if(ch1 == 0 && ch2 == CH2) {
            short int s3;
            s3 = PIND & S3;

            if(s3 == 0) {
                alarme();
            }
        }
   }
}

ISR(INT1_vect) {
    short int desliga;
    desliga = PIND & DESLIGA;

    if(desliga == 0) {
       PORTD &= ~(M1 + ALARME + BUZZER);
    }
}
