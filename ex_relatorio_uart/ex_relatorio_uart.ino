#define FOSC 16000000U // FOSC = frequencia do oscilador = Velocidade de clock do arduino (16MHz)
#define BAUD 9600 // Define qual vai ser o baudrate
#define MYUBRR FOSC / 16 / BAUD - 1
// MYUBRR = UART Baud Rate Register
/*
  FOSC/16: Representa a frequencia específica do clock que vai ser usado na transmissão. Nesse caso, FOSC / 16 = 1000000 Hz (1MHz).
  (FOSC/16)/BAUD: Divide ainda mais a frequência para o clock desejado. Determina quantos ciclos de clock precisa pra transmitir um bit. Nesse caso, (FOSC/16)/BAUD = 104,1667
  (FOSC/16/BAUD)-1: Subtraindo 1 do valor ajusta porque o registro UBRR espera que o valor real seja 1 a menos do valor atual. Nesse caso, (FOSC/16/BAUD)-1 = 103,1667.
*/
#define botao1 (1 << PD7)
#define botao2 (1 << PD4)

#define led_verde (1 << PD6)
#define led_vermelho (1 << PD5)

//Declarando variáveis globais
char msg_tx[20]; // vetor para transmissão
char msg_rx[32]; // vetor do receptor
int pos_msg_rx = 0; // auxiliar para a posição do receptor
int tamanho_msg_rx = 5; // tamanho da mensagem que vai no receptor
unsigned int x = 0, y = 0, valor = 0; // auxiliares

//Prototipos das funcoes
void UART_Init(unsigned int ubrr);
void UART_Transmit(char *dados);

// Função main
int main(void)
{
  UART_Init(MYUBRR);
  PORTD |= botao1 + botao2;
  DDRD = led_verde + led_vermelho;
  PORTD &= ~(led_verde + led_vermelho);

  PCICR = (1 << PCIE2);
  PCMSK2 = botao1 + botao2;

  sei();

  x = 0;
  y = 0;
  
  // Super-loop
  while (1) {
  }
}

ISR(PCINT2_vect) {
  short int b1 = PIND & botao1;
  short int b2 = PIND & botao2;
  
  if (b1 == botao1) {
    UART_Transmit("Vezes que o botao 1 foi pressionado: ");

    x++;
    itoa(x, msg_tx, 10);

    UART_Transmit(msg_tx);
    UART_Transmit("\n");

    _delay_ms(500);
  }

  if (b2 == botao2) {
    UART_Transmit("Vezes que o botao 2 foi pressionado: ");

    y++;

    itoa(y, msg_tx, 10);

    UART_Transmit(msg_tx);
    UART_Transmit("\n");

    _delay_ms(500);
  }

  if(x == 8) {
    x = 0;
    PORTD |= led_verde;
    PORTD &= ~(led_vermelho);  
  }

  if(y == 9) {
    y = 0;
    PORTD |= led_vermelho;
    PORTD &= ~(led_verde);  
  }
}

// Interrupt do USART
ISR(USART_RX_vect)
{
  // Escreve o valor recebido pela UART na posição pos_msg_rx do buffer msg_rx
  msg_rx[pos_msg_rx++] = UDR0;
  if (pos_msg_rx == tamanho_msg_rx)
    pos_msg_rx = 0;
}

// Transmissão UART
void UART_Transmit(char *dados)
{
  // Envia todos os caracteres do buffer dados ate chegar um final de linha
  while (*dados != 0)
  {
    while (!(UCSR0A & (1 << UDRE0))); // Aguarda a transmissão acabar
    // Escreve o caractere no registro de tranmissão
    UDR0 = *dados;
    // Passa para o próximo caractere do buffer dados
    dados++;
  }
}
/**
   while (!(UCSR0A & (1 << UDRE0)));
   Esta linha aguarda até que o registrador de status da UART (UCSR0A) tenha seu bit UDRE0 (USART Data Register Empty)
   definido. Isso indica que o registrador de dados (UDR0) está vazio e pronto para receber o próximo caractere para
   transmissão. Essa espera garante que a transmissão anterior tenha sido concluída antes de enviar o próximo caractere.
*/

// Iniciando o UART
void UART_Init(unsigned int ubrr)
{
  // Configura a baud rate */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  // Habilita a recepcao, tranmissao e interrupcao na recepcao */
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  // Configura o formato da mensagem: 8 bits de dados e 1 bits de stop */
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
/**
   UBRR0H = (unsigned char)(ubrr >> 8);
        Esta linha configura o registrador de alta velocidade (UBRR0H) com a parte superior de 8 bits do valor de ubrr
   UBRR0L = (unsigned char)ubrr;
        Esta linha configura o registrador de baixa velocidade (UBRR0L) com a parte inferior de 8 bits do valor de ubrr
   UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
        RXEN0 - Rx enable, habilita a recepção
        TXEN0 - Tx enable, habilita a transmissão
        RXCIE0 - habilita a interrupção
   UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
        UCSZ01 e UCSZ00 são os bits que especificam o tamanho dos dados transmitidos.
        Nesse caso, a configuração é para 8 bits de dados e 1 bit de stop.
*/
