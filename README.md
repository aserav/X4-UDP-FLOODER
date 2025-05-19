# 🔥 X4 UDP Flooder v4.0

A high-performance, multithreaded UDP flood testing tool written in C++ for Linux. Designed for stress testing and bandwidth benchmarking in **controlled, authorized environments** only.

---

## 🚀 Features

- 🧵 **Multithreaded Engine** – High-speed parallel packet sending
- 📊 **Live Stats**:
  - Real-time Mbps & average speed
  - Total MB sent
  - Ping latency (ms) or timeout
  - Elapsed & remaining time
- 🧠 **Built-in ICMP ping** before and during the attack
- 📄 **Optional detailed logging** (`--log`)
- ⚙️ **Custom packet size** support via `--packet-size=SIZE`
- 🌐 Supports **IPv4 and domain names**
- 📦 Graceful `Ctrl+C` handling with cleanup, final stats & log save

---

## 💻 Usage

```bash
./x4 <IP/Domain> <PORT> <THREADS> <DURATION> [--packet-size=SIZE] [--log]
```

### Example:
```bash
./x4 8.8.8.8 53 100 60 --packet-size=1024 --log
```

---

## 📥 Installation (Linux Only)

### Requirements:
- Linux-based OS
- g++ compiler (`sudo apt install g++`)
- Root privileges (required for ping support)

### Compile:

```bash
g++ x4.cpp -o x4 -lpthread
```

(Optional: Move to system path)

```bash
sudo mv x4 /usr/local/bin
```

---

## 🪟 Running on Windows?

You can run this tool on Windows using **Windows Subsystem for Linux 2 (WSL2)**:

1. Open PowerShell and install WSL:
   ```powershell
   wsl --install
   ```

2. Install Ubuntu:
   ```powershell
   wsl --install -d Ubuntu
   ```

3. Launch Ubuntu from Start Menu and install build tools:
   ```bash
   sudo apt update && sudo apt install g++ build-essential
   ```

4. Compile and run:
   ```bash
   g++ x4.cpp -o x4 -lpthread
   sudo ./x4 <ip> <port> <threads> <duration> ...
   ```

> ⚠️ WSL 1 does **not support raw sockets** – use **WSL 2** for proper ping functionality.

---

## ⚠️ Legal Disclaimer

> This tool is intended **only for educational and authorized testing purposes**. Never use it on networks you do not own or have explicit permission to test. The developer is not responsible for any misuse or resulting consequences.
