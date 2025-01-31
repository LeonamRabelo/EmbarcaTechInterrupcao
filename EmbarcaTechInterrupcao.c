#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define BOTAO_A 5
#define BOTAO_B 6
#define WS2812_PIN 7
#define LED_PIN_RED 13
#define LED_PIN_BLUE 12
#define LED_PIN_GREEN 11

static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
uint numero = 0;//variavel para inicializar o numero com 0

// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 0; // Intensidade do vermelho
uint8_t led_g = 0; // Intensidade do verde
uint8_t led_b = 200; // Intensidade do azul

static inline void put_pixel(uint32_t pixel_grb){
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b){
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

bool led_numeros[10][NUM_PIXELS] = {
    //Número 0
    {
    0, 1, 1, 1, 0,      
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0,   
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0   
    },

    //Número 1
    {0, 1, 1, 1, 0,      
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0,    
    0, 1, 1, 0, 0,  
    0, 0, 1, 0, 0   
    },

    //Número 2
    {0, 1, 1, 1, 0,      
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0,    
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0   
    },

    //Número 3
    {0, 1, 1, 1, 0,      
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,    
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0   
    },

    //Número 4
    {0, 1, 0, 0, 0,      
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,    
    0, 1, 0, 1, 0,     
    0, 1, 0, 1, 0   
    },

    //Número 5
    {0, 1, 1, 1, 0,      
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,   
    0, 1, 0, 0, 0,  
    0, 1, 1, 1, 0   
    },

    //Número 6
    {0, 1, 1, 1, 0,      
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0,    
    0, 1, 0, 0, 0,  
    0, 1, 1, 1, 0   
    },

    //Número 7
    {0, 1, 0, 0, 0,      
    0, 0, 0, 1, 0,   
    0, 1, 0, 0, 0,    
    0, 0, 0, 1, 0,  
    0, 1, 1, 1, 0  
    },

    //Número 8
    {0, 1, 1, 1, 0,      
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0,    
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0   
    },

    //Número 9
    {0, 1, 1, 1, 0,      
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0,    
    0, 1, 0, 1, 0,  
    0, 1, 1, 1, 0   
    }
};

void set_one_led(uint8_t r, uint8_t g, uint8_t b, int numero){
    //Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    //Define todos os LEDs com a cor especificada
    for(int i = 0; i < NUM_PIXELS; i++){
        if(led_numeros[numero][i]){ //Chama a matriz de leds com base no numero passado
            put_pixel(color); //Liga o LED com um no buffer
        }else{
            put_pixel(0);  //Desliga os LEDs com zero no buffer
        }
    }
}

//Função para a inicialização dos GPIOs
void InicializarGPIOs(){
    stdio_init_all();
    //Inicializa pinos dos leds
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_put(LED_PIN_RED, 0);
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
    gpio_put(LED_PIN_BLUE, 0);
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_put(LED_PIN_GREEN, 0);
    //Inicializa pinos dos botoes
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_B); 
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
}

//Fazer o led piscar 5 vezes por segundo (1 segundo = 1000 milisegundos)
void BlinkLedRed(){
//Cada ciclo de 200 milisegundos(ligado e desligado) fazendo um total de 5 ciclos (piscando 5 vezes por segundo)
    gpio_put(LED_PIN_RED, 1);
    sleep_ms(100); 
    gpio_put(LED_PIN_RED, 0); 
    sleep_ms(100); 
}


//Função de interrupção com Debouncing
void gpio_irq_handler(uint gpio, uint32_t events){
uint32_t current_time = to_us_since_boot(get_absolute_time());

    if(current_time - last_time > 200000){ //200 ms de debouncing
    last_time = current_time; //Atualiza o tempo do último evento
    
        //Caso o botão A seja pressionado
        if(gpio == BOTAO_A){
        numero++;   //Incrementa o numero
        if(numero==10){ //Se o numero for maior que 9, ou seja, passou da casa das unidades, ele volta para 0
            numero=0;
        }
        }
        //Caso o botão B seja pressionado
        if(gpio == BOTAO_B){
        numero--;   //Decrementa o numero
        if(numero==-1){ //Se o numero for menor que 0, ou seja, fora do limite dos positivos, ele volta para 9
            numero=9;
        }
        }
    set_one_led(led_r, led_g, led_b, numero);   //Envia o numero para o led
    }
}

int main(){
    //Inicializa o pio
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    InicializarGPIOs(); //Chama a funcao para inicializar os GPIOs

    //Configuração da interrupção com callback para os botoes A e B
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while(1){
    BlinkLedRed();  //Chama a função para fazer o led piscar 5 vezes por segundo
    set_one_led(led_r, led_g, led_b, numero);   //Envia o numero inicializado com 0 para o led
    }

    return 0;
}
