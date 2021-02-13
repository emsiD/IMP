// Matej Dubec, xdubec00, 85%
#include "MK60D10.h"

#define SPK 0x10 // Speaker is on PTA4

int pressed_up = 0, pressed_down = 0;
int freq = 0x2FAF080;

int n = 0;
unsigned char c;


/* Initialize the MCU - basic clock settings, turning the watchdog off */
void MCUInit(void)  {
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}

/* Initialize ports */
void PortsInit(void)
{
    /* Turn on all port clocks */
	SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK;
	SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;

    /* Set corresponding PTB pins (connected to LED's) for GPIO functionality */
    PORTA->PCR[4] = PORT_PCR_MUX(0x03);  // Speaker 0x03
    PORTE->PCR[8] = PORT_PCR_MUX(0x03);  // UART_TX
    PORTE->PCR[9] = PORT_PCR_MUX(0x03);  // UART_RX

    /* Change corresponding PTB port pins as outputs */
    PTA->PDDR = GPIO_PDDR_PDD(SPK);      // Speaker as output
    PTA->PDOR &= GPIO_PDOR_PDO(~SPK);    // Speaker off, beep_flag is false
	PTA->PDDR |=  GPIO_PDDR_PDD(0x0010); //  UART as output
}

/* Initialize UART */
void UART5Init(void) {
	UART5->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
	UART5->BDH = 0;
	UART5->BDL = 26; // Baud rate  115 200 Bd, 1 stop bit
	UART5->C4 = 15;  // Oversampling ratio 16
	UART5->C1 = 0;   // 8 data bits
	UART5->C3 = 0;
	UART5->MA1 = 0;
	UART5->MA2 = 0;
	UART5->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK); // Turn on transmitter and receiver
}

/* Initialize timer */
void Timer0Init(void)
{
	/* Sets the register to zero */
	FTM0_CNT = 420;

	/* Sets the register to particular OF value */
	FTM0_MOD = 0xFFFF;

	/* Sets the FTM generation mode on canal 1 in control register */
    FTM0_C1SC = 0x28;

    /* Configures the timer in his state and control register */
    FTM0_SC = 0x08;
}

/* Sends one character via UART */
void SendChar(char ch)  {
    while(!(UART5->S1 & UART_S1_TDRE_MASK) && !(UART5->S1 & UART_S1_TC_MASK));
    UART5->D = ch;
}

/* Receive char from UART */
char ReceiveChar(void) {
	while(!(UART5->S1 & UART_S1_RDRF_MASK));
	return UART5->D;
}

/* Sends string via UART */
void SendString(char *s)  {
	int i = 0;
	while (s[i]!=0){
		SendChar(s[i++]);
	}
}

/* Function for delay */
void delay(int bound) {
  int i;
  for(i=0; i<bound; i++);
}

/* Function for playing tones */
void PlayTone(int frequency) {
	FTM0_MOD = freq / frequency;
	FTM0_C1V = FTM0_MOD / 2;
	delay(1000000);
	FTM0_C1V = 0;
}

int main(void)
{
	int increment = 1;
	unsigned int compare = 0;
    MCUInit();
    PortsInit();
	UART5Init();
	Timer0Init();
	SendString(" Pre spustenie pesníčky stlač klávesu p.\r\n");	// Informative strings
	SendString("Pre voľné hranie stlač inú klávesu, ako p.\r\n");

	while (1) {
		c = ReceiveChar();
		if (c == 'p') { // User selected to play preloaded song (kohutik jaraby)
			PlayTone(784);
			delay(25000);
			PlayTone(880);
			delay(25000);
			PlayTone(988);
			delay(1000000);
			PlayTone(1060);
			delay(1000000);
			PlayTone(1060);
			delay(25000);
			PlayTone(1060);
			delay(50000);

			PlayTone(1060);
			delay(25000);
			PlayTone(1060);
			delay(25000);
			PlayTone(880);
			delay(1000000);
			PlayTone(784);
			delay(1000000);
			PlayTone(784);
			delay(25000);
			PlayTone(784);
			delay(50000);

			PlayTone(880);
			delay(25000);
			PlayTone(988);
			delay(25000);
			PlayTone(784);
			delay(1000000);
			PlayTone(1060);
			delay(1000000);
			PlayTone(1060);
			delay(25000);
			PlayTone(1060);
			delay(50000);

			PlayTone(988);
			delay(25000);
			PlayTone(880);
			delay(25000);
			PlayTone(784);
			delay(1000000);
			PlayTone(784);
			delay(1000000);
			PlayTone(784);
			delay(25000);
			PlayTone(784);
			delay(50000);

			PlayTone(784);
			delay(25000);
			PlayTone(880);
			delay(25000);
			PlayTone(988);
			delay(1000000);
			PlayTone(1060);
			delay(1000000);
			PlayTone(1060);
			delay(25000);
			PlayTone(1060);
			delay(50000);

			PlayTone(988);
			delay(25000);
			PlayTone(880);
			delay(25000);
			PlayTone(784);
			delay(1000000);
			PlayTone(784);
			delay(1000000);
			PlayTone(784);
			delay(25000);
			PlayTone(784);
			delay(50000);

		}
		else { // User selected to play piano live
			switch (c) {
				case 'a': // C
					PlayTone(523);
					break;
				case 's': // D
					PlayTone(587);
					break;
				case 'd': // E
					PlayTone(659);
					break;
				case 'f': // F
					PlayTone(698);
					break;
				case 'g': // G
					PlayTone(784);
					break;
				case 'h': // A
					PlayTone(880);
					break;
				case 'j': // B
					PlayTone(988);
					break;
				case 'k': // C
					PlayTone(1060);
					break;
				default:
					SendString("Klavesy vytvarajuce zvuk su: a, s, d, f, g, h, j\r\n");
			}
			SendChar(c);
		}
	}
}
