#ifndef MSG_H_
#define MSG_H_

struct Task {
  int id;
};

struct Done {
  int id;
};

static const int MSG_PRIO = 16;
static const char* TASK_QUEUE_NAME = "/task_queue_abc123"; // should be random
static const char* DONE_QUEUE_NAME = "/done_queue_def456"; // should be random

#endif
