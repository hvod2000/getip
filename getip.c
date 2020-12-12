#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/wireless.h>

char *ifaddrs2str(struct ifaddrs *ifa) {
    char ip[16], essid[32] = "";
    struct iwreq req;
    strcpy(req.ifr_ifrn.ifrn_name, ifa->ifa_name);
    req.u.essid.pointer = essid;
    req.u.essid.length = 32;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    ioctl(sock, SIOCGIWESSID, &req); close(sock);
    getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr), ip, 16, NULL, 0, 1);
    char *res = malloc(strlen(ifa->ifa_name) + strlen(ip) + strlen(essid) + 4);
    sprintf(res, "%s:%s<%s>", ifa->ifa_name, essid, ip);
    return res;
}

int main() {
    struct ifaddrs *ifas;
    getifaddrs(&ifas);
    for (struct ifaddrs *ifa = ifas; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL
            || ifa->ifa_addr->sa_family != AF_INET
            || strcmp(ifa->ifa_name, "lo") == 0) continue;
        printf("%s\n", ifaddrs2str(ifa));
    }
    free(ifas);
}
