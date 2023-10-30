#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <arpa/inet.h>  // 为了 htons
#include <bpf/bpf_helpers.h>  // 为了 SEC 宏

SEC("xdp_firewall")
int xdp_prog_simple(struct xdp_md *ctx) {
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ethhdr *eth = data;
    if ((void*)eth + sizeof(*eth) > data_end)
        return XDP_DROP;

    if (eth->h_proto == htons(ETH_P_IP)) {
        struct iphdr *ip = data + sizeof(*eth);
        if ((void*)ip + sizeof(*ip) > data_end)
            return XDP_DROP;

        // 示例：阻止所有ICMP流量
        if (ip->protocol == IPPROTO_ICMP)
            return XDP_DROP;
    }

    return XDP_PASS;
}
char _license[] SEC("license") = "GPL";
