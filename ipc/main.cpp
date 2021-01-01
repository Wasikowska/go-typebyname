#include <iostream>
#include <string>

#include "master.h"
#include "slave.h"
#include "worker.h"

using namespace std;

// this is a draft about parallel compilation, in a distributed
// environment.
//
// the computing network should be configured staticly. the default
// setting is that there's only one machine node in the network, and
// everying happens in that node.
//
// logiclly, there're two kinds of machine node in the network: the
// master and the slave. the master distribute tasks to each
// slave. and the slave node would spawn many workers to do the
// job. each worker is a process that do the compilation, both
// compiling and linking. in summary, we have a static computing
// network, while the master node generates tasks and distributes the
// taks to each slave nodes; a salve node controls all its worker
// process, and do the job.
//
// the master mode controls the whole building process. the tasks have
// two kinds:
// 1. compilation work: compile a module
// 2. linking work: link some object file
//
// the master and slave communicates through network, and the slave
// and workers communicates through local message queues

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cout << "error!" << std::endl;
    return 1;
  }

  string role(argv[1]);
  if (role == "master") {
    master();
  } else if (role == "slave") {
    slave();
  } else if (role == "worker") {
    worker();
  }

  return 0;
}
