#ifndef _DEF_H
#define _DEF_H

/**************************************
Definição dos pinos de entrada e saida
***************************************/
#define IN1    13
#define IN2    12
#define OUT1   14
#define OUT2   4 

/*******************************************
 Estrutura que armazena os dados dos outputs
  output1_status - estado da saida 1
  output2_status - estado da saida 2
  output1_onTime - temporização da saida 1
  output2_onTime - temporização da saida 2
********************************************/
typedef struct outputStruct{
    bool output1_status;
    bool output2_status;
    int output1_onTime;
    int output2_onTime;
}output_data;

/*******************************************
  Estrutura que armazena os dados dos inputs
  input1_status - estado da entrada 1
  input2_status - estado da entrada 2
  input1_click_type - tipo de entrada 1
  input2_click_type - tipo de entrada 2
********************************************/  
typedef struct inputStruct{ 
    bool input1_status;
    bool input2_status;
    uint8_t input1_click_type;
    uint8_t inpu2_click_type;
}input_data;

/*******************************************
  Estrutura que armazena os dados de rede
 ip - ip do modulo
 sm - mascara de rede do modulo
 gtw - gateway de rede do modulo
 dhpc - flag de dhcp
********************************************/  
typedef struct networkStruct{
  int ip[4];
  int sm[4];
  int gtw[4];
  int dhcp;
}network_data;


network_data network;
output_data output;
input_data input;

#endif