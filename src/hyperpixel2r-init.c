
#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#define CLK 27
#define MOSI 26
#define CS 18
#define DELAY 100 // clock pulse time in microseconds
#define WAIT 120000 // wait time in microseconds

struct gpiod_chip *chip;

struct gpiod_line *line_clk;
struct gpiod_line *line_mosi;
struct gpiod_line *line_cs;

const char *consumer = "HyperPixel Init";


int setup_pins(void)
{
    chip = gpiod_chip_open("/dev/gpiochip0");

    line_clk = gpiod_chip_get_line(chip, CLK);
    if(gpiod_line_request_output(line_clk, consumer, 0)) {
	printf("Unable to request output: BCM %d", CLK);
        return -1;
    }

    line_mosi = gpiod_chip_get_line(chip, MOSI);
    if(gpiod_line_request_output(line_mosi, consumer, 0)) {
	printf("Unable to request output: BCM %d", MOSI);
        return -1;
    }

    line_cs = gpiod_chip_get_line(chip, CS);
    if(gpiod_line_request_output(line_cs, consumer, 1)) {
	printf("Unable to request output: BCM %d", CS);
        return -1;
    }
}


void cleanup_pins(void)
{
    // Return the touch interrupt pin to a usable state
    gpiod_line_release(line_clk);
    gpiod_line_release(line_mosi);
    gpiod_line_release(line_cs);
    gpiod_chip_close(chip);
}

void send_bits(uint16_t data, uint16_t count){
    int x;
    int mask = 1 << (count-1);
    for(x = 0; x < count; x++){
        gpiod_line_set_value(line_mosi, (data & mask) > 0);
        data <<= 1;

	    gpiod_line_set_value(line_clk, 0);
        usleep(DELAY);
	    gpiod_line_set_value(line_clk, 1);
        usleep(DELAY);
    }
    gpiod_line_set_value(line_mosi, 0);
}

void write_9bit(uint16_t command){
    gpiod_line_set_value(line_cs, 0);
    send_bits(command, 9);
    gpiod_line_set_value(line_cs, 1);
}

void setup_lcd(void){

    write_9bit(0x01);  // Niko added reset

    usleep(240);

    write_9bit(0xFF);
    write_9bit(0x100 | 0x77);
    write_9bit(0x100 | 0x01);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x10);

    write_9bit(0xC0);
    write_9bit(0x100 | 0x3B); // Scan line
    write_9bit(0x100 | 0x00);

    write_9bit(0xC1);
    write_9bit(0x100 | 0x0B); // VBP
    write_9bit(0x100 | 0x02);

    write_9bit(0xC2);
    write_9bit(0x100 | 0x00);  // 07 OR 00
    write_9bit(0x100 | 0x02);

    write_9bit(0xCC);
    write_9bit(0x100 | 0x10);

    // Gamma option B:
    write_9bit(0xB0);  // Positive Voltage Gamma Control
    write_9bit(0x100 | 0x02);
    write_9bit(0x100 | 0x13);
    write_9bit(0x100 | 0x1B);
    write_9bit(0x100 | 0x0D);
    write_9bit(0x100 | 0x10);
    write_9bit(0x100 | 0x05);
    write_9bit(0x100 | 0x08);
    write_9bit(0x100 | 0x07);
    write_9bit(0x100 | 0x07);
    write_9bit(0x100 | 0x24);
    write_9bit(0x100 | 0x04);
    write_9bit(0x100 | 0x11);
    write_9bit(0x100 | 0x0E);
    write_9bit(0x100 | 0x2C);
    write_9bit(0x100 | 0x33);
    write_9bit(0x100 | 0x1D);

    write_9bit(0xB1);  // Negative Voltage Gamma Control
    write_9bit(0x100 | 0x05);
    write_9bit(0x100 | 0x13);
    write_9bit(0x100 | 0x1B);
    write_9bit(0x100 | 0x0D);
    write_9bit(0x100 | 0x11);
    write_9bit(0x100 | 0x05);
    write_9bit(0x100 | 0x08);
    write_9bit(0x100 | 0x07);
    write_9bit(0x100 | 0x07);
    write_9bit(0x100 | 0x24);
    write_9bit(0x100 | 0x04);
    write_9bit(0x100 | 0x11);
    write_9bit(0x100 | 0x0E);
    write_9bit(0x100 | 0x2C);
    write_9bit(0x100 | 0x33);
    write_9bit(0x100 | 0x1D);

    write_9bit(0xFF);
    write_9bit(0x100 | 0x77);
    write_9bit(0x100 | 0x01);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x11);

    write_9bit(0xB0);  // VOP  3.5375+ *x 0.0125
    write_9bit(0x100 | 0x5D);  // 6D OR 5D

    write_9bit(0xB1);  // VCOM amplitude setting
    write_9bit(0x100 | 0x43);  // 37 OR 43

    write_9bit(0xB2);  // VGH Voltage setting
    write_9bit(0x100 | 0x81);  // 12V

    write_9bit(0xB3);
    write_9bit(0x100 | 0x80);

    write_9bit(0xB5);  // VGL Voltage setting
    write_9bit(0x100 | 0x43);  // -8.3V

    write_9bit(0xB7);
    write_9bit(0x100 | 0x85);

    write_9bit(0xB8);
    write_9bit(0x100 | 0x20);

    write_9bit(0xC1);
    write_9bit(0x100 | 0x78);

    write_9bit(0xC2);
    write_9bit(0x100 | 0x78);

    write_9bit(0xD0);
    write_9bit(0x100 | 0x88);

    write_9bit(0xE0);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x02);

    write_9bit(0xE1);
    write_9bit(0x100 | 0x03);
    write_9bit(0x100 | 0xA0);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x04);
    write_9bit(0x100 | 0xA0);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x20);
    write_9bit(0x100 | 0x20);

    write_9bit(0xE2);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);

    write_9bit(0xE3);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x11);
    write_9bit(0x100 | 0x00);

    write_9bit(0xE4);
    write_9bit(0x100 | 0x22);
    write_9bit(0x100 | 0x00);

    write_9bit(0xE5);
    write_9bit(0x100 | 0x05);
    write_9bit(0x100 | 0xEC);
    write_9bit(0x100 | 0xA0);
    write_9bit(0x100 | 0xA0);
    write_9bit(0x100 | 0x07);
    write_9bit(0x100 | 0xEE);
    write_9bit(0x100 | 0xA0);
    write_9bit(0x100 | 0xA0);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);

    write_9bit(0xE6);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x11);
    write_9bit(0x100 | 0x00);

    write_9bit(0xE7);
    write_9bit(0x100 | 0x22);
    write_9bit(0x100 | 0x00);

    write_9bit(0xE8);
    write_9bit(0x100 | 0x06);
    write_9bit(0x100 | 0xED);
    write_9bit(0x100 | 0xA0);
    write_9bit(0x100 | 0xA0);
    write_9bit(0x100 | 0x08);
    write_9bit(0x100 | 0xEF);
    write_9bit(0x100 | 0xA0);
    write_9bit(0x100 | 0xA0);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);

    write_9bit(0xEB);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x40);
    write_9bit(0x100 | 0x40);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);

    write_9bit(0xED);
    write_9bit(0x100 | 0xFF);
    write_9bit(0x100 | 0xFF);
    write_9bit(0x100 | 0xFF);
    write_9bit(0x100 | 0xBA);
    write_9bit(0x100 | 0x0A);
    write_9bit(0x100 | 0xBF);
    write_9bit(0x100 | 0x45);
    write_9bit(0x100 | 0xFF);
    write_9bit(0x100 | 0xFF);
    write_9bit(0x100 | 0x54);
    write_9bit(0x100 | 0xFB);
    write_9bit(0x100 | 0xA0);
    write_9bit(0x100 | 0xAB);
    write_9bit(0x100 | 0xFF);
    write_9bit(0x100 | 0xFF);
    write_9bit(0x100 | 0xFF);

    write_9bit(0xEF);
    write_9bit(0x100 | 0x10);
    write_9bit(0x100 | 0x0D);
    write_9bit(0x100 | 0x04);
    write_9bit(0x100 | 0x08); // Positive Voltage Gamma Control
    write_9bit(0x100 | 0x1F);

    write_9bit(0xFF);
    write_9bit(0x100 | 0x77);
    write_9bit(0x100 | 0x01);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x13);

    write_9bit(0xEF);
    write_9bit(0x100 | 0x08);

    write_9bit(0xFF);
    write_9bit(0x100 | 0x77);
    write_9bit(0x100 | 0x01);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);
    write_9bit(0x100 | 0x00);

    write_9bit(0xCD);  // RGB format COLCTRL
    write_9bit(0x100 | 0x08);
    // write_9bit(0x100 | 0x00)

    write_9bit(0x36); // MadCtl
    write_9bit(0x100 | 0x08);

    write_9bit(0x3A);  // Colmod
    write_9bit(0x100 | 0x66);

    write_9bit(0x11);
    usleep(120);

    write_9bit(0x29);
    usleep(20);

}

int main(int argc, char **argv)
{
    setup_pins();
    setup_lcd();

    cleanup_pins();
    return 0;
}

