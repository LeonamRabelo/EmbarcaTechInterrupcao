# EmbarcaTech - Interrupções no microcontrolador RP2040

Este projeto utiliza a **BitDog Lab** com o **Raspberry Pi Pico W** para controlar uma matriz de LEDs WS2812, exibindo números de 0 a 9, utilizando interrupção nos botões, que são usados para incrementar e decrementar o valor exibido. Além disso, um LED vermelho pisca 5 vezes por segundo.

## 📌 Funcionalidades
- Exibição de números de 0 a 9 em uma matriz de LEDs WS2812.
- Controle dos números através de dois botões, com uso de debouncing.
- Efeito de piscar no LED vermelho a 5 Hz.
- Uso de interrupções para garantir resposta rápida aos botões.

## 🚀 Como Funciona
1. **Inicialização:**
   - Os pinos do Raspberry Pi Pico W são configurados.
   - A matriz de LEDs WS2812 é inicializada.
   - Os botões são configurados com **interrupções** para detectar pressionamentos.
   
2. **Lógica dos LEDs:**
   - Uma matriz `led_numeros` armazena a representação dos números de 0 a 9.
   - A função `set_one_led(r, g, b, numero)` exibe o número correspondente na matriz de LEDs.
   
3. **Botões:**
   - O botão A incrementa o número exibido.
   - O botão B decrementa o número exibido.
   - A interrupção com debounce evita leituras erradas.

4. **Piscar o LED vermelho:**
   - O LED vermelho pisca 5 vezes por segundo.

## 📌 Ferramentas para utilizar o código
- SDK da Raspberry Pi Pico.
- Extensão no VSCode da Raspberry Pi Pico.
- Configuração do Wokwi no VSCode (Opcional).
- Compilação e Execução: Para compilar o projeto, utilize o CMake. Certifique-se de que o CMakeLists.txt está configurado corretamente e siga os passos de compilação com o SDK do Raspberry Pi Pico.
  
**Está disponibilizado toda estrutura para simulação no Wokwi para realização de testes, caso não tenha a placa BitDogLab.**

## 📷 Demonstração
Link do vídeo sobre a tarefa: https://www.youtube.com/watch?v=wiyyqXWzKbQ

## 🤝 Contribuições
Sinta-se à vontade para abrir **issues** e enviar **pull requests** para melhorias!
