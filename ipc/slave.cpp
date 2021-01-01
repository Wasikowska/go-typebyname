#include "slave.h"
#include "msg.h"
#include "worker.h"
\
#include <iostream>
#include <stdio.h>

#include <asm-generic/errno-base.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/epoll.h>

using namespace std;

void slave() {
  // create message queue
  mq_attr attr = {0, 1024, sizeof(Task), 0};
  // 0700 is not the right permission...
  mqd_t task =
      mq_open(TASK_QUEUE_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRWXU, &attr);
  attr = {0, 1024, sizeof(Done), 0};
  mqd_t done = mq_open(DONE_QUEUE_NAME, O_CREAT | O_RDONLY, S_IRUSR, &attr);

  if (task == ((mqd_t)-1)) {
    if (errno == EEXIST) {
      cout << "message queue exist!!!, retry another name" << endl;
      // should try other random name
      return;
    } else {
      perror("can not use 'mq_open' to create mq");
      return;
    }
  }

  // generate tasks
  int taskNum{17};
  for (int i = 0; i < taskNum; i++) {
    Task t = {i};
    if (mq_send(task, reinterpret_cast<const char *>(&t), sizeof(Task), 8) ==
        -1) {
      cout << "mq_send error : " << errno << endl;
      return;
    }
  }

  // start the workers
  int workerNum{7};
  for (int i = 0; i < workerNum; i++) {
    pid_t pid = fork();
    if (pid == -1) {
      cout << "can not create child process!" << endl;
      return;
    } else if (pid == 0) {
      worker();
      break;
    }
  }

  int doneNum{0};

  int doneEpoll = epoll_create1(0);
  if (doneEpoll == -1) {
    cout << "can not create done epoll fd" << endl;
    return;
  }
  
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = done;
  if (epoll_ctl(doneEpoll, EPOLL_CTL_ADD, done, &ev) == -1) {
    perror("can not add done mq to epoll ctl");
    return;
  }

  struct epoll_event events;
  while (true) {
    int fd = epoll_wait(doneEpoll, &events, 1, -1);
    if (fd == -1) {
      perror("epoll_wait");
      return;
    }

    Done d{0};
    unsigned int prio{0};
    mq_receive(done, reinterpret_cast<char*>(&d), sizeof(Done), &prio);

    doneNum++;
    if (doneNum == taskNum) {
      cout << "SLAVE: the job is done !!! terminate each worker" << endl;
      for (int i = 0; i < workerNum; i++) {
	Task t = {-1};
        mq_send(task, reinterpret_cast<const char *>(&t), sizeof(Task), 8);
      }
      
      break;
    }
  }

  mq_close(task);
  mq_close(done);
  mq_unlink(TASK_QUEUE_NAME);
  mq_unlink(DONE_QUEUE_NAME);
}
