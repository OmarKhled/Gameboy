void sound_init()
{
	CLKPR = (1 << CLKPCE);
	CLKPR = 0b00000011;
	DDRD = 0x00;		// Buttons
	DDRC = (1 << DDC5); // PC5 output
}

void wave(float frequency)
{
    PORTC = 0; // initial output 0

    TCNT2 = 150;

    TCCR2A = 0;                                            // normal mode, int clk
    TCCR2B = ((1 << CS20) | (1 << CS21) | (1 << CS22));    // p=1024, start Timer2

    while ((TIFR2 & (1 << TOV2)) == 0)
    {
        // Timer0
        float period = 1000000 * (1 / frequency) / 2; // us
        unsigned char compare_value = (unsigned char)(period / 64) - 1; // Calculate compare match value for CTC mode

        TCCR0A = (1 << WGM01);                          // Set CTC mode
        OCR0A = compare_value;                         // Set compare match value
        TCCR0B = ((1 << CS00) | (1 << CS01));          // p=64, start Timer0

        while ((TIFR0 & (1 << OCF0A)) == 0)
            ; // Wait for compare match flag OCF0A=1

        TCCR0B = 0;                   // Stop Timer0
        TIFR0 = (1 << OCF0A);         // Clear OCF0A flag
        PORTC = PORTC ^ (1 << PORTC5); // Toggle PORTC5

    } // Wait for Timer2 flag TOV2=1

    TCCR2B = 0;             // Stop Timer2
    TIFR2 = (1 << TOV2);    // Clear TOV2 flag

    PORTC = 0; // Set output 0
}

void tone(unsigned char key)
{
	wave(melody[key]);
}

void lose_sound()
{
	tone(5);
	tone(4);
	tone(3);
}

void win_sound()
{
	tone(3);
	tone(4);
	tone(5);
	tone(4);
	tone(5);
}

void welcome_sound()
{
	tone(1);
	tone(4);
	tone(3);
	tone(5);
}
