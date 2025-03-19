# Geladeira Peltier
Este repositório contém o projeto de uma mini geladeira que utiliza uma pastilha de efeito Peltier em conjunto com um microcontrolador MSP430, visando proporcionar um sistema de refrigeração compacto e eficiente.

## Descrição do Projeto
A mini geladeira foi desenvolvida com o intuito de explorar as propriedades termoelétricas das pastilhas Peltier, que permitem a transferência de calor de um lado para o outro quando submetidas a uma corrente elétrica. O microcontrolador MSP430 é responsável por gerenciar o funcionamento do sistema, controlando a temperatura interna e o acionamento do módulo Peltier conforme necessário.

## Estrutura do Repositório
main.c: Arquivo principal contendo o código-fonte que implementa a lógica de controle da geladeira.

defines.h: Arquivo de cabeçalho com definições e constantes utilizadas no projeto.

geladeira.h: Arquivo de cabeçalho específico para as funções relacionadas ao controle da geladeira.

Lib_G2553.h: Arquivo de cabeçalho que contém bibliotecas e definições específicas para o microcontrolador MSP430G2553.

relogio.h: Arquivo de cabeçalho responsável pelas funções de temporização e controle de tempo no sistema.

## Funcionamento
O sistema monitora constantemente a temperatura interna da geladeira por meio de sensores apropriados. Quando a temperatura ultrapassa um determinado limite, o microcontrolador ativa a pastilha Peltier para resfriar o interior até que a temperatura desejada seja atingida. Após alcançar a temperatura alvo, o sistema desativa a pastilha para economizar energia e evitar resfriamento excessivo. Ademais, é possível selecionar a opção de temporizar o funcionamento da geladeira, isto é setar um horário para ligá-la e/ou desligá-la

## Observações Importantes
Durante o desenvolvimento e testes do projeto, identificou-se uma limitação significativa relacionada ao ajuste da curva de histerese para o chaveamento do relé. Em determinadas situações, ao atingir a temperatura final, o sistema acionava o relé repetidamente em um curto espaço de tempo, o que pode comprometer a eficiência e a durabilidade do dispositivo. Recomenda-se uma análise mais aprofundada e ajustes no algoritmo de controle para mitigar este comportamento indesejado.
Além disso, por se tratar de um projeto do ensino técnico, algumas implementações não estão em sua forma mais otimizada, mas achei válido colocá-lo aqui para servir de ajuda para alguém com a ideia de fazer algo parecido.
