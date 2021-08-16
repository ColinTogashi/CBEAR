#ifdef ENABLE_DOCTEST_IN_LIBRARY
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#endif

#include <iostream>
#include <cstring>
#include <sched.h>
#include <sys/mman.h>

#include <chrono>
#include <thread>

#include <cstdio>
#include <getopt.h>

#include "bear_sdk.h"
#include "bear_macro.h"
#include "utils/loop_time_stats.h"

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

  // Initialize BEAR instance
  bear::BEAR bear_handle = bear::BEAR(dev_name, 8000000);

  // Setup timing
  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;

  auto time_start = high_resolution_clock::now();

  /*
  loop_time_stats l("stats_artemis_all_motors.txt",loop_time_stats::output_mode::fileout_only);
  for (int idx = 0; idx < 100000; idx++) {
    l.loop_starting_point();

    float ret_val{0.0};

    auto t1 = high_resolution_clock::now();
    ret_val = _bear_handle.GetPresentPosition(2);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    float freq;
    freq = 1000 / ms_double.count();

    std::cout << "--------------------" << std::endl;
    std::cout << "Elapsed Time in ms: " << ms_double.count() << std::endl;
    std::cout << "Frequency: " << freq << std::endl;
  }
  l.store_loop_time_stats();
  // */

  /*
  loop_time_stats l("stats_artemis_all_motors_no_print.txt",loop_time_stats::output_mode::fileout_only);

  std::vector<uint8_t> _mIDs{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 65, 66};
  std::vector<uint8_t> _write_add{bear_macro::GOAL_VELOCITY, bear_macro::GOAL_POSITION};
  std::vector<uint8_t> _read_add{bear_macro::PRESENT_POSITION, bear_macro::PRESENT_VELOCITY, bear_macro::PRESENT_IQ};

  for (int idx = 0; idx < 100000; idx++) {
    l.loop_starting_point();
//    auto t1 = high_resolution_clock::now();

    std::vector<std::vector<float>> data{{0.3, 0.5},
                                         {0.6, 0.7},
                                         {0.3, 0.4},
                                         {0.5, 0.6},
                                         {0.3, 0.4},
                                         {0.2, 0.2},
                                         {0.4, 0.2},
                                         {0.6, 0.5},
                                         {0.4, 0.7},
                                         {0.4, 0.8},
                                         {0.0, 0.0},
                                         {0.0, 0.0}};
    std::vector<std::vector<float>> ret_vec_rw;
    ret_vec_rw = _bear_handle.BulkReadWrite(_mIDs, _read_add, _write_add, data);

//    auto t2 = high_resolution_clock::now();
//    duration<double, std::milli> ms_double = t2 - t1;
//    float freq;
//    freq = 1000 / ms_double.count();
//
//    std::cout << "--------------------" << std::endl;
//    std::cout << "Elapsed Time in ms: " << ms_double.count() << std::endl;
//    std::cout << "Frequency: " << freq << std::endl;
  }
  l.store_loop_time_stats();
//   */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
  while (1) {
    std::vector<uint8_t> mIDs{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 65, 66};
    std::vector<uint8_t> write_add{bear_macro::GOAL_VELOCITY, bear_macro::GOAL_POSITION};
    std::vector<uint8_t> read_add{bear_macro::PRESENT_POSITION, bear_macro::PRESENT_VELOCITY, bear_macro::PRESENT_IQ};
    std::vector<std::vector<float>> data{{0.0, 0.0},
                                         {0.0, 0.0},
                                         {0.0, 0.0},
                                         {0.0, 0.0},
                                         {0.0, 0.0},
                                         {0.0, 0.0},
                                         {0.0, 0.0},
                                         {0.0, 0.0},
                                         {0.0, 0.0},
                                         {0.0, 0.0},
                                         {0.0, 0.0},
                                         {0.0, 0.0}};

//    std::vector<uint8_t> _mIDs{1, 2};
//    std::vector<uint8_t> _write_add{bear_macro::GOAL_VELOCITY, bear_macro::GOAL_POSITION};
//    std::vector<uint8_t> _read_add{bear_macro::PRESENT_POSITION, bear_macro::PRESENT_VELOCITY, bear_macro::INPUT_VOLTAGE};
//    std::vector<std::vector<float>> data{{0.3, 0.5},
//                                         {0.6, 0.7}};

    std::vector<std::vector<float>> ret_vec_rw;
    auto t1 = high_resolution_clock::now();
//    bool write_status;
//    write_status = _bear_handle.BulkWrite(_mIDs, _write_add, data);

//    std::vector<std::vector<float>> ret_vec;
//    ret_vec = _bear_handle.BulkRead(_mIDs, _read_add);
    ret_vec_rw = bear_handle.BulkReadWrite(mIDs, read_add, write_add, data);

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;



//    float ret_val{0.0};
//    auto t1 = high_resolution_clock::now();
//    ret_val = _bear_handle.GetPresentPosition(2);
//    auto t2 = high_resolution_clock::now();
//    duration<double, std::milli> ms_double = t2 - t1;

    float freq;
    freq = 1000 / ms_double.count();

    std::cout << "Return Nested Vector: " << std::endl;
    for (const auto &out_vec: ret_vec_rw) {
      for (const auto &in_vec: out_vec) {
        std::cout << in_vec << "\t";
      }
      std::cout << std::endl;
    }
//    std::cout << "Present Position: " << ret_val << std::endl;
    std::cout << "Elapsed Time in ms:\t" << ms_double.count() << std::endl;
    std::cout << "Frequency:\t\t" << freq << std::endl;
    std::cout << "Run Time [s]: \t\t" << (t2-time_start).count()/1000000000. << std::endl;
    std::cout << "--------------------" << std::endl;
//    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "\033[2J\033[1;1H";
  }
#pragma clang diagnostic pop

}
