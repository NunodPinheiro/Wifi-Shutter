#include <stdio.h>
#include "mgos.h"
#include <stdlib.h>
#include "discovery.h"
#include "mgos_config.h"
#include "def.h"

/********************************************************
 Função que converte uma string com informação ip para um
array de inteiros com informação ip.
 Argumentos:
  - Entrada - string com uma informação ip (ip, gateway ou
              netmask)
            - Array de inteiros de 4 posições para guardar
              a informação ip  
 ********************************************************/
void get_ip_from_str_udp(char* str, int* ip) {
  unsigned int i;
  int flag = 0;
  ip[0] = 0;
  ip[1] = 0;
  ip[2] = 0;
  ip[3] = 0;
  for (i = 0; i<strlen(str);i++) {
    if (str[i] >= '0' && str[i]<= '9') {              
      ip[flag] = ip[flag] * 10 + (int)(str[i]-'0');
    } else if (str[i] == '.') {
      flag ++;
    }  
  }    
}

/********************************************************
 Função responsável por obter as informações de rede
 ********************************************************/

void get_network(){
  char gw[20];
  char netmask[22];
  char sta_ip[16];
  struct mgos_net_ip_info ip_info;  memset(&ip_info, 0, sizeof(ip_info));
  memset(sta_ip, 0, sizeof(sta_ip));
  memset(gw, 0, sizeof(gw));
  memset(netmask, 0, sizeof(netmask));
  if (mgos_net_get_ip_info(MGOS_NET_IF_TYPE_WIFI, MGOS_NET_IF_WIFI_STA, &ip_info)) {
    mgos_net_ip_to_str(&ip_info.ip, sta_ip);
    get_ip_from_str_udp(sta_ip, network.ip);
    mgos_net_ip_to_str(&ip_info.gw, gw);
    get_ip_from_str_udp(gw, network.gtw);
    mgos_net_ip_to_str(&ip_info.netmask, netmask);
    get_ip_from_str_udp(netmask, network.sm);
  }
}

/********************************************************
 Função responsavel por obter o mac address
 ********************************************************/
void get_mac_address (char * mac_str) {
  uint8_t mac_uin8t[6];
  device_get_mac_address(mac_uin8t);                  
  sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", mac_uin8t[0]&0xFF, mac_uin8t[1]&0xFF, mac_uin8t[2]&0xFF, mac_uin8t[3]&0xFF, mac_uin8t[4]&0xFF, mac_uin8t[5]&0xFF);    
}

/********************************************************
Callback para receção do multicast UDP para a descoberta
do modulo na rede. Quando recebe "IOTBRAGACHALLENGE" 
devolve os dados de rede e mac address
Argumentos
  - Estrutura mongoose com os dados comunicados
  - Tipo de evento
  - Não utilizado
  - Argumento para retornar dados por referência
 ********************************************************/
void udp_handler(struct mg_connection *nc, int ev, void *p, void *user_data) {
  struct mbuf *io = &nc->recv_mbuf;
  (void) p;
  switch (ev) {
    case MG_EV_RECV: {
      char mac_str[18];
      char msg[161];
      //uint8_t macAddress[18];
      get_mac_address(mac_str);
      get_network();

      if(strcmp(io->buf, "IOTBRAGACHALLENGE")) {
        mbuf_remove(io, io->len); 
        sprintf(msg,"{\"IP\":\"%d.%d.%d.%d\",\"GW\":\"%d.%d.%d.%d\",\"SM\":\"%d.%d.%d.%d\",\"MAC\":\"%s\",\"FW\":\"1.0.0\",\"HW\":\"1.0.0\",\"DHCP\":%d}",
        network.ip[0], network.ip[1], network.ip[2], network.ip[3],
        network.gtw[0],network.gtw[1],network.gtw[2],network.gtw[3],
        network.sm[0],network.sm[1],network.sm[2],network.sm[3],
        mac_str, network.dhcp);
        mg_send(nc, msg, strlen(msg));
      }
      nc->flags |= MG_F_SEND_AND_CLOSE;
      break;
    } 
  }
  (void) user_data;
}

/********************************************************
 Função aonde se declara as callback para uma comunicação
 ********************************************************/
int init_listener(struct mg_mgr *mgr) {
  struct mg_bind_opts bopts;
  char listener_spec[128];
  snprintf(listener_spec, sizeof(listener_spec), "udp://:8654");
  memset(&bopts, 0, sizeof(bopts));
  struct mg_connection *lc = mg_bind_opt(mgr, listener_spec, udp_handler, NULL, bopts);
  if (lc == NULL) {
    return 0;
  }

  return 1;
}

/********************************************************
 Função aonde se limpa as configurações de rede criadas
 neste caso o ssid e password da rede ficando outra vez
 em modo AP
 ********************************************************/
void clean_conf(){
  char *err = NULL;

  mgos_sys_config_set_wifi_sta_enable(false);
  mgos_sys_config_set_wifi_ap_enable(true);
  save_cfg(&mgos_sys_config, &err); /* Writes conf9.json */
  printf("Saving configuration: %s\n", err ? err : "no error");
  free(err);
  
  mgos_system_restart_after(100);
}

void init_discovery(){
    init_listener(mgos_get_mgr());
}