#include <stdio.h>
#include <stdlib.h>
#include "mgos.h"
#include "output.h"
#include "input.h"
#include "def.h"

#define DEBOUNCE_TIME       50
#define FOLLOW_MODE         

void In1_handler_Neg();
void In1_handler_Pos();
void In2_handler_Neg();
void In2_handler_Pos();
void clean_previsious_trigger(uint8_t out);

#ifdef FOLLOW_MODE //configurar como follow mode - neste modo a saida vai "seguir" o estado da entrada
                   //caso não esteja selecionado as entradas vão funcionar por transição descendente.

/********************************************************
 Função aonde se declara as callbacks para as entradas
 e a respetiva configuração (pull up, pull down ou nenhum)
 ********************************************************/
void set_trigger_edge(uint8_t out){ 
    if(out == 1){
        if(!input.input1_status){
            mgos_gpio_set_button_handler(IN1, MGOS_GPIO_PULL_UP, MGOS_GPIO_INT_EDGE_POS, DEBOUNCE_TIME, In1_handler_Neg, NULL);
        }else if(input.input1_status){
            mgos_gpio_set_button_handler(IN1,MGOS_GPIO_PULL_UP, MGOS_GPIO_INT_EDGE_NEG, DEBOUNCE_TIME, In1_handler_Pos, NULL);
        }
    }else if(out == 2){
        if(!input.input2_status){
            mgos_gpio_set_button_handler(IN2, MGOS_GPIO_PULL_UP, MGOS_GPIO_INT_EDGE_POS, DEBOUNCE_TIME, In2_handler_Neg, NULL);
        }else if(input.input2_status)
            mgos_gpio_set_button_handler(IN2,MGOS_GPIO_PULL_UP, MGOS_GPIO_INT_EDGE_NEG, DEBOUNCE_TIME, In2_handler_Pos, NULL);
    }
}
/********************************************************
 Conjunto de funções responsaveis por atuar as saidas
 conforme o estado das entradas
 ********************************************************/

void In1_handler_Neg(){
    input.input1_status = true;
    turn_on(1);
   // clean_previsious_trigger(1);
    set_trigger_edge(1);
}

void In1_handler_Pos(){
    input.input1_status = false;
    turn_off(1);
    //clean_previsious_trigger(1);
    set_trigger_edge(1);
}

void In2_handler_Neg(){
    input.input2_status = true;
    turn_on(2);
    //clean_previsious_trigger(2);
    set_trigger_edge(2);
}

void In2_handler_Pos(){
    input.input2_status = false;
    turn_off(2);
    //clean_previsious_trigger(2);
    set_trigger_edge(2);
}

/********************************************************
 Função de limpeza de callbacks 
 ********************************************************/
void clean_previsious_trigger(uint8_t out){
    if(out == 1){
        mgos_gpio_set_button_handler(IN1, MGOS_GPIO_PULL_UP, MGOS_GPIO_INT_EDGE_NEG, DEBOUNCE_TIME, NULL, NULL);
    }else if(out == 2){
        mgos_gpio_set_button_handler(IN2, MGOS_GPIO_PULL_UP, MGOS_GPIO_INT_EDGE_NEG, DEBOUNCE_TIME, NULL, NULL);
    }
}
#endif
#ifndef FOLLOW_MODE
/********************************************************
 Funções para o caso de não se pretender follow mode
 ********************************************************/

void In1_handler_Neg(){
    toggle_output(1);
}
void In2_handler_Neg(){
    toggle_output(2);
}
void set_trigger_edge(uint8_t out){
    if(out ==1){
        mgos_gpio_set_button_handler(IN1, MGOS_GPIO_PULL_UP, MGOS_GPIO_INT_EDGE_NEG, DEBOUNCE_TIME, In1_handler_Neg, NULL);
    }else if(out == 2){
        mgos_gpio_set_button_handler(IN2, MGOS_GPIO_PULL_UP, MGOS_GPIO_INT_EDGE_NEG, DEBOUNCE_TIME, In2_handler_Neg, NULL);
    } 
}

#endif
/********************************************************
 Função aonde se inicializa os pinos de entrada
 ********************************************************/
void init_inputs(){
    mgos_gpio_set_mode(IN1, MGOS_GPIO_MODE_INPUT);
    mgos_gpio_set_mode(IN2, MGOS_GPIO_MODE_INPUT);
    input.input1_status = false;
    input.input2_status = false;
    mgos_gpio_enable_int(IN1);
    mgos_gpio_enable_int(IN2);
    set_trigger_edge(1);
    set_trigger_edge(2);
}





    