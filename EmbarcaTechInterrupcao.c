#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define LED_PIN_RED 11

// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 0; // Intensidade do vermelho
uint8_t led_g = 0; // Intensidade do verde
uint8_t led_b = 200; // Intensidade do azul

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}


void set_one_led(uint8_t r, uint8_t g, uint8_t b)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (led_Nove[i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

// Número 0
bool led_Zero[NUM_PIXELS] = {
    0, 1, 1, 1, 0,      
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0,    //ok
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0   
};

// Número 1
bool led_Um[NUM_PIXELS] = {
    0, 1, 1, 1, 0,      
    0, 0, 1, 0, 0, //ok
    0, 0, 1, 0, 0,    
    0, 1, 1, 0, 0,  
    0, 0, 1, 0, 0   
};

// Número 2
bool led_Dois[NUM_PIXELS] = {
    0, 1, 1, 1, 0,      
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0,    
    0, 0, 0, 1, 0,  //ok
    0, 1, 1, 1, 0   
};

// Número 3
bool led_Tres[NUM_PIXELS] = {
    0, 1, 1, 1, 0,      
    0, 0, 0, 1, 0, //ok
    0, 1, 1, 1, 0,    
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0   
};

// Número 4
bool led_Quatro[NUM_PIXELS] = {
    0, 1, 0, 0, 0,      
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,    
    0, 1, 0, 1, 0,      //ok
    0, 1, 0, 1, 0   
};

// Número 5
bool led_Cinco[NUM_PIXELS] = {
    0, 1, 1, 1, 0,      
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,    //ok
    0, 1, 0, 0, 0,  
    0, 1, 1, 1, 0   
};

// Número 6
bool led_Seis[NUM_PIXELS] = {
    0, 1, 1, 1, 0,      
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0,    //ok
    0, 1, 0, 0, 0,  
    0, 1, 1, 1, 0   
};

// Número 7
bool led_Sete[NUM_PIXELS] = {
    0, 0, 1, 0, 0,      
    0, 0, 1, 0, 0,   //ok
    0, 0, 1, 0, 0,    
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0  
};

// Número 8
bool led_Oito[NUM_PIXELS] = {
    0, 1, 1, 1, 0,      
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0,    //ok
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0   
};

// Número 9
bool led_Nove[NUM_PIXELS] = {
    0, 1, 1, 1, 0,      
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,    //ok
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0   
};

void InicializarGPIOs(){
    stdio_init_all();
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_put(LED_PIN_RED, 0);
}

void BlinkLedRed(){
   while(1){
    gpio_put(LED_PIN_RED, 1);
    sleep_ms(100); 
    gpio_put(LED_PIN_RED, 0); 
    sleep_ms(100);
   } 
}

int main(){
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    while (1)
    {
        set_one_led(led_r, led_g, led_b);
        //sleep_ms(tempo);
        //set_one_led(0, 0, 0);
        //sleep_ms(tempo);
    }

    return 0;
}
