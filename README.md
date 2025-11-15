# National Cheng Kung University 2025 Special Recruitment

This repository contains the examination environment for NCKU's 2025 Special Recruitment, implementing embedded neural network inference and algorithm benchmarking on ESP32 platforms using micro-ROS integration.

## System Overview

The software system consists of three distinct components:

- **Client**: ESP32 firmware (`src/client/`) - embedded C++ code running micro-ROS client for inference and computation tasks
- **Host**: ROS 2 Python package (`src/host/`) - publishes test data to the client via ROS 2 topics
- **Judge**: CI validation package (`ci/judge/`) - validates client outputs, measures accuracy, and stores results (read-only for examinees)

The client operates as a micro-ROS client, communicating with the ROS 2 host system through a micro-ROS agent. This architecture enables real-time inference and computation on resource-constrained hardware while maintaining integration with the broader ROS 2 ecosystem.

## Problems and Scoring

This exam consists of **3 problems** (p1, p2, and p3) worth **100 points total**.

| Problem | Points | Boards | Focus Area               |
| ------- | ------ | ------ | ------------------------ |
| 1       | 40     | 1      | MNIST + micro-ROS basics |
| 2       | 20     | 2      | Multi-board performance  |
| 3       | 40     | 3      | Algorithm porting        |

---

### Problem 1: Single Board MNIST (40 points)

**Objective**: Implement MNIST digit classification using Embedded Binary Neural Networks (EBNN) on a single ESP32 board.

#### Task Details

| Board  | Environment | Input Topic      | Input Type                     | Output Topic    | Output Type              |
| ------ | ----------- | ---------------- | ------------------------------ | --------------- | ------------------------ |
| Single | `p1`        | `/mnist_input_a` | `MnistImage` (28×28 grayscale) | `/mnist_output` | `Prediction` (digit 0-9) |

**Test Dataset**: 50 MNIST images

**Algorithm Reference**: [EBNN (Embedded Binary Neural Networks)](https://github.com/kunglab/ebnn/) for efficient MNIST digit classification on resource-constrained hardware.

#### Implementation Requirements

**Editable Files**: You are only allowed to modify the following files.

- ✏️ **Client**: `src/client/p1_ebnn_mnist/main.cpp`
- ✏️ **Host**: `src/host/src/srecruit_host/p1_ebnn_mnist.py`, `src/host/src/srecruit_host/settings.toml`
- ✏️ **Headers**: `include/ebnn.h`, `include/simple_mnist.h` (algorithm implementations from EBNN repository)

**Client Tasks** (ESP32/C++):

- Configure micro-ROS settings
- Implement message subscription and publishing
- Use provided EBNN headers for inference
- Ensure predictions are correctly published with matching IDs

**Host Tasks** (PC/Python):

- Implement ROS 2 publisher node from scratch
- Configure publishing behavior and timing
- Edit `settings.toml` to adjust publisher rate (`publisher.p1` field)

#### Scoring Breakdown

| Component                                          | Points | Criteria                                                      |
| -------------------------------------------------- | ------ | ------------------------------------------------------------- |
| (1) **ROS Setup (Domain)**                         | 5      | Successfully connect client to ROS 2 network (node_count ≥ 2) |
| (2) **Basic Function (mnist_callback and feeder)** | 15     | Receive at least one image and publish prediction             |
| (3) **Performance Optimization**                   | 20     | Process images at high speed (linear scale)                   |
| **TOTAL**                                          | **40** |                                                               |

#### More about Scores

It requires **≥40 correct predictions** out of 50 to obtain the scores.

The actual scores that you will get and the relative performance results are listed in the following table.

| Speed (img/s) | Points |     | Speed (img/s) | Points |
| ------------- | ------ | --- | ------------- | ------ |
| ≤5.0          | 0      |     | 15.0          | 16.4   |
| 7.0           | 4.6    |     | 20.0          | 17.1   |
| 9.0           | 9.1    |     | 25.0          | 17.9   |
| 11.0          | 13.7   |     | 35.0          | 19.3   |
| 12.0          | **16** |     | ≥40.0         | **20** |

---

### Problem 2: Dual Board MNIST (20 points)

**Objective**: Distribute MNIST inference across two ESP32 boards for increased throughput.

#### Task Details

| Board | Environment  | Input Topic      | Input Type                     | Output Topic    | Output Type              |
| ----- | ------------ | ---------------- | ------------------------------ | --------------- | ------------------------ |
| **A** | `p2_board_a` | `/mnist_input_a` | `MnistImage` (28×28 grayscale) | `/mnist_output` | `Prediction` (digit 0-9) |
| **B** | `p2_board_b` | `/mnist_input_b` | `MnistImage` (28×28 grayscale) | `/mnist_output` | `Prediction` (digit 0-9) |

**Test Dataset**: 50 MNIST images (distributed between both boards)

**Algorithm Reference**: [EBNN (Embedded Binary Neural Networks)](https://github.com/kunglab/ebnn/)

#### Implementation Requirements

**Editable Files**: You are only allowed to modify the following files.

- ✏️ **Client**: `src/client/p2_ebnn_mnist_dual/board_a/main.cpp`, `board_b/main.cpp`
- ✏️ **Host**: `src/host/src/srecruit_host/p2_ebnn_mnist_dual.py`, `src/host/src/srecruit_host/settings.toml`
- ✏️ **Headers**: `include/ebnn.h`, `include/simple_mnist.h` (algorithm implementations from EBNN repository)

**Client Tasks** (ESP32/C++):

- Implement two separate firmware programs for boards A and B
- Board A: Subscribe to `/mnist_input_a`, publish to `/mnist_output`
- Board B: Subscribe to `/mnist_input_b`, publish to `/mnist_output`

**Host Tasks** (PC/Python):

- Implement ROS 2 publisher node from scratch
- Distribute 50 total images between `/mnist_input_a` and `/mnist_input_b` topics
- Edit `settings.toml` to adjust publisher rate and test dual-board performance (`publisher.p2` field)

#### Scoring Breakdown

| Component                    | Points | Criteria                                        |
| ---------------------------- | ------ | ----------------------------------------------- |
| **Performance Optimization** | 20     | High-speed dual-board processing (linear scale) |

#### More about Scores

Performance points require **≥40 correct predictions** total:

The actual scores that you will get and the relative performance results are listed in the following table.

| Speed (img/s) | Points |     | Speed (img/s) | Points |
| ------------- | ------ | --- | ------------- | ------ |
| ≤20.0         | 0      |     | 45.0          | 16.5   |
| 25.0          | 4.0    |     | 50.0          | 17.0   |
| 30.0          | 8.0    |     | 60.0          | 18.0   |
| 35.0          | 12.0   |     | 70.0          | 19.0   |
| 40.0          | **16** |     | ≥80.0         | **20** |

---

### Problem 3: MiBench Triple Board (40 points)

**Objective**: Port and execute three MiBench algorithms (QSort, SHA-1, CRC32) across three ESP32 boards.

**Algorithm Reference**: [MiBench: A Free, Commercially Representative Embedded Benchmark Suite](https://vhosts.eecs.umich.edu/mibench/)

#### Task Details

| Board | Algorithm | Environment  | Input Topic    | Input Type         | Output Topic    | Output Type        | Function                                      |
| ----- | --------- | ------------ | -------------- | ------------------ | --------------- | ------------------ | --------------------------------------------- |
| **A** | QSort     | `p3_board_a` | `/qsort_input` | `QsortVertexArray` | `/qsort_output` | `QsortVertexArray` | Sort 3D vertices by distance from origin      |
| **B** | SHA-1     | `p3_board_b` | `/qsort_input` | `QsortVertexArray` | `/sha_output`   | `ShaOutput`        | Compute SHA-1 hash of serialized vertices     |
| **C** | CRC32     | `p3_board_c` | `/qsort_input` | `QsortVertexArray` | `/crc32_output` | `Crc32Output`      | Compute CRC32 checksum of serialized vertices |

**Test Dataset**: 20 vertex arrays (each algorithm processes the same 20 arrays)

> [!NOTE]
> All three boards subscribe to the **same** `/qsort_input` topic but perform different computations (sorting, hashing, checksumming).

#### Implementation Requirements

**Editable Files**:

- ✏️ **Client**: `src/client/p3_mibench/board_a/main.cpp` (QSort), `board_b/main.cpp` (SHA), `board_c/main.cpp` (CRC32)
- ✏️ **Host**: `src/host/src/srecruit_host/settings.toml` (adjust publisher rate **ONLY**)
- ✏️ **Headers**: `include/qsort.h`, `include/sha.h`, `include/crc32.h` (optional, to reduce `main.cpp` length)

> [!IMPORTANT]
> **Problem 3 Host Restriction**: The host publisher (`src/host/src/srecruit_host/p3_mibench.py`) is **pre-implemented and read-only**. You may **ONLY** edit `settings.toml` to adjust the publishing rate (`publisher.p3` field). All other host customization is prohibited.

**Client Tasks** (ESP32):

Port MiBench algorithms to work with ROS 2 messages. All boards subscribe to `/qsort_input` and receive `QsortVertexArray` messages.

**Board A: QSort**

- Subscribe to `/qsort_input`, publish to `/qsort_output`
- Sort 3D vertices (x, y, z coordinates as `int32`) by distance from origin (0, 0, 0)
- Output format: `QsortVertexArray` with sorted vertices

**Board B: SHA-1**

- Subscribe to `/qsort_input`, publish to `/sha_output`
- Compute SHA-1 hash (160-bit digest) of the input vertex data
- Serialize vertex data as little-endian byte stream
- Output format: `ShaOutput` with 20-byte digest (`uint8[20]`)

**Board C: CRC32**

- Subscribe to `/qsort_input`, publish to `/crc32_output`
- Compute CRC32 checksum (32-bit) of the input vertex data
- Serialize vertex data as little-endian byte stream
- Output format: `Crc32Output` with `uint32` checksum

**Configuration**:

- Edit `settings.toml` to adjust publisher rate and test triple-board performance

#### Scoring Breakdown

| Component                    | Points | Criteria                                       |
| ---------------------------- | ------ | ---------------------------------------------- |
| **QSort Correctness**        | 10     | Proportional to correct outputs (max 20 tests) |
| **SHA Correctness**          | 10     | Proportional to correct outputs (max 20 tests) |
| **CRC32 Correctness**        | 10     | Proportional to correct outputs (max 20 tests) |
| **Performance Optimization** | 10     | Fast execution across all tasks                |
| **TOTAL**                    | **40** |                                                |

#### More about Scores

**Scores for QSort, SHA, CRC32**

Each algorithm is scored independently.

```
algorithm_points = (correct / 20) × 10
```

Example: 18 correct QSort results = (18/20) × 10 = **9.0 points**

**Scores for Performance Optimization**

Performance points require **≥18 correct** for **ALL three algorithms**:

```
correct_per_second = total_correct / elapsed_seconds
```

| Speed (correct/s) | Points |     | Speed (correct/s) | Points |
| ----------------- | ------ | --- | ----------------- | ------ |
| ≤3.5              | 0      |     | 9.0               | 4.8    |
| 5.0               | 1.3    |     | 11.0              | 6.5    |
| 6.0               | 2.2    |     | 12.0              | 7.4    |
| 7.0               | 3.0    |     | 14.0              | 9.1    |
| 8.0               | 3.9    |     | ≥15.0             | **10** |

> [!NOTE]
> With 54-60 total correct answers, achieving ≥15 correct/s requires completion in ≤4 seconds.

---

## Quick Start Guide

### Prerequisites

#### Hardware Requirements

| Item                       | Quantity | Notes                                 |
| -------------------------- | -------- | ------------------------------------- |
| ESP32 boards (NodeMCU-32S) | 1-3      | Depends on problem (P1=1, P2=2, P3=3) |
| USB cables                 | 1-3      | One per board                         |
| Development machine        | 1        | Ubuntu 22.04 LTS recommended          |

#### Software Requirements

| Component (docs)                                                             | Purpose                            |
| ---------------------------------------------------------------------------- | ---------------------------------- |
| **Docker** — [docs](https://docs.docker.com/)                                | Start the srecruit env             |
| **Pixi** — [pixi.sh](https://pixi.sh/latest/)                                | Package manager for ROS 2 + Python |
| **PlatformIO** — [platformio.org](https://platformio.org/)                   | ESP32 firmware build system        |
| **micro-ROS agent** — [GitHub](https://github.com/micro-ROS/micro-ROS-Agent) | Serial ↔ ROS 2 bridge              |

### Start Environment in Devcontainer

0. You can first enter the command `docker rm $(docker ps -aq)` in the terminal to ensure that Docker is in its clean state.

1. Click the **Remote Explorer** icon in the bottom-left corner.

2. In the Remote Explorer panel click **Reopen in Container**.  
    ![Step 1-2](images/Step12.png)

3. Make sure `srecruit_env` appears in the Remote Explorer.

4. Wait for VS Code to download and install extensions.

5. If VS Code prompts you to restart, click **Reload**.  
    ![Step 3-5](images/Step345.png)

6. Wait until initialization and configuration complete.  
    ![Step 6](images/Step6.png)

7. To map a USB device into the container, edit `.devcontainer/devcontainer.json` and add a device entry, for example:

    `--device=/dev/ttyUSB0:/dev/ttyUSB0`

    The left path is the device on your host; the right path is the device inside the container.  
    ![Step 7](images/Step7.png)

### Running a Problem

> [!IMPORTANT]
> To use `pio` commands, you should open a PlatformIO terminal via the bottom toolbar. You can click the button number 6 to open the PlatformIO CLI Terminal.
>
> ![PlatformIO_GUI](images/PlatformIO_GUI.png)

#### Single Board (Problem 1)

**Workflow**: Flash firmware → Start agent → Run validation → View scores

```bash
# Terminal 1: Flash firmware and start agent
pio run -t upload -e p1 --upload-port /dev/ttyUSB0 && ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB0

# Terminal 2: Run validation
pixi run judge -p 1

# View scores
pixi run show-db
```

#### Multi-Board (Problems 2 & 3)

**Workflow**: Flash each board → Start agent per board → Run validation

<details>
<summary><b>Problem 2 Example (2 boards)</b></summary>

```bash
# Terminal 1: Board A
pio run -t upload -e p2_board_a --upload-port /dev/ttyUSB0 && ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB0

# Terminal 2: Board B
pio run -t upload -e p2_board_b --upload-port /dev/ttyUSB1 && ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB1

# Terminal 3: Run validation
pixi run judge -p 2
```

</details>

<details>
<summary><b>Problem 3 Example (3 boards)</b></summary>

```bash
# Terminal 1: Board A (QSort)
pio run -t upload -e p3_board_a --upload-port /dev/ttyUSB0 && ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB0

# Terminal 2: Board B (SHA)
pio run -t upload -e p3_board_b --upload-port /dev/ttyUSB1 && ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB1

# Terminal 3: Board C (CRC32)
pio run -t upload -e p3_board_c --upload-port /dev/ttyUSB2 && ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB2

# Terminal 4: Run validation
pixi run judge -p 3
```

</details>

## Client Development (ESP32 Firmware)

### Directory Structure

```
src/client/
├── p1_ebnn_mnist/          # Problem 1: Single board MNIST
│   └── main.cpp
├── p2_ebnn_mnist_dual/     # Problem 2: Dual board MNIST
│   ├── board_a/main.cpp
│   └── board_b/main.cpp
└── p3_mibench/             # Problem 3: MiBench algorithms
    ├── board_a/main.cpp    # QSort implementation
    ├── board_b/main.cpp    # SHA-1 implementation
    └── board_c/main.cpp    # CRC32 implementation
```

### PlatformIO Environments

| Environment  | Problem | Board  | Algorithm | Description                  |
| ------------ | ------- | ------ | --------- | ---------------------------- |
| `p1`         | 1       | Single | EBNN      | MNIST digit classification   |
| `p2_board_a` | 2       | A      | EBNN      | MNIST (first board)          |
| `p2_board_b` | 2       | B      | EBNN      | MNIST (second board)         |
| `p3_board_a` | 3       | A      | QSort     | Sort vertices by distance    |
| `p3_board_b` | 3       | B      | SHA-1     | Hash serialized vertices     |
| `p3_board_c` | 3       | C      | CRC32     | Checksum serialized vertices |

### Build Commands

> [!IMPORTANT]
> Always specify the environment with `-e <env>` flag to avoid building all 6 environments (slow).

#### Common Operations

| Operation           | Command                                                                                                        | Example                                                                                                                 |
| ------------------- | -------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| **Build only**      | `pio run -e <env>`                                                                                             | `pio run -e p1`                                                                                                         |
| **Build + Upload**  | `pio run -t upload -e <env> --upload-port <port>`                                                              | `pio run -t upload -e p1 --upload-port /dev/ttyUSB0`                                                                    |
| **Upload + Agent**  | `pio run -t upload -e <env> --upload-port <port> && ros2 run micro_ros_agent micro_ros_agent serial -D <port>` | `pio run -t upload -e p1 --upload-port /dev/ttyUSB0 && ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB0` |
| **Clean micro-ROS** | `pio run --target clean_microros -e <env>`                                                                     | `pio run --target clean_microros -e p1`                                                                                 |

#### Examples

```bash
# ✅ GOOD: Build only Problem 1
pio run -e p1

# ❌ BAD: Builds all 6 environments (very slow)
pio run

# ✅ RECOMMENDED: Upload and immediately start agent
pio run -t upload -e p1 --upload-port /dev/ttyUSB0 && ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB0
```

### Using VS Code PlatformIO GUI

The PlatformIO extension provides a graphical interface:

- **Environment selector**: Bottom status bar (click to select `p1`, `p2_board_a`, etc.)
- **Build button**: Bottom toolbar (hammer icon)
- **Upload button**: Bottom toolbar (arrow icon)
- **Serial monitor**: Bottom toolbar (plug icon)

> [!WARNING]
> VS Code may auto-activate the Pixi environment. Build firmware in a clean PlatformIO terminal, not in Pixi shell.

### Common Build Issues

| Issue                                | Solution                                                        |
| ------------------------------------ | --------------------------------------------------------------- |
| **micro-ROS compilation failed**     | `pio run --target clean_microros -e p1 && pio run -e p1`        |
| **Build is very slow**               | Always use `-e <env>` flag to avoid building all 6 environments |
| **Permission denied on serial port** | `sudo usermod -a -G dialout $USER` then log out/in              |
| **PlatformIO fails in Pixi**         | Use separate PlatformIO terminal (not in Pixi shell)            |

#### When to Clean micro-ROS Cache

Run `pio run --target clean_microros -e <env>` when:

- ✅ After modifying message definitions in `extra_packages/custom_msgs/`
- ✅ Encountering unexplained micro-ROS compilation errors
- ✅ After switching between different message versions

**Example**:

```bash
# Clean and rebuild Problem 1
pio run --target clean_microros -e p1
pio run -e p1
```

## Host Environment

The host runs on your development machine:

- **Host package** (`src/host/`): Publishes test data (editable)
- **Judge package** (`ci/judge/`): Validates outputs (read-only)

All dependencies are managed by Pixi.

> [!IMPORTANT]
> Do NOT build client firmware in Pixi environment - use separate PlatformIO terminal.

### Publisher Rate Configuration

Publisher rates are configured in `src/host/src/srecruit_host/settings.toml`:

```toml
[publisher]
# Problem 1: MNIST feeder node publish rate (images/second)
p1 = 11.8
# Problem 2: MNIST feeder node publish rate (images/second)
p2 = 11.8
# Problem 3: MiBench feeder node publish rate (tests/second)
p3 = 3.0
```

## Project Structure

```
├── src/
│   ├── client/                 # ESP32 firmware (C++) - EDITABLE
│   │   ├── p1_ebnn_mnist/      # Problem 1
│   │   ├── p2_ebnn_mnist_dual/ # Problem 2 (board_a, board_b)
│   │   └── p3_mibench/         # Problem 3 (board_a, board_b, board_c)
│   └── host/                   # ROS 2 publishers (Python) - EDITABLE
│       └── src/srecruit_host/
│           ├── p1_ebnn_mnist.py       # P1 publisher (implement from scratch)
│           ├── p2_ebnn_mnist_dual.py  # P2 publisher (implement from scratch)
│           ├── p3_mibench.py          # P3 publisher (read-only)
│           └── settings.toml          # Publisher rate config (editable)
├── ci/judge/                   # Validation (Python) - READ-ONLY
├── extra_packages/custom_msgs/ # Message definitions
├── include/                    # Algorithms (EBNN, QSort, SHA, CRC32)
├── platformio.ini              # Build config
└── pixi.toml                   # Dependencies
```

## ROS 2 Topics

### Problem 1: Single Board MNIST

| Topic            | Message Type | Direction      | Content               |
| ---------------- | ------------ | -------------- | --------------------- |
| `/mnist_input_a` | `MnistImage` | Host → Client  | 28×28 grayscale image |
| `/mnist_output`  | `Prediction` | Client → Judge | Predicted digit (0-9) |

### Problem 2: Dual Board MNIST

| Topic            | Message Type | Direction           | Content               |
| ---------------- | ------------ | ------------------- | --------------------- |
| `/mnist_input_a` | `MnistImage` | Host → Board A      | 28×28 grayscale image |
| `/mnist_input_b` | `MnistImage` | Host → Board B      | 28×28 grayscale image |
| `/mnist_output`  | `Prediction` | Both Boards → Judge | Predicted digit (0-9) |

### Problem 3: MiBench Triple Board

| Topic           | Message Type       | Direction         | Content                       |
| --------------- | ------------------ | ----------------- | ----------------------------- |
| `/qsort_input`  | `QsortVertexArray` | Host → All Boards | Array of 3D vertices          |
| `/qsort_output` | `QsortVertexArray` | Board A → Judge   | Sorted vertices (by distance) |
| `/sha_output`   | `ShaOutput`        | Board B → Judge   | SHA-1 hash (20 bytes)         |
| `/crc32_output` | `Crc32Output`      | Board C → Judge   | CRC32 checksum (4 bytes)      |

> [!NOTE]
> All three boards in Problem 3 subscribe to the same `/qsort_input` topic but publish to different output topics.

## micro-ROS Agent

The micro-ROS agent bridges serial/USB communication between ESP32 clients and the ROS 2 host environment.

### Installation

> [!NOTE]
> The micro-ROS agent is pre-built and sourced in the Docker environment at `/root/micro_ros_ws/`. You can use the agent commands directly without manual sourcing.

**Installation options (for non-Docker setups):**

- **Source build**: [micro-ROS setup](https://github.com/micro-ROS/micro_ros_setup)
- **Docker container**: [micro-ROS Dockers](https://github.com/micro-ROS/docker)
- **Snap package**: [Snapcraft micro-ROS agent](https://snapcraft.io/microros-agent)

### Usage

Start the agent after flashing firmware to your ESP32:

```bash
# Single board
ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB0

# Multi-board (run in separate terminals)
ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB0
ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB1
ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB2
```

> [!CAUTION]
> **DO NOT** run the agent while uploading firmware - this causes serial port conflicts. The recommended workflow (`pio run -t upload && ros2 run ...`) ensures the agent starts immediately after upload completes.

## Troubleshooting

<details>
<summary><b>Devcontainer Startup Error</b></summary>

### Problem

Devcontainer to fail to start
![startup_error](images/startup_error.png)

### Solution

1. Please first check the port-related settings in devcontainer.json.
If devcontainer.json specifies a port that does not exist, it may cause the devcontainer to fail to start.
You can use the command `ls -l /dev/tty*` on the host machine to check which ports currently exist and are available for use.
2. If you have confirmed that all the ports specified in devcontainer.json are correct, please run the command `docker rm $(docker ps -aq)` in the terminal to return Docker to a clean state.

### When to use

- After click the button `Reopen in Container` in the remote explorer panel.

</details>

<details>
<summary><b>micro-ROS Compilation Errors</b></summary>

### Problem

Compilation fails with micro-ROS errors or interface errors.

### Solution

Clean the micro-ROS cache and rebuild:

```bash
pio run --target clean_microros -e p1  # Clean cache
pio run -e p1                           # Rebuild
```

### When to use

- After modifying message definitions in `extra_packages/custom_msgs/`
- Encountering unexplained micro-ROS compilation errors
- After switching between different message versions

</details>

<details>
<summary><b>Build Performance Issues</b></summary>

### Problem

Building is very slow or builds unexpected environments.

### Solution

Always specify the environment:

```bash
# ✅ GOOD: Builds only p1
pio run -e p1

# ❌ BAD: Builds all 6 environments
pio run
```

</details>

<details>
<summary><b>Serial Port Access</b></summary>

### Problem

`Permission denied: '/dev/ttyUSB0'`

### Solution

Add your user to the `dialout` group:

```bash
sudo usermod -a -G dialout $USER
```

### Important

Log out and log back in for changes to take effect.
</details>

<details>
<summary><b>micro-ROS Agent Issues</b></summary>

### Problem

`Error: Port already in use` or agent not starting.

### Solution

Kill existing agent processes:

```bash
pkill -f "ros2 run micro_ros_agent"
```

### Reminder

Never run the agent while uploading firmware (causes serial conflicts).
</details>

<details>
<summary><b>Environment Conflicts</b></summary>

### Problem

PlatformIO cannot compile or strange build errors.

### Solution

Build firmware in a clean shell (not in Pixi environment):

- Open PlatformIO terminal from VS Code bottom toolbar, OR
- Use a fresh terminal without `pixi shell` activated

### Why

Client firmware and host code use different toolchains.
</details>

<details>
<summary><b>ROS Communication Issues</b></summary>

### Problem

Client not receiving messages from host.

### Checklist

1. ✅ Firmware uploaded successfully
2. ✅ Agent running and connected (`ros2 run micro_ros_agent micro_ros_agent serial -D /dev/ttyUSB0`)
3. ✅ Judge/host publishing messages (`pixi run judge -p 1`)
4. ✅ Topics visible: `pixi run ros2 topic list`

### Debug

Check agent output for connection status.
</details>

<details>
<summary><b>Pixi Package Errors</b></summary>

### Problem

Import errors, package not found, or unusual Pixi errors.

### Solution

Clean and reinstall:

```bash
pixi clean
pixi install
```

</details>

## References

### Platform & Framework

- [ROS 2 Humble Documentation](https://docs.ros.org/en/humble/)
- [micro-ROS](https://micro.ros.org/)
- [First micro-ROS Application Tutorial](https://micro.ros.org/docs/tutorials/core/first_application_rtos/freertos/)
- [PlatformIO](https://docs.platformio.org/en/latest/)
- [micro-ROS PlatformIO Integration](https://github.com/micro-ROS/micro_ros_platformio)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/index.html)

### Algorithms & Benchmarks

- [EBNN (Embedded Binary Neural Networks)](https://github.com/kunglab/ebnn/) - Efficient neural network inference for resource-constrained embedded systems
- [MiBench Benchmark Suite](https://vhosts.eecs.umich.edu/mibench/) - Commercially representative embedded benchmark suite (QSort, SHA, CRC32)
