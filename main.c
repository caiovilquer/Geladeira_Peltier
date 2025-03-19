/*******************************************************************************
                       Refrigerador com Placa Peltier
  Este projeto se trata de um refrigerador cuja temperatura poderá ser definida
pelo usuário, além de poder selecionar um tempo para ligar. O sistema conta com
um sensor de temperatura que irá enviar um sinal para o MSP430. Caso o sinal não 
esteja na faixa desejada, a peltier é ligada e quando a temperatura está na 
faixa, a peltier desliga.

  Existem dois valores de temperatura que quando digitados exercem funções 
especiais:
  00 -> Liga o modo turbo (liga sem depender da temperatura lida)
  01 -> Possibilita escolher entre °C ou °F

  Além disso, o sistema conta com a possibilidade de serem lidas as medidas 
tanto em °C quanto em °F, leds para iluminação interna e um sensor magnético que
detecta o estado da porta (aberta ou fechada). Caso a porta esteja aberta, a 
peltier desliga e os leds internos acendem.
  
*******************************************************************************/

#include "io430.h"
#include <stdlib.h>
#include <stdio.h>

// Variáveis globais.
unsigned char tecla=0;                                          // Variavel que é atribuido o temp da tecla pressionada
int tensao = 0;                                                 // Valor de tensão do ADC10
int y=0;                                                        // Contador de interrupções
unsigned char segundos, minutos, horas = 0;                     // Variaveis usadas no relogio
char h = 0;                                                     // Variavel que diz se o horario deve ser mostrado (1) ou não (0)           

// Includes de bibliotecas criadas
#include "defines.h"                                            // Biblioteca de defines
#include "Lib_G2553.h"                                          // Biblioteca de funções do kit G2553
#include "geladeira.h"                                          // Biblioteca com as funções de funcionamento da geladeira
#include "relogio.h"                                            // Biblioteca com as funções de funcionamento do relogio

/*******************************************************************************
NOME: main
FUNÇÃO: Função principal do programa
*******************************************************************************/
void main( void )
{
  WDTCTL = WDTPW + WDTHOLD;                                     // Stop watchdog timer to prevent time out reset
  // Variaveis Usadas
  char  anu  [] = {"    Anulado     "};
  char conf  [] = {"Confirma?       "};
  char gel   [] = {"   Geladeira    "};
  char des   [] = {"   Desligada       "};
  char  ini  [] = {" DIGITE A TEMP:  "};
  char erro  [] = {"Temp nao aceita  "};
  char turbo [] = {"MODO TURBO       "};
  char tempe  [] = {"Temperatura:  "};
  char string [15];
  char j = 0;                                                   // Contem o estagio em que a varredura se encontra 
  char t = 0;                                                   // Variavel que contem se a temperatura será em celsius (0) ou em fahrenheit(1)
  char dez = 0;                                                 // Variavel das dezenas do valor de temperatura
  char uni = 0;                                                 // Variavel das unidades do valor de temperatura
  char valor = 0;                                               // Contem o valor de temperatura escolhido pelo usuario
  float temp;                                                   // Variavel que contem a temperatura dentro da geladeira
  
  // Configurações
  Config_LCD();                                                 // Configuração dos I/Os relativos ao display
  Config_Tec();                                                 // Configuração dos I/Os relativos ao teclado
  Init_LCD ();                                                  // Configuração do display
 // config_io ();
  Config_ADC ();                                                // Configuração do ADC
  __enable_interrupt ();                                        // Ativa as interrupções do msp
  envia_c_f;                                                    // Envia inicialização pro display
  configura_relogio ();                                         // Função de configuração do horario atual
  config_timer();                                               // Configura o timer para o relogio, com base de tempo de 1s
  while(1)
  {
    temp_em;                                                    // Envia "temperatura em  celsius (1) ou em fahrenheit(2)"
    while (j == 0)                                              // Primeiro estagio de varredura 
    {
      if (tecla == '2')                                         // Caso tecla = 2, a temperatura será em fahrenheit
      {
        t = 1;
        j++;                                                    // Proximo estagio
        tecla = 0;
      }
      if (tecla == '1')                                         // Caso tecla = 1, a temperatura será em celsius
      {
        t = 0;
        j++;                                                    // Proximo estagio
        tecla = 0;
      }
    }
    h = 1;                                                      // O horario será mostrado no display                                                    
    limpa_1;                                                    // Limpa a linha 1
    envia_ini;                                                  // Envia "DIGITE A TEMP:"
    while (j==1)                                                // Segundo estagio de varredura
    {
      if (tecla && tecla != '*' && tecla != '#')                // Caso tecla for pressionada e for diferente de # e *
      {
        h = 0;                                                  // O horario não será mostrado no display 
        dez = ((tecla-0x30)*10);                                // Dezena da temperatura desejada
        envia_esc;                                              // Envia "Temperatura:"
        __delay_cycles (_50ms);
        envia_dez;                                              // Envia a dezena para o display
        tecla = 0;
        j++;                                                    // Proximo estagio
      }
    }
    while (j==2)                                                // Terceiro estagio de varredura
    {
      if (tecla && tecla != '*' && tecla != '#')                // Caso tecla for pressionada e for diferente de # e *
      {
        uni = (tecla - 0x30);                                   // Unidade da temperatura desejada
        envia_uni;                                              // Envia a unidade para o display
        envia_comando (0x8E);
        if (t == 0)
        {envia_graus_c;}                                        // Caso temperatura esteja em celsius, envia ºC
        if (t == 1)
        {envia_graus_f;}                                        // Caso temperatura esteja em fahrenheit, envia ºF
        envia_conf;                                             // Envia "Confirma?"
        j++;                                                    // Proximo estagio
      } 
      if (tecla == '*')                                         // Caso pressione * a ação é anulada
      {
        envia_anu;
        __delay_cycles (_1s);
        envia_ini;
        j=1;                                                    // Retorna ao segundo estagio
      }      
    }
    while (j==3)                                                // Quarto estagio
    {
      valor = dez + uni;                                        // Valor da temperatura escolhida
      if (valor == 01)                                          // caso valor seja igual a 01, é possivel escolher a unidade de temperatura novamente
      {
        j = 0;                                                  // Retorna ao primeiro estagio
        tecla = 0;
      }
      if (tecla == '#')                                         // Caso tecla = #, a ação é confirmada
      {
        tecla = 0;
        if (celsius)                                            // Caso temperatura seja maior que a escala, envia erro ao display e volta ao segundo estagio (celsius)
        {
          if ((valor > tmax_c) || (valor < tmin_c) && valor !=0 && valor !=01)envia_erro; 
        }
        if (fah)                                                // Caso temperatura seja maior que a escala, envia erro ao display e volta ao segundo estagio (fahrenheit)
        {
          if ((valor > tmax_f) || (valor < tmin_f) && (valor !=0) && (valor !=01))envia_erro; 
        }
        if (valor == 00)                                        // Caso temperatura escolhida seja igual a 00, o modo turbo é ativado
        {
          envia_turbo;
          liga_pel;
          liga_vent;
          inicia_adc;
          while (tecla != '*')                                  // Cancela a ação caso * seja pressionada
          {
            if (celsius)                                        // Envia temperatura em graus celsius                           
            {
              if (y == 4000)
              {
                //envia_graus_c;
                envia_temp_c;
                envia_graus_c;
                envia_string ("                  ");
              }
            }
            if (fah)                                            // Envia temperatura em graus fahrenheit
            {
              if (y == 4000)
              {
                //envia_graus_f;
                envia_temp_f;
                envia_graus_f;
                envia_string ("                  ");
              }
            }
            if (tecla == '1')                                   // A tecla 1 troca as unidade de temperatura
            {
              t ^= BIT0;
              tecla = 0;
            }
            if (sensor_mag == 0)liga_led
            else desliga_led;
          }
          para_adc;
          envia_desliga;                                        // Envia "desligado"
        }    
        if (celsius)                                            // Liga a geladeira caso esteja em celsius
        {
          if ((valor <= tmax_c) && (valor >= tmin_c))           // Só entra na função caso o valor esteja dentro da escala             
          {
            limpa_1;                                            // limpa a primeira linha 
            limpa_2;                                            // limpa a segunda linha 
            envia_deseja                                        // Envia "Deseja escolher um horario?"
            while (tecla != '*' && tecla != '#'){}              // Confirma = #, Não desejo = *
            if (tecla == '#')sis_ligado_horario ();             // Caso aperte #, podera escolher o horario de ligar a geladeira
            limpa_1;                                            // limpa a primeira linha
            limpa_2;                                            // limpa a segunda linha
            tecla = 0;
            h = 0;                                              // Horario não será mais mostrado
            inicia_adc                
            sis_ligado (temp, valor, t);                        // Chama função de ligar a geladeira
            valor =0;
            desliga_pel;
            desliga_vent;
            para_adc;
            envia_desliga;                                      // Envia "Desliga" 
          }
        }
        if (fah)                                                // Liga a geladeira caso esteja em fahrenheit
        {
          if ((valor <= tmax_f) && (valor >= tmin_f))           // Só entra na função caso o valor esteja dentro da escala           
          {
            limpa_1;                                            // limpa a primeira linha 
            limpa_2;                                            // limpa a segunda linha 
            envia_deseja;                                       // Envia "Deseja escolher um horario?"
            while (tecla != '*' && tecla != '#'){}              // Confirma = #, Não desejo = *
            if (tecla == '#')sis_ligado_horario ();             // Caso aperte #, podera escolher o horario de ligar a geladeira
            limpa_1;                                            // limpa a primeira linha 
            limpa_2;                                            // limpa a segunda linha 
            tecla = 0;
            h = 0;                                              // Horario não será mais mostrado
            inicia_adc                
            sis_ligado (temp, valor, t);                        // Chama função de ligar a geladeira
            valor = 0;
            desliga_pel;
            desliga_vent;
            para_adc;
            envia_desliga;                                      // Envia "Desliga" 
          }
        }
        __delay_cycles (_1s);
        envia_ini;                                              // Envia "DIGITE A TEMP:"
        tecla = 0;
        j=1;                                                    // Volta ao primeiro estagio de varredura
      }     
      if (tecla == '*')                                         // Caso aperte * o valor escolhido é anulado
      {
        envia_anu;        
        __delay_cycles (_1s);
        envia_ini;                                              // Envia "DIGITE A TEMP:"
        j=1;                                                    // Volta ao primeiro estagio de varredura
      }
    }
    tecla = 0;
  }
}