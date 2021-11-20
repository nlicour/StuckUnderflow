#include "remote.h"

#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>

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
RemoteSystem* create_system()
{
    return new RemoteSystem();
}

void destroy_system(RemoteSystem* rs)
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

    /*
    const char* msg = "hello";
    if (send(rs->socket_handle, msg, strlen(msg), 0) < 0)
    {
        fprintf(stderr, "Fail to send message\n");
    }
    */

    char reply[1024];
    if (recv(rs->socket_handle, reply, 1024, 0) < 0)
    {
        fprintf(stderr, "Failed to receive.");
    }

    if (strncmp(reply, "24HC21 remote", 13) != 0)
    {
        fprintf(stderr, "Failed to connect.\n");
        return false;
    }

    printf("Remote connected!\n");
    return true;
}
} // namespace remote
