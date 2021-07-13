#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#endif

#include <iostream>
#include <string>
#include <cstring>
#include <regex>
#include <cassert>
#include <sched.h>
#include <sys/mman.h>

#include <cstdio>
#include <getopt.h>

#include "bear_sdk.h"

// Settings
#define DEVICENAME "/dev/ttyUSB0"
#define DEBUGGING 0
#define REALTIME 1

void usage(char *progname) {
  printf("Usage: %s\n", progname);
  printf(" [ -h | --help ] Display this help.\n");
  printf(" [ -d | --device ] Port to open.\n");
}

int main(int argc, char *argv[]) {
  if (REALTIME) {
    struct sched_param param;
//    memset(&param, 0, sizeof(param));
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
      std::cerr << "[ BearWatch ] [ RT ] Failed in setting a real-time scheduler." << std::endl;
    } else {
      std::cout << "[ BearWatch ] [ RT ] Successfully setup a real-time scheduler!" << std::endl;
    }
    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
      std::cerr << "[ BearWatch ] [ RT ] Failed in locking memory." << std::endl;
    } else {
      std::cout << "[ BearWatch ] [ RT ] Successfully locked memory!" << std::endl;
    }
  }

  std::cout << "RoMeLa CBEAR BearWatch" << std::endl;

  char *dev_name = (char *) DEVICENAME;

  while (1) {
    int option_index{};
    int retargv{};
    static struct option long_options[] = {
        {"h", no_argument, 0, 0},
        {"help", no_argument, 0, 0},
        {"d", required_argument, 0, 0},
        {"device", required_argument, 0, 0},
        {0, 0, 0, 0}
    };

    retargv = getopt_long_only(argc, argv, "", long_options, &option_index);

    if (retargv == -1) break; // All command line options have been parsed

    if (retargv == '?') { // If arguments are missing
      usage(argv[0]);
      return 0;
    }

    switch (option_index) {
      case 0:
      case 1:usage(argv[0]);
        return 0;
        break;
      case 2:
      case 3:
        if (strlen(optarg) == 1) {
          char tmp[20];
          sprintf(tmp, "/dev/ttyUSB%s", optarg);
          dev_name = strdup(tmp);
        } else
          dev_name = strdup(optarg);
        break;
      default:usage(argv[0]);
        return 0;
    }
  }

//  // Initialize BEAR instance
//  bear::PacketManager packetManager = bear::PacketManager();
//
//  // Get the port
//  bear::PortManager portManager = bear::PortManager(dev_name, 8000000);
//
//  if (!DEBUGGING) {
//    // Open port
//    if (portManager.OpenPort()) {
//      printf("Success! Port opened!\n");
//      printf(" - Device Name: %s\n", dev_name);
//      printf(" - Baudrate: %d\n\n", portManager.GetBaudRate());
//    } else {
//      printf("Failed to open port! [%s]\n", dev_name);
//      printf("Exiting...\n");
//      return 0;
//    }
//  }

  // Initialize BEAR instance
  bear::BEAR bear_handle = bear::BEAR(dev_name, 8000000);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
  while(1) {
//    int ret_val{0};
//    ret_val = bear_handle.ping(1);
//    std::cout << "Ping Result: " << ret_val << std::endl;

    float ret_val{0.0};
    ret_val = bear_handle.GetPresentPosition(2);
    std::cout << "Present Position: " << ret_val << std::endl;
  }
#pragma clang diagnostic pop

}