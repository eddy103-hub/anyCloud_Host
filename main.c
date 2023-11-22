#include "DAVE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app.h"
#include "at_commander.h"

int main(void) {


    DAVE_STATUS_t status;

    status = DAVE_Init(); /* Initialization of DAVE APPs */

    if (status != DAVE_STATUS_SUCCESS) {
        /* Placeholder for error handler code. The while loop below can be replaced with a user error handler. */
        XMC_DEBUG("DAVE APPs initialization failed\n");

        while (1U) {
        }
    }


    while (1U) {

    	ATCMD_Task();

    }
}
