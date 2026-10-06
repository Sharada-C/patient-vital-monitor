# Patient Vital-Signs Monitor in C

A multithreaded C simulation of a bedside patient vital-sign monitoring system.

The system simulates multiple sensors producing patient vital readings concurrently, processes those readings through a bounded ring buffer, evaluates alarm conditions, and logs the results to a file.

> **Note:** This is a software simulation for learning and engineering demonstration purposes. It is not a medical device and must not be used for clinical decisions.

---

## Features

- Multithreaded sensor simulation using POSIX threads
- Three concurrent sensor producers:
  - Heart rate
  - SpO2
  - Temperature
- Bounded FIFO ring buffer for producer-consumer communication
- Mutex and condition-variable synchronization
- Atomic shutdown state
- SIGINT (`Ctrl+C`) graceful shutdown
- Rule-based alarm evaluation
- Timestamped monitoring logs
- Partial thread-creation error handling
- CMake build system
- Automated tests using CTest
- AddressSanitizer and UndefinedBehaviorSanitizer validation

---

## System Architecture


              ┌─────────────────────┐
              │ Heart Rate Sensor   |
              │ Producer Thread     │
              └──────────┬──────────┘
                         │
                         │
              ┌──────────▼──────────┐
              │                     │
              │   Bounded Ring      │
              │      Buffer         │
              │                     │
              └──────────┬──────────┘
                         │
                         │
              ┌──────────▼──────────┐
              │ Processor Thread    │
              │                     │
              │ Alarm Evaluation    │
              │ Logging             │
              └─────────────────────┘
                         ▲
                         │
        ┌────────────────┼────────────────┐
        │                │                │
   SpO2 Sensor     Temperature Sensor   HR Sensor
   Producer        Producer             Producer
