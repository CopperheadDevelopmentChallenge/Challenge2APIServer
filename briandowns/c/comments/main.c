#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include <ulfius.h> 

#include "api.h"
#include "log.h"
#include "store.h"

#define STR1(x) #x
#define STR(x) STR1(x)

#define VERSION      "0.1"
#define DEFAULT_PORT 8080
#define DATA_FILE    "data.json"

int control = 0;
int port = DEFAULT_PORT;

static char usage[] = "usage: %s [-pvh] -p [port]\n \
    -p\tport for server to listen on\n \
    -v\tversion\n \
    -h\thelp\n";

/**
 * sig_handler captures ctrl-c
 */
static void sig_handler(int dummy) {
    control = 1;
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    signal(SIGINT, sig_handler);
    int opt;

    while ((opt = getopt(argc, argv, "p:vh")) != -1) {
        switch (opt) {
        case 'p':
            if (strlen(optarg) < 1) exit(EXIT_FAILURE);
            port = atoi(optarg);
            break;
        case 'v':
            printf("version %s\n", VERSION);
            exit(EXIT_SUCCESS);
        case 'h':
            fprintf(stderr, usage, argv[0]);
            exit(EXIT_SUCCESS);
        default:
            fprintf(stderr, usage, argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    store = store_new("data.json");
    if (store == NULL)  {
        return EXIT_FAILURE;
    }

    struct _u_instance instance;

    if (ulfius_init_instance(&instance, port, NULL, NULL) != U_OK) {
        log_json("error", "unable to intialize server");
        return EXIT_FAILURE;
    } 

    if (api_add_routes(&instance) != 0) {
        log_json("error", "unable to add routes");
        return EXIT_FAILURE;
    }

    if (ulfius_start_framework(&instance) == U_OK) {
        char lp[29];
        sprintf(lp, "starting framework on port %d", instance.port);
        log_json("info", lp);

        getchar();
    } else {
        log_json("error", "error starting framework");
    }

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    return EXIT_SUCCESS;
}
