// Biblioteca de definições e funções para o kit G2553

#define Fclk   1.2e6           // Freq. de clk do DCO para o G2553
#define _5ms   5e-3 *Fclk
#define _10ms  10e-3*Fclk
#define _15ms  15e-3*Fclk
#define _20ms  20e-3*Fclk
#define _30ms  30e-3*Fclk
#define _2ms   (int)(2e-3*Fclk)
#define _2s    2*Fclk
#define _100us 100e-6*Fclk
#define _500ms 500e-3*Fclk
#define _50ms  50e-3*Fclk
#define _1s    1*Fclk
#define _1_5s  1.5*Fclk
//prototipo
void envia (unsigned char palavra);
int descobrir (unsigned char j);
void envia_dado(unsigned char palavra);
// Dando nomes mais interessantes para os bits das portas P1 e P2
// Porta P1 - conexões com o LCD 
#define LCDPort_out  P1OUT                       // Os bits de dados do LCD estão conectados
#define LCDPort_dir  P1DIR                       // na porta P1
#define LCDPort_sel  P1SEL
#define LCD_D4       BIT4
#define LCD_D5       BIT5
#define LCD_D6       BIT6
#define LCD_D7       BIT7
#define LCDData      (LCD_D4+LCD_D5+LCD_D6+LCD_D7) // Barramento de 4 bits do display

// Porta P1 - conexõeos com o teclado
#define LedVm        BIT0                        // Leds que já vêm montado no kit
#define LedVd        BIT1
#define ChS2         BIT3                        // Micro chave que já vem montada no kit

#define TecLin_out   P1OUT                       // As linhas do teclado estão conectadas na
#define TecLin_dir   P1DIR                       // porta P1
#define TecLin_sel   P1SEL                                                                                
#define TecL1        BIT4                        // Cada uma das linhas do teclado
#define TecL2        BIT5
#define TecL3        BIT6
#define TecL4        BIT7
#define TecLins     (TecL1+TecL2+TecL3+TecL4)

// Porta P2
#define LCDCtl_out   P2OUT                       // Os bit En e RS estão conecetados na 
#define LCDCtl_dir   P2DIR                       // porta P2
#define LCDCtl_sel   P2SEL  
#define LCDCtl_ie    P2IE
#define LCDCtl_ifg   P2IFG
#define LCDEn        BIT1                        // Enable  = Bit de controle do LCD
#define LCDRS        BIT7                        // Este bit é compartilhado com a coluna 1
                                                 // do teclado e o pino RS do LCD.
#define TecCol_in    P2IN                        // As colunas do teclado estão conectadas 
#define TecCol_dir   P2DIR                       // na porta P2  
#define TecCol_sel   P2SEL  
#define TecCol_ie    P2IE
#define TecCol_ies   P2IES
#define TecCol_ifg   P2IFG
#define TecCol_ren   P2REN
#define TecCol_out   P2OUT
#define TecC1        BIT0                        // Cada uma das colunas do teclado
#define TecC2        BIT5
#define TecC3        BIT6
#define TecCols      (TecC1+TecC2+TecC3)


// Primeira palavra de configuração do display LCD para o kit LaunchPad
#define LCD_DL       0                           // BIT4 = DL, 1 = barramento de 8 bits, 0 = 4 bits
#define LCD_N        BIT3                        // BIT3 = N, 1 = 2 linhas, 0 = 1 linha
#define LCD_F        0                           // BIT2 = f, 1 = matriz 5x10, 0 = matriz 5x8


/*******************************************************************************
NOME: envia_comando 
FUNÇÃO: Faz RS = 0 e envia uma palavra para o display que será interpretada
        como comando
ENTRADA: palavra
SAIDA: -
*******************************************************************************/

void envia_comando(unsigned char palavra)
{
  LCDCtl_ie &= ~TecCols;                        // Desabilita as INTs das colunas do teclado
  LCDCtl_dir |= LCDRS;                          // Redireciona bit como saída
  LCDCtl_out &= ~LCDRS;                         // Faz RS = 0 = comando
  envia(palavra);                               // Envia a palavara para o display
  LCDCtl_dir &= ~LCDRS;                         // Redireciona bit como entrada
  LCDCtl_ifg &= ~TecCols;                       // Reseta os flags de INT que podem ter sido ativados
  LCDCtl_ie |= TecCols;                         // Rehabilita as INTs das colunas do teclado
  
  // Ao final da rotina é importante que o registrador de saída que atende ao 
  // teclado tenha em nível zero os bits correspondentes às colunas C1 a C4 para
  // que assim seja efetivado o pull down.
  LCDCtl_out &= ~(TecC1+TecC2+TecC3);
}

/*******************************************************************************
NOME: envia_dado      
FUNÇÃO: Faz RS = 1 e envia uma palavra para o display que será interpretada
        como caracteres ASCII a ser exibido
ENTRADA: palavra      
SAIDA: -
*******************************************************************************/

void envia_dado(unsigned char palavra)
{
  LCDCtl_ie &= ~TecCols;                        // Desabilita as INTs das colunas
  LCDCtl_dir |= LCDRS;                          // Redireciona bit como saída
  LCDCtl_out |= LCDRS;                          // Faz RS = 1 = Dado ou caracterer
  envia(palavra);                               // Envia a palavara para o display
  LCDCtl_dir &= ~LCDRS;                         // Redireciona bit como entrada
  LCDCtl_ifg &= ~TecCols;                       // Reseta os flags de INT que podem ter sido ativados
  LCDCtl_ie |= TecCols;                         // Rehabilita as INTs das colunas  
  
  // Ao final da rotina é importante que o registrador de saída que tbm atende ao 
  // teclado tenha em nível zero os bits correspondentes às colunas C1 a C4 para
  // que assim seja efetivado o pull down.
  LCDCtl_out &= ~(TecC1+TecC2+TecC3);
}


/*******************************************************************************
Função para o display LCD - Conexão de 4bits
Nome: Envia
Função: envia um byte para o display. Pode ser tanto comando quanto dado
         Utiliza a conexão de 4 bits com o display, sendo:
         D7 = P1.7
         D6 = P1.6
         D5 = P1.5
         D4 = P1.4
*******************************************************************************/

void envia (unsigned char palavra)
{   
  LCDPort_out = (LCDPort_out & 0x0F) + (palavra & 0xF0); // Envia apenas o nibble superior
  LCDCtl_out &= ~LCDEn;           //Gera pulso de nível alto no pino Enable do display
  LCDCtl_out |= LCDEn;
  LCDCtl_out &= ~LCDEn;
  __delay_cycles(_100us);
  LCDPort_out = (LCDPort_out & 0x0F) + ((palavra<<4) & 0xF0); // Envia apenas o nibble inferior
  LCDCtl_out |= LCDEn;            //Gera pulso de nível alto no pino Enable do display
  LCDCtl_out &= ~LCDEn;

  
  // Restabelece os 4 bits das linhas do teclado com nível alto.
  LCDPort_out |= TecLins;
}
/*******************************************************************************
 Nome: EnviaString
 Função para enviar palavras para o display
*******************************************************************************/
void envia_string(char *string)
  {
    int i=0;
    while(*(string+i))    
      {
     envia_dado(*(string+i));
     i++;
      }
  }

/*******************************************************************************
Função de configuração do I/Os relativos ao teclado
Nome: Config_Tec
*******************************************************************************/

void Config_Tec(void)
{
  // Configurar a linhas como saídas e as colunas como entrada
  // Ativar os resistores de pull-down nas C1 a C4. 
  // A coluna C1 é compartilhada com o pino RS do display e já possui o seu 
  // pull-down (um resistor externo de 4k7).
  TecLin_sel &= ~TecLins;               // Seleciona I/O
  TecLin_dir |= TecLins;                // As Linhas são saídas
  TecLin_out |= TecLins;                // Inicializa as linhas com 1
  
  TecCol_sel &= ~TecCols;               // Seleciona I/O             
  TecCol_dir &= ~TecCols;               // As Colunas são entradas
  TecCol_ies &= ~TecCols;               // Seleciona INT na borda de subida
  TecCol_ifg &= ~TecCols;               // Reseta flags de INT
  TecCol_ie  |=  TecCols;               // Habilita INTs
  TecCol_ren |= (TecC1+TecC2+TecC3);               // Habilita pull up ou down
  TecCol_out &= ~(TecC1+TecC2+TecC3);               // Seleciona pull down   
}
/*******************************************************************************
Função de configuração do ADC10
Nome: Config_ADC
*******************************************************************************/
void Config_ADC(void)
{
  P1SEL |= BIT0;
  ADC10CTL1 = INCH_0 + ADC10DIV_7 + ADC10SSEL_0 + CONSEQ_2;
  ADC10CTL0 = ADC10SHT_3 + ADC10ON + REF2_5V  + REFON + SREF_1 + ADC10IE + MSC;
  P1DIR |= (BIT2);
  desliga_pel;
  P1DIR |= (BIT3);
  desliga_vent;
  P1DIR &= ~BIT1;
  P1REN |= BIT1;
  P1OUT &= ~BIT1;
}
/*******************************************************************************
Função de configuração do I/Os relativos ao display
Nome: Config_LCD
*******************************************************************************/

void Config_LCD(void)
{
  // As linhas do teclado são compartilhadas com o barramento de dados de 4
  // bits do display, mesmo assim vamos configura-las aqui nesta rotina tbm.
  // Assim a rotina fica independente a rotina de configuração de tecaldo,
  // caso as conexões sejam alterada e deixem de ser compartilhadas.

  // Configura os 4 bits do barramento de dados como I/O
  LCDCtl_sel &= ~(LCDEn + LCDRS);       //Seleciona I/O

  // Configura pinos Enable e RS como saída e inicializa com zero
  LCDCtl_dir |=   LCDEn + LCDRS;
  LCDCtl_out &= ~(LCDEn + LCDRS);

  // Configura os bit do barramento de dados como saída
  LCDPort_sel &= ~LCDData;              // Seleciona I/O
  LCDPort_dir |=  LCDData;              // Configura como saída
    
}
/*******************************************************************************
NOME: envia_meio_comando
FUNÇÃO: Faz RS = 0 e envia o nibble superir para o display que será interpretado
        como comando
        Esta função é utilizada na inicialzação do display com a interface de
        4 bits. Neste caso o nibble 0010 deve ser enviado uma vez sozinho e
        depois combinado com o nibble inferior. Este último envio é feito
        com a função envia_comando já existente.
ENTRADA: palavra
SAIDA: -
*******************************************************************************/

void envia_meio_comando(unsigned char palavra)
{
  LCDCtl_ie &= ~TecCols;                        // Desabilita as INTs das colunas do teclado
  LCDCtl_out &= ~LCDRS;                         // Faz RS = 0 = comando

  // Envia apenas o nibble superior
  LCDPort_out = (LCDPort_out & 0x0F) + (palavra & 0xF0);
  LCDCtl_out &= ~LCDEn;           //Gera pulso de nível alto no pino Enable do display
  LCDCtl_out |= LCDEn;
  LCDCtl_out &= ~LCDEn;
  __delay_cycles(_100us);
  LCDCtl_ifg &= ~TecCols;                       // Reseta os flags de INT que podem ter sido ativados
  LCDCtl_ie |= TecCols;                         // Rehabilita as INTs das colunas do teclado

  // Restabelece os 4 bits das linhas do teclado com nível alto.
  LCDPort_out |= TecLins;

}
/*******************************************************************************
NOME: Init_LCD
FUNÇÃO: inicializa o display
*******************************************************************************/
void Init_LCD()
{
  __delay_cycles(_50ms);
  envia_meio_comando(0x30);      // Envia apenas o nibble superior 0011 por 3 vezes seguidas
  __delay_cycles(_5ms);
  envia_meio_comando(0x30);
  __delay_cycles(_100us);
  envia_meio_comando(0x30);
  envia_meio_comando(0x20);      // Envia apenas o nibble superior 0010
  envia_comando(0x28);           // 4 bits de dados- duas linhas- matriz 5x8
  __delay_cycles(_5ms);
  envia_comando(0x0C);           //display e cursor ativos sem piscar
  envia_comando(0x06);           // deslocamento cursor para direita
  envia_comando(0x01);           // limpa display e retorna o cursor para 1 posição
  __delay_cycles(_2ms);
}
/*******************************************************************************
 Nome: teclado
 Descobre linha do teclado pressionada
*******************************************************************************/
#pragma vector=PORT2_VECTOR
__interrupt void teclado ()
{ 
  const char TAB_TEC [] = {"\00""123456789*0#"}; 
    unsigned char j = 0;   
    __delay_cycles (_15ms);
    if (TecCol_in & (TecC1+TecC2+TecC3))                  // Alguma linha pressionada
   {  
        TecLin_out |= TecL1;
        TecLin_out &= ~(TecL2+TecL3+TecL4);
      if (TecCol_in & (TecC1+TecC2+TecC3))                // Primeira linha pressionada
        {
          j = descobrir(j);
	}
        TecLin_out |= TecL2;
        TecLin_out &= ~(TecL1+TecL3+TecL4);
      if (TecCol_in & (TecC1+TecC2+TecC3))                // Segunda linha pressionada
        {
          j += 0x03;
          j = descobrir(j);
        }
        TecLin_out |= TecL3;
        TecLin_out &= ~(TecL1+TecL2+TecL4);
      if (TecCol_in & (TecC1+TecC2+TecC3))                // Terceira linha pressionada
        {
          j += 0x06;
          j = descobrir(j);
        }
        TecLin_out |= TecL4;
        TecLin_out &= ~(TecL1+TecL2+TecL3);
      if (TecCol_in & (TecC1+TecC2+TecC3))                // Quarta linha pressionada
      {
        j += 0x09;
        j = descobrir(j); 
      }
      tecla = TAB_TEC [j];
      j = 0;
      TecLin_out  |= (TecL1+TecL2+TecL3+TecL4);
   }
    TecCol_ifg  =  TecCol_ifg & ~(TecC1+TecC2+TecC3);        // Zera os flags de interrupção
}
/*******************************************************************************
 Nome: descobrir
 Descobre a coluna do teclado pressionada
*******************************************************************************/

  int descobrir (unsigned char j)
  {   
    unsigned char coluna;
    coluna = TecCol_in & (TecC1+TecC2+TecC3);
    switch (coluna)
    {
      case TecC1:                                        // Primeira coluna pressionada        
        j +=1;
        break;
      case TecC2:                                        // Segunda coluna pressionada
        j +=2;
        break;
      case TecC3:                                        // Terceira coluna pressionada
        j +=3;
        break;   
    } 
      return j;     
  }