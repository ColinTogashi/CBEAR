#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#endif

#include <iostream>
#include <string>
#include <cstring>
#include <regex>
#include <cassert>

#include <cstdio>
#include <getopt.h>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include "bear_sdk.h"

// Settings
#define DEVICENAME "/dev/ttyUSB0"
#define DEBUGGING 1

int getch() {
  struct termios tOld, tNew;
  int retval;
  tcgetattr(STDIN_FILENO, &tOld);
  tNew = tOld;
  tNew.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &tNew);
  retval = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &tOld);
  return retval;
}

void usage(char *progname) {
  printf("Usage: %s\n", progname);
  printf(" [ -h | --help ] Display this help.\n");
  printf(" [ -d | --device ] Port to open.\n");
}

uint32_t floatToUint32(float input) {
  static_assert(sizeof(float) == sizeof(uint32_t), "Float and uint32 are not the same size in your setup!");
  auto *pInt = reinterpret_cast<uint32_t *>(&input);
  return *pInt;
}

int main(int argc, char *argv[]) {
  // Initialize BEAR instance
  bear::PacketManager packetManager = bear::PacketManager();

  std::cout << "RoMeLa CBEAR BearWatch!" << std::endl;

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

  // Get the port
  bear::PortManager portManager = bear::PortManager(dev_name, 8000000);

  if (!DEBUGGING) {
    // Open port
    if (portManager.OpenPort()) {
      printf("Success! Port opened!\n");
      printf(" - Device Name: %s\n", dev_name);
      printf(" - Baudrate: %d\n\n", portManager.GetBaudRate());
    } else {
      printf("Failed to open port! [%s]\n", dev_name);
      printf("Press any key to terminate.\n");
      getch();
      return 0;
    }
  }

  std::string input;
//    char input[128];
  char cmd[80];
  char param[20][30];
  std::vector<std::string>::size_type num_param;
  char *token;
  uint8_t bear_error;
  uint32_t read_val_u32; // Used for storing read register values
  float read_val_f32;
  std::regex reg("\\s+");

  while (1) {
    std::cout << "[CMD] ";
    std::getline(std::cin, input);
//        printf("[CMD] ");
//        fgets(input, sizeof(input), stdin);

//        char *p;
//        if ((p == strchr(input, '\n')) != NULL) *p = '\0';
//        fflush(stdin);
    tcflush(0, TCIFLUSH); // TODO: Check to make sure this works well

    if (input.length() == 0) {
      std::cout << "Please input a command." << std::endl;
      continue;
    }

    std::sregex_token_iterator iter(input.begin(), input.end(), reg, -1);
    std::sregex_token_iterator end;
    std::vector<std::string> vec(iter, end);
//        token = strtok(input, " ");

//        num_param = 0;

    num_param = vec.size();

//        while (token != 0)
//        {
//            strcpy(param[num_param++], token);
//            token = strtok(0, " ");
//        }

    if (vec[0] == "help" || vec[0] == "h" || vec[0] == "?") {
      printf("Help manual not yet implemented.\n");
    } else if (vec[0] == "baud") {
      if (num_param == 2) // TODO: Make this 3 ?
      {
        if (portManager.SetBaudRate(std::stoi(vec[1])) == false)
          std::cerr << "[ BearWatch ] Failed to change baudrate!" << std::endl;
        else
          std::cerr << "[ BearWatch ] Baudrate successfully changed to: " << std::stoi(vec[1]) << std::endl;
      } else {
        std::cerr << "[ BearWatch ] Invalid parameters, try again or type \"help\"." << std::endl;
        continue;
      }
    } else if (vec[0] == "ping") {
      if (num_param == 1) {
        std::cerr << "[ BearWatch ] Invalid PING attempt! Try again or type \"help\"." << std::endl;
        continue;
      }

      std::cerr << "Pinging motor..." << std::endl;

      for (int idx = 1; idx < num_param; idx++) {
        if (packetManager.ping(&portManager, atoi(vec[idx].c_str()), &bear_error)
            == COMM_SUCCESS) // TODO: Convert back C-string to string
          std::cerr << "[ BearWatch ] PING SUCCESS for motor ID: " << std::stoi(vec[idx]) << std::endl;
        else
          std::cerr << "[ BearWatch ] FAILED PING for motor ID: " << std::stoi(vec[idx]) << std::endl;
      }
    } else if (vec[0] == "scan") {
      if (num_param > 1) {
        std::cerr << "[ BearWatch ] SCAN takes one parameter only! Try again or type \"help\"." << std::endl;
      } else {
        std::cerr << "[ BearWatch ] Scanning for connected BEAR modules ..." << std::endl;

        for (int id = 1; id < 253; id++) {
          if (packetManager.ping(&portManager, id, &bear_error) == COMM_SUCCESS)
            std::cerr << "\t\tDETECTED | ID: " << id << std::endl;
//                    else
//                        std::cerr << "\tfailed | ID: " << id << std::endl; // Likely don't need to notify fails also.
        }
        // TODO: Report found total number of motors.
      }
    } else if (vec[0] == "save") {
      if (num_param > 2) {
        std::cerr << "[ BearWatch ] Save takes only one parameter!" << std::endl;
      } else {
        if (packetManager.save(&portManager, std::stoi(vec[1]), &bear_error) == COMM_SUCCESS)
          std::cerr << "[ BearWatch ] Successfully saved configurations to the flash memory for motor ID: "
                    << std::stoi(vec[1]) << std::endl;
        else
          std::cerr << "[ BearWatch ] Failed to save the configurations for motor ID: " << std::stoi(vec[1])
                    << std::endl;
      }
    } else if (vec[0] == "write") // write ID TYPE ADDR VALUE
    {
      if (num_param == 5) {
        if (vec[2] == "c") {
          if ((std::stoi(vec[3]) > 2) && (packetManager.WriteConfigRegister(&portManager,
                                                                            std::stoi(vec[1]),
                                                                            std::stoi(vec[3]),
                                                                            floatToUint32(std::stof(vec[4].c_str())),
                                                                            &bear_error) == COMM_SUCCESS))
            std::cerr << "[ BearWatch ] Succesfully wrote the byte!" << std::endl;
          else if ((std::stoi(vec[3]) < 3) && (packetManager.WriteConfigRegister(&portManager,
                                                                                 std::stoi(vec[1]),
                                                                                 std::stoi(vec[3]),
                                                                                 std::stoi(vec[4]),
                                                                                 &bear_error) == COMM_SUCCESS))
            std::cerr << "[ BearWatch ] Succesfully wrote the byte!" << std::endl;
          else
            std::cerr << "[ BearWatch ] Failed to write the byte!" << std::endl;
        } else if (vec[2] == "s") {
          if ((std::stoi(vec[3]) > 1 && std::stoi(vec[3]) < 14) && (packetManager.WriteStatusRegister(&portManager,
                                                                                                      std::stoi(vec[1]),
                                                                                                      std::stoi(vec[3]),
                                                                                                      floatToUint32(std::stof(
                                                                                                          vec[4].c_str())),
                                                                                                      &bear_error)
              == COMM_SUCCESS))
            std::cerr << "[ BearWatch ] Succesfully wrote the byte!" << std::endl;
          else if ((std::stoi(vec[3]) < 2 || (std::stoi(vec[3]) >= 14 && std::stoi(vec[3]) <= 15))
              && (packetManager.WriteStatusRegister(&portManager,
                                                    std::stoi(vec[1]),
                                                    std::stoi(vec[3]),
                                                    std::stoi(vec[4]),
                                                    &bear_error) == COMM_SUCCESS))
            std::cerr << "[ BearWatch ] Succesfully wrote the byte!" << std::endl;
          else
            std::cerr << "[ BearWatch ] Failed to write the byte!" << std::endl;
        } else {
          std::cerr << "[ BearWatch ] Select either \"s\" or \"c\" as the type of register to write to." << std::endl;
        }

      } else {
        std::cerr << "[ BearWatch ] Incorrect number of parameters specified for writing a byte." << std::endl;
      }
    } else if (vec[0] == "read") // read ID TYPE ADDR
    {
      if (vec[2] == "c") {
        if ((std::stoi(vec[3]) < 3) && (packetManager.ReadConfigRegister(&portManager,
                                                                         std::stoi(vec[1]),
                                                                         std::stoi(vec[3]),
                                                                         &read_val_u32,
                                                                         &bear_error) == COMM_SUCCESS))
          std::cerr << "[ BearWatch ] Reading from configuration register: " << read_val_u32 << std::endl;
        else if ((std::stoi(vec[3]) > 2) && (packetManager.ReadConfigRegister(&portManager,
                                                                              std::stoi(vec[1]),
                                                                              std::stoi(vec[3]),
                                                                              &read_val_f32,
                                                                              &bear_error) == COMM_SUCCESS))
          std::cerr << "[ BearWatch ] Reading from configuration register: " << read_val_f32 << std::endl;
        else
          std::cerr << "[ BearWatch ] Failed to read from configuration register." << std::endl;
      } else if (vec[2] == "s") {
        if ((std::stoi(vec[3]) < 2) && (packetManager.ReadStatusRegister(&portManager,
                                                                         std::stoi(vec[1]),
                                                                         std::stoi(vec[3]),
                                                                         &read_val_u32,
                                                                         &bear_error) == COMM_SUCCESS))
          std::cerr << "[ BearWatch ] Reading from status register: " << read_val_u32 << std::endl;
        else if ((std::stoi(vec[3]) > 1) && (packetManager.ReadStatusRegister(&portManager,
                                                                              std::stoi(vec[1]),
                                                                              std::stoi(vec[3]),
                                                                              &read_val_f32,
                                                                              &bear_error) == COMM_SUCCESS))
          std::cerr << "[ BearWatch ] Reading from status register: " << read_val_f32 << std::endl;
        else
          std::cerr << "[ BearWatch ] Failed to read from status register." << std::endl;
      } else {
        std::cerr << "[ BearWatch ] Select either \"s\" or \"c\" as the type of register to read from." << std::endl;
      }
    } else {
      std::cerr << "[ BearWatch ] Invalid command, retry or type 'help'." << std::endl;
    }

    // To prevent annoying highlighting in CLion
    std::cout << "DELETE NOT DELETED!" << std::endl;
    break; // Delete later!
  }

}