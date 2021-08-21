
class MyCircularQueue {
private:
  // use circular buffer as internal implementation
  int *arr{nullptr};
  int front{0};
  int rear{0};
  int len{0};

  int size() {
    return (rear + len - front) % len;
  }

  int advance(int index) {
    if (index < len - 1) {
      return index + 1;
    } else {
      return 0;
    }
  }

public:
  MyCircularQueue(int k) {
    len = k + 1;
    arr = new int[len];
  }
    
  bool enQueue(int value) {
    if (isFull()) {
      return false;
    }

    arr[rear] = value;
    rear = advance(rear);
    return true;
  }
    
  bool deQueue() {
    if (isEmpty()) {
      return false;
    }

    front = advance(front);
    return true;
  }
    
  int Front() {
    if (isEmpty()) {
      return -1;
    }
    return arr[front];
  }
    
  int Rear() {
    if (isEmpty()) {
      return -1;
    }
    if (rear == 0)
      return arr[len - 1];
    else
      return arr[rear - 1];
  }
    
  bool isEmpty() {
    return front == rear;
  }
    
  bool isFull() {
    return size() == len - 1;
  }
};
