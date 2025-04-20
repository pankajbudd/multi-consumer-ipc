# 🧠 Flip-Flop Shared Memory IPC

This project demonstrates inter-process communication (IPC) in C++ using **POSIX shared memory**, **mutexes**, and **condition variables**. It implements a **single producer, multiple consumer** model using a **flip-flop buffer** strategy to manage string data between processes safely and efficiently.

---

## 📁 Components

### ✅ `common.h`
Defines shared constants and the `SharedMemory` structure used by all processes. It includes:
- Two flip-flop buffers (`char buffer[2][11]`)
- Synchronization primitives (`pthread_mutex_t`, `pthread_cond_t`)
- Control variables (`current_index`, `reader_count`, `version`, etc.)

---

### 🧑‍🏭 `producer.cpp`
Creates and initializes the shared memory region. It:
- Writes a random string to the shared memory.
- Switches buffers after all consumers have read the current data.
- Uses `pthread_mutex` and `pthread_cond_broadcast` for coordination.

---

### 👀 `consumer.cpp`
Each consumer:
- Connects to the shared memory region.
- Waits for new data using condition variables.
- Reads from the current buffer when notified and increments `reader_count`.

---

### 🧹 `cleanup.cpp`
Utility to clean up shared memory:
```bash
./cleanup
```
Calls `shm_unlink` to remove the shared memory object after use.

---

## 🛠️ Build Instructions

```bash
g++ -o producer producer.cpp -pthread
g++ -o consumer consumer.cpp -pthread
g++ -o cleanup cleanup.cpp -pthread
```

---

## 🚀 Run Instructions

### 1. Start the Producer
```bash
./producer
```

### 2. Start up to 2 Consumers (in separate terminals)
```bash
./consumer
```

### 3. Cleanup After Use
```bash
./cleanup
```

> ⚠️ Make sure not to exceed `MAX_CONSUMERS` (default: 2), as defined in `common.h`.

---

## 🔍 SharedMemory Layout

```cpp
struct SharedMemory {
    char buffer[2][11];        // Two alternating string buffers
    int current_index;         // Current buffer index being written/read
    int reader_count;          // Number of consumers who read the latest message
    int active_consumers;      // Total number of active consumers
    int version;               // Incremented on each write for synchronization

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
```

---

## 📌 Highlights

- 🌀 **Flip-flop buffer** ensures clean alternation between writes and reads.
- 🔐 **Thread-safe synchronization** using `pthread_mutex` and `pthread_cond`.
- 🧵 **Multiple consumers** can safely read the same data.
- 💬 **Random string generation** simulates message/data broadcast from producer.

---

## 🧪 Example Output

```
[Producer] Wrote: YyDjNvQxZL to buffer index: 0 (version: 1)
[Consumer 10012] Read: YyDjNvQxZL (ver: 1)
[Producer] Flipping buffer index to 1
[Producer] Wrote: AdGpLcVkzR to buffer index: 1 (version: 2)
...
```

---

## 📦 Possible Enhancements

- Graceful shutdown handling for producer and consumers.
- Dynamic consumer detection.
- Data logging or file output.
- Support for variable-length strings.

---

## 📄 License

This project is open-sourced for educational and demonstrative purposes. Feel free to use, modify, or extend it.

---

## 👨‍💻 Author

Made with ❤️ in C++.

---