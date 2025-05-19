# 🔥 X4 UDP Flooder v1.0

**A high-performance, multithreaded UDP flood testing tool written in C++ for Linux.**  
Built for **stress testing** and **bandwidth benchmarking** in **controlled, authorized environments** only.

---

## 🚀 Features

- 🧵 **Multithreaded Engine** – High-speed parallel packet sending  
- 📊 **Live Stats**:
  - Real-time Mbps & average speed  
  - Total MB sent  
  - Ping latency (ms) or timeout  
  - Elapsed & remaining time  
- 🧠 **Built-in ICMP Ping** – Automatic ping before and during flood  
- 📄 **Optional Logging** – Save detailed stats with `--log`  
- ⚙️ **Custom Packet Size** – Set via `--packet-size=SIZE`  
- 🌐 **IPv4 & Domain Support** – Accepts domains or direct IPs  
- ⛔ **Graceful Ctrl+C Handling** – Clean exit with summary + optional log  

---

## 💻 Usage

```bash
./x4 <IP/Domain> <PORT> <THREADS> <DURATION> [--packet-size=SIZE] [--log]
```

### Example:
```bash
./x4 8.8.8.8 53 100 60 --packet-size=1024 --log
# (--packet-size is optional; defaults to 1024 if not specified)
```

---

## 📥 Installation (Linux)

### Requirements:
- Linux OS  
- `g++` compiler (`sudo apt install g++`)  
- Root privileges (for ICMP ping support)

### Steps:

```bash
# Install git if needed
sudo apt install git

# Clone and compile
git clone https://github.com/aserav/X4-UDP-FLOODER-V1.0
cd X4-UDP-FLOODER-V1.0
g++ x4.cpp -o x4 -lpthread

# Run the flooder
./x4 <target> <port> <threads> <duration>
```

(Optional) Move the binary to system path:

```bash
sudo mv x4 /usr/local/bin
```

---

## 🪟 Running on Windows (via WSL2)

You can run this tool using **Windows Subsystem for Linux 2 (WSL2)**:

1. **Install WSL:**
   ```powershell
   wsl --install
   ```

2. **Install Ubuntu:**
   ```powershell
   wsl --install -d Ubuntu
   ```

3. **Inside Ubuntu shell:**
   ```bash
   sudo apt update
   sudo apt install git g++ build-essential

   git clone https://github.com/aserav/X4-UDP-FLOODER-V1.0
   cd X4-UDP-FLOODER-V1.0
   g++ x4.cpp -o x4 -lpthread
   sudo ./x4 <target> <port> <threads> <duration> ...
   ```

> ⚠️ WSL **1 does not support raw sockets**. Use **WSL 2** for full functionality (including ping).

---

## ⚠️ Legal Disclaimer

> This software is for **educational and authorized testing** only. Do **not** use it on networks you do not own or lack explicit permission to test.  
> The developer assumes **no responsibility** for misuse or damages caused by this software.

---

## 🌐 Recommended VPS Setup (Performance Tips)

For best results, use a **VPS with 5–10Gbps throughput**. Higher outbound speed = more accurate benchmark and stress test results.

### ✅ Recommended Provider: **Cloudzy**

- Offers **10Gbps bandwidth** options  
- **Affordable** plans with **high throughput**  
- Excellent for network testing and packet flooding  
- Not sponsored — just proven performance from personal use

> **Pro tip:** Choose a VPS with:
> - Strong single-core performance  
> - No bandwidth throttling  
> - 10Gbps port (if available)  
> - Low latency to target