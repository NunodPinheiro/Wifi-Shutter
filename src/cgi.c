
#include <stdio.h>
#include "mgos.h"
#include <stdlib.h>
#include "cgi.h"
#include "def.h"
#include "mgos_http_server.h"
#include "mongoose.h"
#include "mgos_adc.h"
#include "output.h"
#include "discovery.h"

/*******************************************
 Função responsavel pela receção HTTP das 
 ordens de atuação de saida.
 Argumentos:
  entrada - mensagem HTTP recebida
  ação - Chama o handler das saidas e envia 
  feedback relativo à mensagem recebida
  Saida - void
********************************************/  
int get_output_ctrl_parameters(const char* msg);

static void output_command_handler(struct mg_connection *c, int ev, void* p, void *user_data){
    if (ev == MG_EV_HTTP_REQUEST) {
    struct http_message *hm = (struct http_message *) p;
    struct mg_str *s = hm->body.len > 0 ? &hm->body : &hm->query_string;
    
    if (get_output_ctrl_parameters(s->p)) {
      mg_send_response_line(c, 400, "Content-type: application/json\r\n\r\n{\"Bad Request\"}");
    } else {
      mg_send_response_line(c, 200, "Content-type: application/json\r\n\r\n{\"OK\"}");
    }
    
    c->flags |= MG_F_SEND_AND_CLOSE;
  }
}

/*******************************************
 Função responsavel por actualizar as saidas

  entrada - mensagem HTTP recebida
  Saida - void
********************************************/  
int get_output_ctrl_parameters(const char* msg){
    if(json_scanf(msg, strlen(msg), "{output1:%B, output2:%B}", &output.output1_status, &output.output2_status) > 0){
        update_output();
        return 0;
    }else{
        return 1;
    }    
}

/*******************************************
 Função responsavel pela nofificação HTTP do
 estado das entradas 
 Argumentos:
  entrada - mensagem HTTP recebida
  Saida - void
********************************************/  
static void input_state_handler(struct mg_connection *c, int ev, void *p, void *user_data){
  char msg[200];

  if (ev == MG_EV_HTTP_REQUEST) {
    sprintf(msg, "Content-type: application/json\r\n\r\n{\"input1\":\"%d\", \"input2\":\"%d\"}", !mgos_gpio_read(IN1), !mgos_gpio_read(IN2));
    mg_send_response_line(c, 200,msg);
    c->flags |= MG_F_SEND_AND_CLOSE;
  }
}

/*******************************************
 Função responsavel pela nofificação HTTP do
 estado das saidas
 Argumentos:
  entrada - mensagem HTTP recebida
  Saida - void
********************************************/  
static void output_state_handler(struct mg_connection *c, int ev, void *p, void *user_data){
  char msg[200];

  if (ev == MG_EV_HTTP_REQUEST) {
    sprintf(msg, "Content-type: application/json\r\n\r\n{\"output1\":\"%d\", \"output2\":\"%d\"}", mgos_gpio_read(OUT1), mgos_gpio_read(OUT2));
    mg_send_response_line(c, 200,msg);
    c->flags |= MG_F_SEND_AND_CLOSE;
  }
}

/*******************************************
 Função responsavel por repor as configurações
 de rede de fabrica
 Argumentos:
  entrada - mensagem HTTP recebida
  Saida - void
********************************************/
static void clean_handler(struct mg_connection *c, int ev, void* p, void* user_data){
  if (ev == MG_EV_HTTP_REQUEST) {
    clean_conf();
    mg_send_response_line(c, 200, "Content-type: application/json\r\n\r\n{\"OK\"}");
    c->flags |= MG_F_SEND_AND_CLOSE;
  }
}

/*******************************************
 Função que declara os endpoints HTTP
 Argumentos:
  entrada - void
  Saida - void
********************************************/
void init_server(){
    mgos_register_http_endpoint("/output_command.cgi", output_command_handler, NULL);
    mgos_register_http_endpoint("/input_state.shtml", input_state_handler, NULL);
    mgos_register_http_endpoint("/output_state.shtml", output_state_handler, NULL);
    mgos_register_http_endpoint("/clean_configurations.cgi", clean_handler, NULL);
}