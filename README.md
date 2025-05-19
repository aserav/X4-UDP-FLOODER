# 🔥 X4 UDP Flooder v1.0

😏😏😏 A high-performance, multithreaded UDP flood testing tool written in C++ for Linux. Designed for stress testing and bandwidth benchmarking in **controlled, authorized environments** only.

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
./x4 8.8.8.8 53 100 60 --packet-size=1024 --log  # (--packet-size is optional; defaults to 1024 if not specified)
```

---

## 📥 Installation (Linux Only)

### Requirements:
- Linux-based OS  
- g++ compiler (`sudo apt install g++`)  
- Root privileges (required for ping support)

### Steps:

```bash
sudo apt install git (if not installed already)

git clone https://github.com/aserav/X4-UDP-FLOODER-V1.0
g++ x4.cpp -o x4 -lpthread
./x4
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

3. Launch Ubuntu from Start Menu and install Git and build tools:
   ```bash
   sudo apt install git (if not installed already)
  
   sudo apt update && sudo apt install g++ build-essential
   ```

4. Compile and run:
   ```bash
   git clone https://github.com/aserav/X4-UDP-FLOODER-V1.0
   g++ x4.cpp -o x4 -lpthread
   sudo ./x4 <ip> <port> <threads> <duration> ...
   ```

> ⚠️ WSL 1 does **not support raw sockets** – use **WSL 2** for proper ping functionality.

---

## ⚠️ Legal Disclaimer

> This tool is intended **only for educational and authorized testing purposes**. Never use it on networks you do not own or have explicit permission to test. The developer is not responsible for any misuse or resulting consequences.

---

## 🌐 Recommended VPS Setup (Performance Matters)

For best results, you should run this on a **VPS with at least 5Gbps throughput** – ideally 10Gbps – especially if you're using high thread counts or large packet sizes.

One provider worth checking out is:

### ✅ Cloudzy

- Known for offering **10Gbps throughput VPS** options  
- **Cheap bandwidth** and solid price-to-performance  
- Consistently delivers high port capacity and fast CPUs  
- No affiliation — just what I personally use because it performs extremely well for network stress testing

> 🧠 Tip: Always choose a VPS with high outbound speed, strong single-core performance, and minimal bandwidth restrictions if you want accurate flood/benchmark results.
