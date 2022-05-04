# Codigo exemplo

 

## Descrição

Este firmware permite activar/desativar via wifi as saidas dos modulos tal como ver o estado de entrada.  Também é possivel escolher em compilação de escolher como as entradas digitais vão influenciar as saidas, podendo ser *follow mode* em que a saida vai "seguir" o estado de uma das entradas digitais ou *transition mode* aonde no caso de uma transição negativa do estado de uma entrada digital vai alterar o estado de uma saida.

Este firmware foi desenvolvido utilizando a framework mongoose os.



## Utilização

#### Estrutura geral

 - **Libraries** - diretório com as librarias locais

    - UI - Pagina web que permite configurar as definições do wifi (ssid e password).
    - Wifi - libraria para gerir as definições de wifi e servidor para o UI.

 - **src** - diretório com o código especifico ao projeto

    - main - inicialização das rotinas do firmware.

    - def - declaração das  estruturas de dados gerais ao firmware.

    - discovery - implementação da rotina de resposta a um pedido multicast (UDP broadcast) com as suas configurações de rede e macaddress.

       - | URI                            | Função                    | Argumento         | Resposta                                                     |
         | ------------------------------ | ------------------------- | ----------------- | ------------------------------------------------------------ |
         | udp://255.255.255.0 porta 8654 | Descobrir módulos na rede | IOTBRAGACHALLENGE | IP:xx:xx:xx:xx,<br />GW:xx:xx:xx:xx,<br />SM:xx:xx:xx:xx,<br />MAC:xx:xx:xx:xx:xx,<br />FW:x.x.x,<br />HW:x.x.x:<br />DHCP:x |

         

    - input - implementação das interrupções de entrada.

    - output - implementação das rotinas de atuação.

    - cgi - servidor HTTP e respetivos endpoints. Endpoints disponíveis:

      | endpoint                  | Função                                            | argumentos                                                | resposta                    |
      | ------------------------- | ------------------------------------------------- | --------------------------------------------------------- | --------------------------- |
      | /output_command.cgi       | Alterar o estado das saídas                       | Formato JSON  "{output1: true/false, output2:true/false}" | ok / bad request            |
      | /input_state.shtml        | Receber o estado das entradas                     | n/a                                                       | {input1: 1/0, input2:1/0}   |
      | /output_state.shtml       | Receber o estado das saídas                       | n/a                                                       | {output1: 1/0, output2:1/0} |
      | /clean_configurations.cgi | Limpar as configurações de wifi (ssid e password) | n/a                                                       | ok / bad request            |

      

 - **mos.yml** - ficheiro responsável pela descritivo do projeto, configuração de variáveis de sistema e inclusão de librarias externas e locais.

   



## Instruções

#### Ferramentas necessárias

- **Kit de desenvolvimento BragaIOTchallenge**

- **Cabo mini USB - USB.**

- **Mongoose OS** - ver como instalar em [Mongoose OS](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md#1-download-and-install-mos-tool)

- **[Postman](https://www.postman.com/downloads/)** - aplicação para comunicar com os módulos via wifi.

- **Visual studio code** ou equivalente.

- Aplicação para enviar/receber UDP ou em alternativa um Ip scanner

  

#### Testar codigo exemplo

- Fazer download do codigo exemplo
- Abrir a interface do mongoose OS.
- Colocar o caminho para o diretório com o código exemplo.
- Compilar o código.
  - Caso não tenham instalado o docker será via o comando "mos build" senão será "mos build--local". Para mais informação ver [Mongoose OS](https://mongoose-os.com/docs/mongoose-os/quickstart/setup.md#1-download-and-install-mos-tool) .
- Ligar o modulo Dona conforme as ligações descritas em flash.md.
- Programar o modulo utilizando o comando "mos flash".
- Após programar retirar o modulo Dona do modo programação (desligar o fio PROG MODE e ligar e desligar o modulo). 
- Aceder á rede wifi "BIOTC_EXEMPLO" com a password "bragaiotchallenge". Estes parâmetros podem ser alterados no ficheiro mos.yml.
- Aceder através do browser a 192.168.4.1/index.html.
- Configurar as definições rede.
- Através da aplicação UDP sender/receiver enviar um multicast para descobrir o modulo ou em alternativa fazer um scan à rede.
- Através do postman enviar os endpoints descritos em cima.



## Links com interesse

- https://github.com/cesanta/mongoose
- https://github.com/cesanta/mongoose-os
- https://www.freertos.org/

