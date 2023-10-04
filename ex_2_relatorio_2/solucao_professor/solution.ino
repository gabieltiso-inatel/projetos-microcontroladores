#define LIGA (1 << PD2)
#define DESLIGA (1 << PD3)
#define S1 (1 << PB0)
#define S2 (1 << PB1)
#define MOTOR (1 << PC0)
#define AVANCO (1 << PC1)
#define RECUO (1 << PC2)

int main() {

  DDRC = MOTOR + AVANCO + RECUO;
  PORTD = DESLIGA;

  PORTC &= ~(MOTOR + AVANCO + RECUO);

  for (;;) {
    short int liga, desliga, s1, s2;

    liga = PIND & LIGA;
    desliga = PIND & DESLIGA;
    s1 = PINB & S1;
    s2 = PINB & S2;

    if (desliga == 0) {

      PORTC &= ~(MOTOR + AVANCO + RECUO);

    } 
    
    else {

      if (liga == LIGA){
        PORTC |= MOTOR;
      }
      if ((s1 == S1) && !(PORTC & RECUO)){
        PORTC |= AVANCO;
      }
      if (s1 == 0){
        PORTC &= ~(AVANCO + RECUO);
      }
      if (s2 == S2){
        PORTC &= ~(MOTOR + AVANCO);
        PORTC |= RECUO;
      }
    }
  }
}
