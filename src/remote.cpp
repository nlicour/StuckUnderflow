#include "remote.h"

#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace
{
    const char* remotes_ips[] = {
        "192.168.88.241",
        "192.168.88.242",
        "192.168.88.243",
        "192.168.88.244",
        "192.168.88.245",
    };
}

struct RemoteSystem
{
    int socket_handle;
    struct sockaddr_in server;
};

namespace remote
{
RemoteSystem* create()
{
    return new RemoteSystem();
}

void destroy(RemoteSystem* rs)
{
    delete rs;
}

bool connect(RemoteSystem* rs, uint32_t remote_id)
{
    rs->socket_handle = socket(AF_INET, SOCK_STREAM, 0);
    if (rs->socket_handle < 0)
    {
        fprintf(stderr, "Couldn't create tcp socket\n");
        return false;
    }

    rs->server.sin_family = AF_INET;
    rs->server.sin_port = htons(540);
    rs->server.sin_addr.s_addr = inet_addr(remotes_ips[remote_id]);

    if (connect(rs->socket_handle, (struct sockaddr*)&rs->server, sizeof(rs->server)) < 0)
    {
        fprintf(stderr, "Coudn't connect to remove with IP %s\n", remotes_ips[remote_id]);
        return false;
    }

    return true;
}
} // namespace remote
