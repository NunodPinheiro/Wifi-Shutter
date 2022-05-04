#include <stdio.h>
#include "mgos.h"
#include <stdlib.h>
#include "cgi.h"
#include "output.h"
#include "input.h"
#include "mgos_config.h"



enum mgos_app_init_result mgos_app_init(void) 
{
    init_inputs();
    init_outputs();
    init_server();

    return MGOS_APP_INIT_SUCCESS;
}