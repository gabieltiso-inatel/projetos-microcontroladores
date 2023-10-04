/* PINAGEM
    
Entradas:
    Sensor - PB3
    Liga - PB0
    Desliga - PB1

Saídas:
    Motor - PD2
    Alarme - PD6
*/

#define LIGA (1 << PB0)
#define DESLIGA (1 << PB1)
#define SENSOR (1 << PB3)
#define MOTOR (1 << PD2)
#define ALARME (1 << PD6)

int main(void) {
    DDRD = (MOTOR + ALARME);
    PORTB = (SENSOR + LIGA + DESLIGA);

    PORTD &= ~(MOTOR + ALARME);

    while(1) {
        short int liga, desliga;
        liga = PINB & LIGA;
        desliga = PINB & DESLIGA;

        short int sensor = PINB & SENSOR;

        if(desliga == 0) {
            PORTD &= ~(MOTOR);
        }

        if(liga == 0 && sensor != 0) {
            PORTD |= MOTOR;
            PORTD &= ~(ALARME);
        }

        if(sensor == 0) {
            PORTD &= ~(MOTOR);
            PORTD |= ALARME;
        }
    }
}
