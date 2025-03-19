/*******************************************************************************
NOME: atualiza
FUN��O: Fun��o que atualiza o display com o horario atual
*******************************************************************************/
void atualiza (void)
{
  char string [4];
  envia_comando (0x83);
  sprintf (string, "%2d:", horas);
  if (string [0] == 0x20)string [0] = 0x30;
  envia_string (string);
  sprintf (string, "%2d:", minutos);
  if (string [0] == 0x20)string [0] = 0x30;
  envia_string (string);
  sprintf (string, "%2d", segundos);
  if (string [0] == 0x20)string [0] = 0x30;
  envia_string (string);
}

/*******************************************************************************
NOME: config_timer
FUN��O: Configura��o do timer0
*******************************************************************************/
void config_timer()
{
  BCSCTL2  = DIVS_2;                            //Divide o clock de 1MHz do DCO por 4
  TA0CCTL0 = CCIE;                              // Ativa a interrup��o de captura/compara        
  TA0CTL   = TASSEL_2 + ID_3 + MC_1 + TACLR;    // Seleciona o SMCLK como fonte, divide o clock por 8, seleciona o modo crescente e limpa o TA
  TA0CCR0  = 34375;                             // Configura como 1 segundo a base de tempo
}

/*******************************************************************************
NOME: relogio 
FUN��O: Fun��o de interrup��o do timer, que atualiza o relogio
*******************************************************************************/
#pragma vector = TIMER0_A0_VECTOR
__interrupt void relogio()
{ 
  // Parte para o teclado n�o ficar com delay
  TA0CCTL0 &= ~CCIFG;
  TA0CCTL0 &= ~CCIE; 
  __enable_interrupt ();
  
  if(segundos < 59)
  {segundos ++;}
  else
  {
    segundos = 0;
    if(minutos <59)
    {minutos ++;}
    else
    {
      minutos = 0;
      if(horas <23)
      {horas ++;}
      else
      {
        segundos = 0;
        minutos = 0;
        horas = 0;
      }
    }
  }
  if (h == 1)atualiza ();                       // So ir� atualizar o display se h for igual a 1
  TA0CCTL0 &= ~CCIFG;
  TA0CCTL0 = CCIE;
}

/*******************************************************************************
NOME: configura_relogio
FUN��O: Fun��o de configura��o inicial do relogio
*******************************************************************************/
void configura_relogio (void)
{
  char i = 0;                                   // Variavel que define qual posi��o do caracter esta sendo escrita
  envia_comando (0x80);
  envia_string ("   00:00:00           ");
  envia_comando (0xC0);
  envia_string ("DIGITE O HORARIO  ");
  tecla = 0;
  while (i <= 5)
  {
    if (tecla && tecla != '*' && tecla != '#')
    {
      switch (i)
      {
      case 0:                                           // Dezena das horas
        envia_comando (0x83);
        envia_dado (tecla);
        horas = 10 * (tecla - 0x30);
        break;
      case 1:                                           // Unidade das horas
        envia_comando (0x84);
        envia_dado (tecla);
        horas += (tecla - 0x30);
        break;
      case 2:                                           // Dezena das horas
        envia_comando (0x86);
        envia_dado (tecla);
        minutos = 10 * (tecla - 0x30);
        break;
      case 3:                                           // Unidade das horas
        envia_comando (0x87);
        envia_dado (tecla);
        minutos += (tecla - 0x30);
        break;
      case 4:                                           // Dezena das horas
        envia_comando (0x89);
        envia_dado (tecla);
        segundos = 10 * (tecla - 0x30);
        break;
      case 5:                                           // Unidade das horas
        envia_comando (0x8A);
        envia_dado (tecla);
        segundos += (tecla - 0x30);
        break;
      }
      i++;                                              // Proxima posi��o
      tecla = 0;
    }
  }
  if ((horas > 23) || (minutos > 59) || (segundos > 59))          // Caso horario digitado seja maior que o permitido
  {
    envia_comando (0x80);
    envia_string ("      ERRO      ");
    __delay_cycles (_1s);
    configura_relogio ();
  }
  envia_comando (0xC0);
  envia_string ("CONFIRMA?       ");
  while (tecla != '#')
  {
    if (tecla == '*')configura_relogio ();                      // Anula a a��o e volta ao inicio da fun��o
  }
  tecla = 0; 
}