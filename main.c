/*******************************************************************************
                       Refrigerador com Placa Peltier
  Este projeto se trata de um refrigerador cuja temperatura poder� ser definida
pelo usu�rio, al�m de poder selecionar um tempo para ligar. O sistema conta com
um sensor de temperatura que ir� enviar um sinal para o MSP430. Caso o sinal n�o 
esteja na faixa desejada, a peltier � ligada e quando a temperatura est� na 
faixa, a peltier desliga.

  Existem dois valores de temperatura que quando digitados exercem fun��es 
especiais:
  00 -> Liga o modo turbo (liga sem depender da temperatura lida)
  01 -> Possibilita escolher entre �C ou �F

  Al�m disso, o sistema conta com a possibilidade de serem lidas as medidas 
tanto em �C quanto em �F, leds para ilumina��o interna e um sensor magn�tico que
detecta o estado da porta (aberta ou fechada). Caso a porta esteja aberta, a 
peltier desliga e os leds internos acendem.
  
*******************************************************************************/

#include "io430.h"
#include <stdlib.h>
#include <stdio.h>

// Vari�veis globais.
unsigned char tecla=0;                                          // Variavel que � atribuido o temp da tecla pressionada
int tensao = 0;                                                 // Valor de tens�o do ADC10
int y=0;                                                        // Contador de interrup��es
unsigned char segundos, minutos, horas = 0;                     // Variaveis usadas no relogio
char h = 0;                                                     // Variavel que diz se o horario deve ser mostrado (1) ou n�o (0)           

// Includes de bibliotecas criadas
#include "defines.h"                                            // Biblioteca de defines
#include "Lib_G2553.h"                                          // Biblioteca de fun��es do kit G2553
#include "geladeira.h"                                          // Biblioteca com as fun��es de funcionamento da geladeira
#include "relogio.h"                                            // Biblioteca com as fun��es de funcionamento do relogio

/*******************************************************************************
NOME: main
FUN��O: Fun��o principal do programa
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
  char t = 0;                                                   // Variavel que contem se a temperatura ser� em celsius (0) ou em fahrenheit(1)
  char dez = 0;                                                 // Variavel das dezenas do valor de temperatura
  char uni = 0;                                                 // Variavel das unidades do valor de temperatura
  char valor = 0;                                               // Contem o valor de temperatura escolhido pelo usuario
  float temp;                                                   // Variavel que contem a temperatura dentro da geladeira
  
  // Configura��es
  Config_LCD();                                                 // Configura��o dos I/Os relativos ao display
  Config_Tec();                                                 // Configura��o dos I/Os relativos ao teclado
  Init_LCD ();                                                  // Configura��o do display
 // config_io ();
  Config_ADC ();                                                // Configura��o do ADC
  __enable_interrupt ();                                        // Ativa as interrup��es do msp
  envia_c_f;                                                    // Envia inicializa��o pro display
  configura_relogio ();                                         // Fun��o de configura��o do horario atual
  config_timer();                                               // Configura o timer para o relogio, com base de tempo de 1s
  while(1)
  {
    temp_em;                                                    // Envia "temperatura em  celsius (1) ou em fahrenheit(2)"
    while (j == 0)                                              // Primeiro estagio de varredura 
    {
      if (tecla == '2')                                         // Caso tecla = 2, a temperatura ser� em fahrenheit
      {
        t = 1;
        j++;                                                    // Proximo estagio
        tecla = 0;
      }
      if (tecla == '1')                                         // Caso tecla = 1, a temperatura ser� em celsius
      {
        t = 0;
        j++;                                                    // Proximo estagio
        tecla = 0;
      }
    }
    h = 1;                                                      // O horario ser� mostrado no display                                                    
    limpa_1;                                                    // Limpa a linha 1
    envia_ini;                                                  // Envia "DIGITE A TEMP:"
    while (j==1)                                                // Segundo estagio de varredura
    {
      if (tecla && tecla != '*' && tecla != '#')                // Caso tecla for pressionada e for diferente de # e *
      {
        h = 0;                                                  // O horario n�o ser� mostrado no display 
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
        {envia_graus_c;}                                        // Caso temperatura esteja em celsius, envia �C
        if (t == 1)
        {envia_graus_f;}                                        // Caso temperatura esteja em fahrenheit, envia �F
        envia_conf;                                             // Envia "Confirma?"
        j++;                                                    // Proximo estagio
      } 
      if (tecla == '*')                                         // Caso pressione * a a��o � anulada
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
      if (valor == 01)                                          // caso valor seja igual a 01, � possivel escolher a unidade de temperatura novamente
      {
        j = 0;                                                  // Retorna ao primeiro estagio
        tecla = 0;
      }
      if (tecla == '#')                                         // Caso tecla = #, a a��o � confirmada
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
        if (valor == 00)                                        // Caso temperatura escolhida seja igual a 00, o modo turbo � ativado
        {
          envia_turbo;
          liga_pel;
          liga_vent;
          inicia_adc;
          while (tecla != '*')                                  // Cancela a a��o caso * seja pressionada
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
          if ((valor <= tmax_c) && (valor >= tmin_c))           // S� entra na fun��o caso o valor esteja dentro da escala             
          {
            limpa_1;                                            // limpa a primeira linha 
            limpa_2;                                            // limpa a segunda linha 
            envia_deseja                                        // Envia "Deseja escolher um horario?"
            while (tecla != '*' && tecla != '#'){}              // Confirma = #, N�o desejo = *
            if (tecla == '#')sis_ligado_horario ();             // Caso aperte #, podera escolher o horario de ligar a geladeira
            limpa_1;                                            // limpa a primeira linha
            limpa_2;                                            // limpa a segunda linha
            tecla = 0;
            h = 0;                                              // Horario n�o ser� mais mostrado
            inicia_adc                
            sis_ligado (temp, valor, t);                        // Chama fun��o de ligar a geladeira
            valor =0;
            desliga_pel;
            desliga_vent;
            para_adc;
            envia_desliga;                                      // Envia "Desliga" 
          }
        }
        if (fah)                                                // Liga a geladeira caso esteja em fahrenheit
        {
          if ((valor <= tmax_f) && (valor >= tmin_f))           // S� entra na fun��o caso o valor esteja dentro da escala           
          {
            limpa_1;                                            // limpa a primeira linha 
            limpa_2;                                            // limpa a segunda linha 
            envia_deseja;                                       // Envia "Deseja escolher um horario?"
            while (tecla != '*' && tecla != '#'){}              // Confirma = #, N�o desejo = *
            if (tecla == '#')sis_ligado_horario ();             // Caso aperte #, podera escolher o horario de ligar a geladeira
            limpa_1;                                            // limpa a primeira linha 
            limpa_2;                                            // limpa a segunda linha 
            tecla = 0;
            h = 0;                                              // Horario n�o ser� mais mostrado
            inicia_adc                
            sis_ligado (temp, valor, t);                        // Chama fun��o de ligar a geladeira
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
      if (tecla == '*')                                         // Caso aperte * o valor escolhido � anulado
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