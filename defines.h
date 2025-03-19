// Biblioteca de defines
#define tmax_c 25
#define tmin_c 5
#define tmax_f (((tmax_c * 9)/5) + 32)
#define tmin_f (((tmin_c * 9)/5) + 32)
#define tolerancia 0.2
#define liga_pel        P1OUT &= ~BIT2;
#define desliga_pel     P1OUT |= BIT2;
#define liga_vent       P1OUT &= ~BIT3;
#define desliga_vent    P1OUT |= BIT3;
#define liga_led        P2OUT |= BIT2;
#define desliga_led     P2OUT &= ~BIT2;
#define envia_temp_f    temp = ((2.5*tensao)/1023);temp = (temp/71.43e-3);temp = (((temp * 9)/5) + 32);sprintf (string, "TG = %2.1f", temp);envia_comando (0x80);envia_string (string);
#define envia_temp_c    temp = ((2.5*tensao)/1023);temp = (temp/71.43e-3);if ((P1OUT & BIT2) ==0 )temp -= 0.35;sprintf (string, "TG atual = %2.1f", temp);envia_comando (0x80);envia_string (string);
#define envia_valor_c   sprintf (estring, "TR = %2i", valor_c);envia_comando (0xC0);envia_string (estring);
#define envia_valor_f   sprintf (estring, "TR = %2i", valor_f);envia_comando (0xC0);envia_string (estring);
#define envia_erro      envia_comando (0x80);envia_string (erro);envia_comando (0xC0);envia_string ("                ");
#define envia_turbo     envia_comando (0xC0);envia_string (turbo);
#define envia_desliga   envia_comando (0x80);envia_string (gel);envia_comando (0xC0);envia_string (des);
#define envia_esc       envia_comando (0x80);envia_string (tempe);envia_comando (0xC0);envia_string ("                ");
#define envia_anu       envia_comando (0x80);envia_string (anu);envia_comando (0xC0);envia_string ("                ");
#define para_adc        ADC10CTL0 &= ~(ENC + ADC10SC);
#define inicia_adc      ADC10CTL0 |= ENC + ADC10SC; 
#define envia_ini       envia_comando (0xC0);envia_string (ini);
#define envia_conf      envia_comando (0xC0);envia_string (conf);
#define envia_graus_c   envia_dado (0xdf);envia_dado ('C');
#define envia_graus_f   envia_dado (0xdf);envia_dado ('F');
#define envia_dez       envia_comando (0x8C);envia_dado (tecla);
#define envia_uni       envia_comando (0x8D);envia_dado (tecla);
#define sensor_mag      (P1IN & BIT1)
#define envia_lig       envia_comando (0xC9);envia_string ("|Ligado");
#define envia_par       envia_comando (0xC9);envia_string ("|Parado");
#define temp_em         envia_comando (0x80);envia_string ("Temperatura em   ");envia_comando (0xC0);envia_graus_c;envia_string ("(1) ou ");envia_graus_f;envia_string ("(2)   ");
#define limpa_1         envia_comando (0x80);envia_string ("                        ");
#define limpa_2         envia_comando (0xC0);envia_string ("                        ");
#define envia_c_f       envia_comando (0x80);envia_string ("Caio e Felipe");envia_comando (0xC0);envia_string ("Resfriador"); __delay_cycles (_2s);
#define envia_deseja    envia_comando (0x80);envia_string ("Deseja escolher");envia_comando (0xC0);envia_string ("um horario?");
#define envia_digite    envia_comando (0x80);envia_string ("   00:00:00           ");envia_comando (0xC0);envia_string ("DIGITE O HORARIO  ");
#define celsius         (t == 0)
#define fah             (t == 1)