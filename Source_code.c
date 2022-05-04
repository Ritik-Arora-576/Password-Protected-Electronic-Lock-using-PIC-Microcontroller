/* Main.c file generated by New Project wizard
 *
 * Created:   Sat Apr 17 2022
 * Processor: PIC18F452
 * Compiler:  MPLAB XC8
 */
#define _XTAL_FREQ 1000000
#pragma config OSC = RCIO // Oscillator Selection bits (RC oscillator w/ OSC2 configured as RA6)
#pragma config OSCS = OFF // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L
#pragma config PWRT = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON   // Brown-out Reset Enable bit (Brown-out Reset enabled)
#pragma config BORV = 20  // Brown-out Reset Voltage bits (VBOR set to 2.0V)

// CONFIG2H
#pragma config WDT = OFF   // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128 // Watchdog Timer Postscale Select bits (1:128)

// CONFIG3H
#pragma config CCP2MUX = ON // CCP2 Mux bit (CCP2 input/output is multiplexed with RC1)

// CONFIG4L
#pragma config STVR = ON // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will cause RESET)
#pragma config LVP = ON  // Low Voltage ICSP Enable bit (Low Voltage ICSP enabled)

// CONFIG5L
#pragma config CP0 = OFF // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF // Code Protection bit (Block 1 (002000-003FFFh) not code protected)

// CONFIG5H
#pragma config CPB = OFF // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF // Data EEPROM Code Protection bit (Data EEPROM not code protected)

// CONFIG6L
#pragma config WRT0 = OFF // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF // Write Protection bit (Block 1 (002000-003FFFh) not write protected)

// CONFIG6H
#pragma config WRTC = OFF // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF // Data EEPROM Write Protection bit (Data EEPROM not write protected)

// CONFIG7L
#pragma config EBTR0 = OFF // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define RS RD6
#define EN RD7
#define D0 RB0
#define D1 RB1
#define D2 RB2
#define D3 RB3
#define D4 RB4
#define D5 RB5
#define D6 RB6
#define D7 RB7

int a = 10, i = 0;
char code[4] = "XXXX";
char pass[4] = "$$$$";
int wa = 0;

// Sets the pins (D4, D5, D6, D7) based on the given character (binary) input
void Lcd_SetBit(char data_bit) // Based on the Hex value Set the Bits of the Data Lines
{
    if (data_bit & 1)
        D4 = 1;
    else
        D4 = 0;

    if (data_bit & 2)
        D5 = 1;
    else
        D5 = 0;

    if (data_bit & 4)
        D6 = 1;
    else
        D6 = 0;

    if (data_bit & 8)
        D7 = 1;
    else
        D7 = 0;
}

void Lcd_Cmd(char a)
{
    RS = 0;
    Lcd_SetBit(a); // Incoming Hex value
    EN = 1;
    __delay_ms(4);
    EN = 0;
}

// Clears the LCD screen by setting all the pins (D4, D5, D6, D7)
Lcd_Clear()
{
    Lcd_Cmd(0); // Clear the LCD. All pins set as low
    Lcd_Cmd(1); // Move the cursor to first position
}

// Sets the cursor to anywhere on the 2x16 LCD screen
void Lcd_Set_Cursor(char a, char b)
{
    char temp, z, y;
    if (a == 1)
    {
        temp = 0x80 + b - 1; // 80H is used to move the curser
        z = temp >> 4;       // Lower 8-bits
        y = temp & 0x0F;     // Upper 8-bits
        Lcd_Cmd(z);          // Set Row
        Lcd_Cmd(y);          // Set Column
    }
    else if (a == 2)
    {
        temp = 0xC0 + b - 1;
        z = temp >> 4;   // Lower 8-bits
        y = temp & 0x0F; // Upper 8-bits
        Lcd_Cmd(z);      // Set Row
        Lcd_Cmd(y);      // Set Column
    }
}

void Lcd_Start()
{
    Lcd_SetBit(0x00); // D4, D5, D6, D7 are set as low
    for (int i = 1065244; i <= 0; i--)
        NOP();     // delay
    Lcd_Cmd(0x03); // D4, D5 high
    __delay_ms(5);
    Lcd_Cmd(0x03); // D4, D5 high
    __delay_ms(11);
    Lcd_Cmd(0x03); // D4, D5 high
    Lcd_Cmd(0x02); // D5 high
    Lcd_Cmd(0x02); // D5 high
    Lcd_Cmd(0x08); // D7 high
    Lcd_Cmd(0x00); // All low
    Lcd_Cmd(0x0C); // D6, D7 high (1 1 0 0)
    Lcd_Cmd(0x00); // All low
    Lcd_Cmd(0x06); // D5, D6 high (0 1 1 0)
}

// Prints a character to the LCD
void Lcd_Print_Char(char data)
{
    char Lower_Nibble, Upper_Nibble;
    Lower_Nibble = data & 0x0F;
    Upper_Nibble = data & 0xF0;
    RS = 1; // => RS = 1
    Lcd_SetBit(Upper_Nibble >> 4);
    EN = 1;
    for (int i = 2130483; i <= 0; i--)
        NOP();
    EN = 0;
    Lcd_SetBit(Lower_Nibble);
    EN = 1;
    for (int i = 2130483; i <= 0; i--)
        NOP();
    EN = 0;
}

// Prints a string to the LCD
void Lcd_Print_String(char *a)
{
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Print_Char(a[i]);
}

// Repeatedly asks the user to enter a character on the keypad and returns the value of the key pressed
char get_code()
{

    a = 10;
    while (a == 10)
    {

        // First Column
        RC7 = 1;
        RC6 = 0;
        RC5 = 0;
        if (RD0 == 1 && RD1 == 0 && RD2 == 0 && RD3 == 0)
        {
            return '1';
        }
        if (RD0 == 0 && RD1 == 1 && RD2 == 0 && RD3 == 0)
        {
            return '4';
        }
        if (RD0 == 0 && RD1 == 0 && RD2 == 1 && RD3 == 0)
        {
            return '7';
        }
        if (RD0 == 0 && RD1 == 0 && RD2 == 0 && RD3 == 1)
        {
            return '*';
        }
        __delay_ms(100);

        // Second Column
        RC7 = 0;
        RC6 = 1;
        RC5 = 0;
        if (RD0 == 1 && RD1 == 0 && RD2 == 0 && RD3 == 0)
        {
            return '2';
        }
        if (RD0 == 0 && RD1 == 1 && RD2 == 0 && RD3 == 0)
        {
            return '5';
        }
        if (RD0 == 0 && RD1 == 0 && RD2 == 1 && RD3 == 0)
        {
            return '8';
        }
        if (RD0 == 0 && RD1 == 0 && RD2 == 0 && RD3 == 1)
        {
            return '0';
        }
        __delay_ms(100);

        // Third Column
        RC7 = 0;
        RC6 = 0;
        RC5 = 1;
        if (RD0 == 1 && RD1 == 0 && RD2 == 0 && RD3 == 0)
        {
            return '3';
        }
        if (RD0 == 0 && RD1 == 1 && RD2 == 0 && RD3 == 0)
        {
            return '6';
        }
        if (RD0 == 0 && RD1 == 0 && RD2 == 1 && RD3 == 0)
        {
            return '9';
        }
        if (RD0 == 0 && RD1 == 0 && RD2 == 0 && RD3 == 1)
        {
            return '#';
        }
    }
}

// Get user password and display it on the LCD
void get_input()
{
    for (i = 0; i < 4; i++)
    {
        code[i] = get_code();
        Lcd_Set_Cursor(2, i + 1);
        Lcd_Print_Char(code[i]);
        __delay_ms(500);
    }
}

unsigned char EEPROM_Read(unsigned char address)
{
    EEADR = address;      // Address to be read
    EECON1bits.EEPGD = 0; // Selecting EEPROM Data Memory
    EECON1bits.RD = 1;    // Initialize read cycle
    return EEDATA;        // Returning data
}

void EEPROM_Write(unsigned char address, unsigned char data)
{
    unsigned char INTCON_SAVE; // To save INTCON register value
    EEADR = address;           // Address to write
    EEDATA = data;             // Data to write
    EECON1bits.EEPGD = 0;      // Selecting EEPROM Data Memory
    EECON1bits.WREN = 1;       // Enable writing of EEPROM
    INTCON_SAVE = INTCON;      // Backup INCON interrupt register
    INTCON = 0;                // Disables the interrupt
    EECON2 = 0x55;             // Required sequence for write to internal EEPROM
    EECON2 = 0xAA;             // Required sequence for write to internal EEPROM
    EECON1bits.WR = 1;         // Initialize write cycle
    INTCON = INTCON_SAVE;      // Enables Interrupt
    EECON1bits.WREN = 0;       // To disable write
    while (PIR2bits.EEIF == 0)
        ;              // Checking for completion of write operation
    PIR2bits.EEIF = 0; // Clearing EEIF bit
}

// Reads the "CORRECT" password from ROM and store it in the character array 'pass'
void get_password()
{

    __delay_ms(20);
    pass[0] = EEPROM_Read(0x00); // Read data from address 0
    __delay_ms(20);
    pass[1] = EEPROM_Read(0x01); // Read data from address 2
    __delay_ms(20);
    pass[2] = EEPROM_Read(0x02); // Read data from address 4
    __delay_ms(20);
    pass[3] = EEPROM_Read(0x03); // Read data from address 8
    __delay_ms(20);
}

// Sets the character array 'pass' if not set
void set_default_password()
{

    get_password();
    if (pass[0] == 0xFF && pass[1] == 0xFF && pass[2] == 0xFF && pass[3] == 0xFF)
    {
        EEPROM_Write(0x00, '1'); // Write some data at address 00
        __delay_ms(20);
        EEPROM_Write(0x01, '4'); // Write some data at address 02
        __delay_ms(20);
        EEPROM_Write(0x02, '7'); // Write some data at address 04
        __delay_ms(20);
        EEPROM_Write(0x03, '*'); // Write some data at address 08
    }
}

// Writes the new password in memory
void save_new_password()
{
    EEPROM_Write(0x00, code[0]); // Write some data at address 00
    __delay_ms(20);
    EEPROM_Write(0x01, code[1]); // Write some data at address 02
    __delay_ms(20);
    EEPROM_Write(0x02, code[2]); // Write some data at address 04
    __delay_ms(20);
    EEPROM_Write(0x03, code[3]); // Write some data at address 08
}

// Verifies old password and updates the existing password
void change_password()
{
    Lcd_Clear();
    Lcd_Set_Cursor(1, 1);
    Lcd_Print_String("Current Password");
    get_input();
    __delay_ms(100);

    // If the existing password matches the user input, the new code can be saved into the ROM which will become the new password
    if (code[0] == pass[0] && code[1] == pass[1] && code[2] == pass[2] && code[3] == pass[3])
    {
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_Print_String("Enter New Code");
        get_input();

        save_new_password();
        __delay_ms(500);

        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_Print_String("Password Changed");
        __delay_ms(1000);
    }
    else
    {
        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_Print_String("Invalid");

        // Buzzer ON (First time)
        RC4 = 1;
        __delay_ms(300);
        // Buzzer OFF
        RC4 = 0;
        __delay_ms(300);
        // Buzzer ON (Second time)
        RC4 = 1;
        __delay_ms(300);
        // Buzzer OFF
        RC4 = 0;
        __delay_ms(1000);
    }
}

void main(void)
{
    int i;
    // TRIS is used to control direction (Input or Output) of each IO pin
    TRISD = 0x0F;
    TRISB = 0x00;
    TRISC = 0x01;

    set_default_password();

    Lcd_Start();
    Lcd_Clear();
    Lcd_Set_Cursor(2, 3);
    Lcd_Print_String("Welcome...");
    __delay_ms(1000);

    while (1)
    {
        a = 10;

        // Relay and Red LED OFF
        RC2 = 0;
        RC3 = 0;

        get_password();

        Lcd_Clear();
        Lcd_Set_Cursor(2, 5);
        Lcd_Print_String("Press #");
        char f = get_code();
        while (f != '#')
        {
            f = get_code();
        }
        __delay_ms(500);

        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_Print_String("Enter Password");
        get_input();

        if (code[0] == pass[0] && code[1] == pass[1] && code[2] == pass[2] && code[3] == pass[3])
        {
            // count of wrong attempts
            wa = 0;

            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_Print_String("Access Granted");
            Lcd_Set_Cursor(2, 1);
            Lcd_Print_String("Door Unlocked");

            // Set RC4 pin as high to play the buzzer
            RC4 = 1;
            __delay_ms(300);
            // Stop buzzer after 300 ms
            RC4 = 0;

            // Light up the green LED
            RC2 = 1;
            // Keep the red LED OFF
            RC3 = 0;
            __delay_ms(2500);
            // Switch off green LED
            RC2 = 0;

            // Wait till we don't manually lock the door
            while (RC0 == 0)
                ;
            // If lock button was pushed, lock the door
            RC1 = 1;
            // Motor runs for 2.5 sec and locks the door
            __delay_ms(2500);
            RC1 = 0;
        }

        else if (code[0] == '0' && code[1] == '0' && code[2] == '0' && code[3] == '0')
        {
            wa = 0;

            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_Print_String("Change Password?");
            Lcd_Set_Cursor(2, 1);
            Lcd_Print_String("Press * ");
            char choice = get_code();
            if (choice != '*')
            {
                continue;
            }

            // Red LED is on till the new password is not entered
            RC3 = 1;
            change_password();
            RC3 = 0;
            continue;
        }

        else
        {
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_Print_String("Access Denied");

            // Ensure that door unlocking is disabled
            RC2 = 0;
            // RED LED ON
            RC3 = 1;
            // Buzzer ON (First Time)
            RC4 = 1;
            __delay_ms(300);
            // Buzzer OFF
            RC4 = 0;
            __delay_ms(300);
            // Buzzer ON (Second Time)
            RC4 = 1;
            __delay_ms(300);
            // Buzzer OFF
            RC4 = 0;

            // Increase count of wrong attempts
            wa++;
        }

        // If incorrect combination is entered more than twice, the buzzer will switch ON and remain ON till user pushes the lock button
        if (wa > 2)
        {
            // Buzzer ON
            RC4 = 1;
            while (RC0 == 0)
                ;
            wa = 0;
        }

        // Buzzer OFF
        RC4 = 0;
        __delay_ms(100);
    }
}