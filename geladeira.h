/*******************************************************************************
NOME: sis_ligado_horario 
FUN��O: Fun��o que define o horario de ligar a geladeira, caso o usuario queira
*******************************************************************************/
void sis_ligado_horario (void)
{
  char horas_d;                                         // Hora desejada
  char minutos_d;                                       // Minuto desejado
  char segundos_d;                                      // Segundo desejado
  char i = 0;                                           // Variavel que define qual posi��o do caracter esta sendo escrita
  envia_digite;
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
        horas_d = 10 * (tecla - 0x30);
        break;
      case 1:                                           // Unidade das horas
        envia_comando (0x84);
        envia_dado (tecla);
        horas_d += (tecla - 0x30);
        break;
      case 2:                                           // Dezena das horas
        envia_comando (0x86);
        envia_dado (tecla);
        minutos_d = 10 * (tecla - 0x30);
        break;
      case 3:                                           // Unidade das horas
        envia_comando (0x87);
        envia_dado (tecla);
        minutos_d += (tecla - 0x30);
        break;
      case 4:                                           // Dezena das horas
        envia_comando (0x89);
        envia_dado (tecla);
        segundos_d = 10 * (tecla - 0x30);
        break;
      case 5:                                           // Unidade das horas
        envia_comando (0x8A);
        envia_dado (tecla);
        segundos_d += (tecla - 0x30);
        break;
      }
      i++;                                              // Proxima posi��o
      tecla = 0;
    }
  }
  if ((horas_d > 23) || (minutos_d > 59) || (segundos_d > 59))          // Caso horario digitado seja maior que o permitido
  {
    envia_comando (0x80);
    envia_string ("      ERRO      ");
    __delay_cycles (_1s);
    sis_ligado_horario ();
  }
  envia_comando (0xC0);
  envia_string ("CONFIRMA?       ");
  while (tecla != '#')
  {
    if (tecla == '*')sis_ligado_horario ();             // Anula a a��o e volta ao inicio da fun��o
  }
  tecla = 0;
  limpa_2;                                              // Limpa a segunda linha         
  envia_comando (0xC0);
  envia_string ("ESPERANDO A HORA  ");
  h = 1; 
  while (((horas != horas_d) || (minutos != minutos_d) || (segundos != segundos_d)) && (tecla != '*'))  // Enquanto n�o der o horario fica nesse loop
  {}
  tecla = 0;
}
/*******************************************************************************
NOME: sis_ligado
FUN��O: Fun��o que ativa o sistema de resfriamento e medi��o
*******************************************************************************/
void sis_ligado (float temp, char valor, char t)
{
  char string [14];
  char estring [8];
  char valor_c, valor_f;
  char i = 0;                                           // Variavel que diz se a geladeira esta ligada (0) ou n�o (1)
  if (celsius)                                          // Caso unidade seja celsius
  {
    valor_c = valor;
    valor_f = (((valor_c * 9)/5) + 32);
  }
  if (fah)                                              // Caso unidade seja fahrenheit
  {
    valor_f = valor;
    valor_c = (((valor_f - 32) * 5)/9);
  }
  while (tecla != '*')                                  // Enquanto * n�o for pressionada a geladeira continuar� ligada
  {
    if (y==4000)
    {
      envia_comando (0xCE);
      if (celsius)
      {
        envia_valor_c
        envia_graus_c;
        envia_temp_c;
        envia_graus_c;
        envia_string ("  ");
      }
      if (fah)
      {
        envia_valor_f
        envia_graus_f;
        envia_temp_f;
        envia_graus_f;
      }
      if (sensor_mag && (i == 0))                       // Ira ligar a geladeira somente se a porta estiver fechada e tecla = #
      {
        //temp = temp - 0.4;
        envia_lig;
        if (temp >= valor) liga_pel;                    // Caso a temperatura for maior que o valor a peltier � ligada
        if (temp <= (valor - tolerancia)) desliga_pel;  // Caso a temperatura for menor que o valor - a tolerancia a peltier � desligada
        liga_vent;
        desliga_led;
      }
      if (sensor_mag==0)                                // Caso a porta for aberta, as ventoinhas e a peltier s�o desligadas
      {
        desliga_pel;
        desliga_vent;
        envia_par;
        liga_led;
      }
      if ((i == 1) && (sensor_mag != 0))                // Caso a porta esteja fechada e a tecla * for pressionada somente a peltier � desligada
      {
        desliga_pel;
        envia_par;
        liga_vent;
        desliga_led;
      }
    }
    if (tecla == '1')                                   // A tecla 1 troca as unidades de temperatura
    {
      t ^= BIT0;
      tecla = 0;
    }
    if (tecla == '#')i = 0;
    if (tecla == '0')i = 1;
  }
}
/*******************************************************************************
NOME: adc
FUN��O: Interrup��o ADC
*******************************************************************************/
#pragma vector = ADC10_VECTOR
__interrupt void adc ()
{
  if (y==4001)
  {y=0;}
  y++;
  tensao = ADC10MEM;
}