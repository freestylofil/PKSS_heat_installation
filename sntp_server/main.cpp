#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/buffer.hpp>
#include <byteswap.h>
#include <boost/asio.hpp>
#include <iostream>
#include "sntp_packet.hpp"
#include "structopt/app.hpp"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

struct Options {
    std::optional<int> port = 8123;
    enum class Mode {
        udp, broadcast
    };
    std::optional<Mode> mode = Mode::broadcast;
    std::optional<double> acceleration = 1;
    std::optional<int> period = 1;
}; STRUCTOPT (Options, port, mode, acceleration);

std::time_t accelerate_time(std::time_t t, std::time_t base, double a) {
    return std::round<std::time_t>(base + (t - base) * a);
}

void send_time_broadcast(udp::socket &s, udp::endpoint &endpoint, boost::asio::steady_timer &timer,
                         std::chrono::seconds interval, sntp_packet &req_packet, std::time_t base, double a);

void send_time_broadcast_handler(udp::socket &s, udp::endpoint &endpoint, boost::asio::steady_timer &timer,
                                 std::chrono::seconds interval, sntp_packet &req_packet, std::time_t base, double a) {
    timer.expires_at(timer.expiry() + interval);
    timer.async_wait([&, base, a](const boost::system::error_code &ec) {
        send_time_broadcast(s, endpoint, timer, interval, req_packet, base, a);
    });
}

void send_time_broadcast(udp::socket &s, udp::endpoint &endpoint, boost::asio::steady_timer &timer,
                         std::chrono::seconds interval, sntp_packet &req_packet, std::time_t base, double a) {
    auto now = accelerate_time(std::time(nullptr), base, a);
    fill_sntp_broadcast(&req_packet, now);
    s.async_send_to(boost::asio::buffer(&req_packet, sizeof(sntp_packet)), endpoint,
                    [&, base, a](const boost::system::error_code &ec, std::size_t len) {
                        send_time_broadcast_handler(s, endpoint, timer, interval, req_packet, base, a);
                    });
}

void send_time_udp(udp::socket &s, udp::endpoint &endpoint, sntp_packet &req_packet, std::time_t base, double a);

void
send_time_udp_handler(udp::socket &s, udp::endpoint &endpoint, sntp_packet &req_packet, std::time_t base, double a) {
    s.async_receive_from(boost::asio::buffer(&req_packet, sizeof(sntp_packet)), endpoint,
                         [&, base, a](const boost::system::error_code &ec, std::size_t len) {
                             send_time_udp(s, endpoint, req_packet, base, a);
                         });
}

void send_time_udp(udp::socket &s, udp::endpoint &endpoint, sntp_packet &req_packet, std::time_t base, double a) {
    auto now = accelerate_time(std::time(nullptr), base, a);
    fill_sntp_udp(&req_packet, now);
    s.async_send_to(boost::asio::buffer(&req_packet, sizeof(sntp_packet)), endpoint,
                    [&, base, a](const boost::system::error_code &ec, std::size_t len) {
                        send_time_udp_handler(s, endpoint, req_packet, base, a);
                    });
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main(int argc, char *argv[]) {
    auto options = structopt::app("sntp_server").parse<Options>(argc, argv);

    double a = options.acceleration.value();
    std::time_t base = std::time(nullptr);
    int port = options.port.value();

    sntp_packet packet[1];

    boost::asio::io_context io_ctx;
    boost::asio::ip::udp::socket s(io_ctx);
    udp::endpoint endpoint;

    std::chrono::seconds interval(options.period.value());
    boost::asio::steady_timer timer(io_ctx, interval);

    if (options.mode == Options::Mode::broadcast) {
        s.open(udp::v4());
        s.set_option(boost::asio::ip::unicast::hops(1));
        s.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        s.set_option(boost::asio::socket_base::broadcast(true));
        endpoint = udp::endpoint(boost::asio::ip::address_v4::broadcast(), port);
        timer.async_wait([&, base, a](const boost::system::error_code &ec) {
            send_time_broadcast(s, endpoint, timer, interval, packet[0], base, a);
        });
    }

    if (options.mode == Options::Mode::udp) {
        s = {io_ctx, udp::endpoint(udp::v4(), port)};
        s.set_option(boost::asio::ip::unicast::hops(1));
        s.async_receive_from(boost::asio::buffer(packet), endpoint,
                             [&, base, a](const boost::system::error_code &ec, std::size_t len) {
                                 send_time_udp(s, endpoint, packet[0], base, a);
                             });
    }

    std::cout << "Server started with port " << port << "\n";
    io_ctx.run();
}

#pragma clang diagnostic pop
