#include "worker.h"
#include "msg.h"

#include <cstdio>
#include <iostream>

#include <fcntl.h>
#include <mqueue.h>
#include <sys/epoll.h>

using namespace std;

void worker() {
  mqd_t task = mq_open(TASK_QUEUE_NAME, O_RDONLY);
  mqd_t done = mq_open(DONE_QUEUE_NAME, O_WRONLY);

  int taskEpoll = epoll_create1(0);
  if (taskEpoll == -1) {
    cout << "can not create task epoll fd" << endl;
    return;
  }
  
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = task;
  if (epoll_ctl(taskEpoll, EPOLL_CTL_ADD, task, &ev) == -1) {
    cout << "can not add task mq to epoll ctl" << endl;
    return;
  }

  struct epoll_event events;
  while (true) {
    int fd = epoll_wait(taskEpoll, &events, 1, -1);
    if (fd == -1) {
      perror("epoll_wait");
      return;
    }

    // consume only one task
    Task t{0};
    unsigned int prio{0};
    mq_receive(task, reinterpret_cast<char*>(&t), sizeof(Task), &prio);

    if (t.id == -1) {
      exit(0);
      break;
    }
    cout << "WORKER: do the task " << t.id << endl;

    Done d{t.id};
    mq_send(done, reinterpret_cast<const char*>(&d), sizeof(Done), 10);
  }

  mq_close(task);
  mq_close(done);
}
