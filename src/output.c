#include <stdio.h>
#include "mgos.h"
#include <stdlib.h>
#include "output.h"
#include "def.h"


mgos_timer_id timerID_1;
mgos_timer_id timerID_2;
int timer1_counter;
int timer2_counter;

void on_timed_1();
void on_timed_2();

/********************************************************
 Função aonde se inicializa os pinos de saida
 ********************************************************/
void init_outputs() {
    LOG(LL_INFO,("Inicializar saidas"));
    mgos_gpio_set_mode(OUT1, MGOS_GPIO_MODE_OUTPUT);
    mgos_gpio_set_mode(OUT2, MGOS_GPIO_MODE_OUTPUT);
    mgos_gpio_write(OUT1, false);
    mgos_gpio_write(OUT2, false);
}

/********************************************************
 Função aonde se inicializa os timers (por software)
 ********************************************************/
void start_timer(uint8_t out){
    if(out == 1){
        timerID_1 = mgos_set_timer(1000, MGOS_TIMER_REPEAT, on_timed_1, NULL);
    }else if (out == 2){
        timerID_2 = mgos_set_timer(1000, MGOS_TIMER_REPEAT, on_timed_2, NULL);
    }
}

/********************************************************
 Função aonde se interrompe os timer de saida
 ********************************************************/
void stop_timer(uint8_t out){
    if(out == 1){
        timer1_counter = 0;
	    mgos_clear_timer(timerID_1);
    }else if (out == 2){
        timer2_counter = 0;
		mgos_clear_timer(timerID_2);
    }
}

/********************************************************
 Função para ativar as saidas
 ********************************************************/
void turn_on(uint8_t out) { 
    if(out == 1){
        mgos_gpio_write(OUT1,true); 
        output.output1_status = true;
        LOG(LL_INFO,("Saida 1 activa"));      
    }else if(out == 2){
        mgos_gpio_write(OUT2,true);
        output.output2_status = false;
        LOG(LL_INFO,("Saida 2 ativa"));
	}
}

/********************************************************
 Função para desativar as saidas
 ********************************************************/
void turn_off(uint8_t out) { 
        if(out == 1){
            mgos_gpio_write(OUT1,false);
            output.output1_status = false;
            LOG(LL_INFO,("Saida 1 desativa"));
        }else if(out == 2){
        mgos_gpio_write(OUT2,false);
        output.output2_status = false;
        LOG(LL_INFO,("Saida 2 desativa"));       
    }
}

/********************************************************
 Função para comutar as saidas
 ********************************************************/
void toogle_action(int out) {
    if(out == 1){
      if(mgos_gpio_toggle(OUT1) == 1){
           output.output1_status = true;
           LOG(LL_INFO,("Saida 1 activa"));  
      }else{
            output.output1_status = false;
            LOG(LL_INFO,("Saida 1 desativa"));
      }
    }else if(out == 2){
      if(mgos_gpio_toggle(OUT2) == 1){
            output.output2_status = false;
            LOG(LL_INFO,("Saida 2 ativa"));
       
      }else{		
            output.output2_status = false;
            LOG(LL_INFO,("Saida 2 desativa"));
      }
    } 
}

/********************************************************
 Função para saidas temporizadas (não utilizada)
 ********************************************************/
void on_Timed(uint8_t out, int time){
    if(out == 1 && output.output1_onTime > 0){
        if(!timer1_counter){
            turn_on(out);
            start_timer(out);
        }else{
            stop_timer(out);
            start_timer(out);
        }
    }else if(out == 2 && output.output2_onTime > 0){
        if(!timer2_counter){
            turn_on(out);
            start_timer(out);
        }else{
            stop_timer(out);
            start_timer(out);
        }
    }
}

/********************************************************
 Callback do timer para a saida 1
 ********************************************************/
void on_timed_1(){
	if(output.output1_onTime <= timer1_counter){
		turn_off(1);
		stop_timer(1);
	}
    else
		timer1_counter++;
}

/********************************************************
 Callback do timer para a saida 2
 ********************************************************/
void on_timed_2(){
	if(output.output2_onTime <= timer2_counter){
		turn_off(2);
		stop_timer(2);
	}
	else
		timer2_counter++;
}

/********************************************************
 Função para atualizar as saidas conforme os dados
 recebidos por HTTP
 ********************************************************/
void update_output(){
    if(output.output1_status == true){
        turn_on(1);
    }else{
        turn_off(1);
    }
    if(output.output2_status == true){
        turn_on(2);
    }else{
        turn_off(2);
    }
}