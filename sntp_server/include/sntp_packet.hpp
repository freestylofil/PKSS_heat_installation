#ifndef SNTP_SNTP_PACKET_HPP
#define SNTP_SNTP_PACKET_HPP

#include <cstdint>
#include <ctime>

#include <byteswap.h>

struct [[gnu::packed]] sntp_packet {
    uint32_t header;
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t reference_id;
    uint64_t reference_timestamp;
    uint64_t originate_timestamp;
    uint64_t receive_timestamp;
    uint64_t transmit_timestamp;
//    uint32_t empty[5] = {};
};

uint64_t unix_to_ntp(std::time_t t) {
    auto EPOCH = 2208988800ULL;
//    auto NTP_SCALE_FRAC = 4294967296ULL;
    uint64_t tv_ntp = t + EPOCH;
    return (tv_ntp << 32U);
}

inline void fill_sntp_udp(sntp_packet* packet, std::time_t timestamp) {
    uint64_t timestamp_ntp = bswap_64(unix_to_ntp(timestamp));
    packet->header = 0x00080124; //0 4 4 1 cp prec 0 0 srciden timeolast copied tod tod 0
    packet->root_delay = 0;
    packet->root_dispersion = 0;
    packet->reference_id = 0;
    packet->reference_timestamp = timestamp_ntp;
    packet->originate_timestamp = packet->transmit_timestamp;
    packet->receive_timestamp = timestamp_ntp;
    packet->transmit_timestamp = timestamp_ntp;
}

inline void fill_sntp_broadcast(sntp_packet* packet, std::time_t timestamp) {
    uint64_t timestamp_ntp = bswap_64(unix_to_ntp(timestamp));
    packet->header = 0x00080125;
    packet->root_delay = 0;
    packet->root_dispersion = 0;
    packet->reference_id = 0;
    packet->reference_timestamp = timestamp_ntp;
    packet->originate_timestamp = 0;
    packet->receive_timestamp = 0;
    packet->transmit_timestamp = timestamp_ntp;
}

#endif //SNTP_SNTP_PACKET_HPP
