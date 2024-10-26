#include <18F452.h>

#device adc=10
#use delay (clock=20000000)
#use rs232(baud=2400,xmit=PIN_C6,rcv=PIN_C7)
unsigned int16 adc_value;
unsigned int32 Volts;


//==============================================================================
// Define which timer to use and minor_cycle for RTOS
//==============================================================================
#use rtos(timer=0, minor_cycle=100ms)
//
// Declare TASK "Live" - called every 200ms
// This task flashes the LED on port RD7
//
#task(rate=200ms, max=1ms)
void Live()
{
   output_toggle(PIN_D7); // Toggle RD7 LED
}
//==============================================================================

//==============================================================================
// Declare TASK "Get_voltage" - called every 10ms
//==============================================================================
#task(rate=2s, max=100ms)
void Get_voltage()
{
   adc_value = read_adc(); // Read A/D value
   Volts = (unsigned int32)adc_value*5000;
   Volts = Volts / 1024; // Voltage in mV
}
//
// Declare TASK "To_RS232" - called every millisecond
//
#task(rate=2s, max=100ms)
void To_RS232()
{
   printf("Measured Voltage = %LumV\n\r",Volts); // send to RS232
}


//==============================================================================
//==============================================================================
//======================== Start of MAIN program ===============================
//==============================================================================
//==============================================================================
void main()
{
   set_tris_d(0); // PORTD all outputs
   output_d(0); // Clear PORTD
   set_tris_a(0xFF); // PORTA all inputs
   setup_adc_ports(ALL_ANALOG); // A/D ports
   setup_adc(ADC_CLOCK_DIV_32); // A/D clock
   set_adc_channel(0); // Select channel 0 (AN0)
   delay_us(10);
   rtos_run(); // Start RTOS
}
