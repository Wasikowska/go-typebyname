#include "master.h"
#include "msg.h"
#include <iostream>
#include <vector>

using namespace std;

class NetworkConfig {
public:
  static NetworkConfig* GetInstance() {
    static NetworkConfig config;
    return &config;
  }

  int GetNumOfSalve() {
    return 2;
  }

};

void master() {
  NetworkConfig* config = NetworkConfig::GetInstance();

  if (config->GetNumOfSalve() == 1) {
    // give all tasks to the only slave    
  } else {
    // give the tasks repeatly
  }

  cout << "master" << std::endl;
}
