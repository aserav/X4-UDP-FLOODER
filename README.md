# 🔥 X4 UDP Flooder v1.0 (Layer 4)

**X4 is a high-performance Layer 4 UDP flood testing tool written in C++ for Linux.**  
Built for **bandwidth benchmarking** and **stress testing** in **authorized, controlled environments only.**

---

## 🚀 Features

- 🧵 **Multithreaded Engine** – Sends packets in parallel for max performance  
- 📊 **Live Stats**:
  - Real-time Mbps & average speed  
  - Total data sent (MB)  
  - ICMP ping latency or timeout  
  - Elapsed & remaining time  
- 🧠 **Built-in ICMP Ping** – Measures latency before and during flood  
- ⚙️ **Custom Packet Size** – Adjustable via `--packet-size=SIZE`  
- 📄 **Optional Logging** – Save full session stats with `--log`  
- 🌐 **IPv4 + Domain Support** – Enter IPs or hostnames  
- ⛔ **Graceful Ctrl+C Handling** – Clean exit with full summary and saved log  

---

## 💻 Usage

```bash
./x4 <IP or Domain> <PORT> <THREADS> <DURATION> [--packet-size=SIZE] [--log]
```

### Example:
```bash
./x4 8.8.8.8 53 100 60 --packet-size=1024 --log
# (If --packet-size is omitted, defaults to 1024 bytes)
```

---

## 📥 Installation (Linux & WSL2)

### Requirements:
- A **Linux** system (or **WSL2** on Windows)  
- `g++` and build tools installed  
- **Root privileges** (required for ICMP ping)

### Steps:

```bash
# Update packages
sudo apt update

# Install required dependencies
sudo apt install git g++ build-essential -y

# Clone the project
git clone https://github.com/aserav/X4-UDP-FLOODER-V1.0
cd X4-UDP-FLOODER-V1.0

# Compile the source code
g++ x4.cpp -o x4 -lpthread

# Run the flooder
sudo ./x4 <target> <port> <threads> <duration> [--packet-size=SIZE] [--log]
```

(Optional) Move it to your system path for global access:

```bash
sudo mv x4 /usr/local/bin
```

---

## 🪟 Running on Windows (via WSL2)

To run this on Windows, you must use **WSL2**, which supports raw sockets:

1. **Enable WSL & WSL2:**
   ```powershell
   wsl --install
   ```

2. **Install Ubuntu via WSL:**
   ```powershell
   wsl --install -d Ubuntu
   ```

3. **Follow the same Linux steps above inside Ubuntu terminal.**

> ⚠️ WSL1 does **not support raw sockets**, so ICMP ping will not work. Always use **WSL2**.

---

## ⚠️ Legal Disclaimer

> This software is for **educational use and authorized testing** only.  
> **Do not** use this on networks or systems you do not own or lack explicit permission to test.  
> The developer assumes **no liability** for misuse or damage caused by this software.

---

## 🌐 Recommended VPS for Best Performance

To fully leverage X4’s speed, use a VPS with **at least 5–10Gbps throughput**.

### ✅ Top Pick: **Cloudzy**

- Offers **10Gbps ports**  
- **Affordable pricing**  
- No bandwidth throttling  
- Optimized for high-speed testing  
- Not sponsored — recommended from experience

> **Tips for best results:**
> - Use a VPS with strong **single-core performance**  
> - Avoid hosts with capped outbound speed  
> - Choose a server close to your target for lower ping  
