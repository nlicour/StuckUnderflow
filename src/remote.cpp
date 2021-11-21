#include "remote.h"

#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <cstring>
#include <cstdlib>
#include <vector>

namespace
{
constexpr uint8_t MAX_NUM_REMOTES = 5;

const char* remotes_ips[] = {
    "192.168.88.241",
    "192.168.88.242",
    "192.168.88.243",
    "192.168.88.244",
    "192.168.88.245",
};
} // anonymous namespace

// @Note: the remote system is synchronous.
struct RemoteSystem
{
    struct RemoteListenerContext
    {
        int socket_handle;
    };

    uint8_t connected; // Bitfield.
    RemoteListenerContext contexts[MAX_NUM_REMOTES];
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
    if (remote_id >= MAX_NUM_REMOTES)
    {
        fprintf(stderr, "Invalid remote id %d\n", remote_id);
        return false;
    }

    int socket_handle = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_handle < 0)
    {
        fprintf(stderr, "Couldn't create tcp socket\n");
        return false;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(540);
    server.sin_addr.s_addr = inet_addr(remotes_ips[remote_id]);

    if (connect(socket_handle, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        fprintf(stderr, "Coudn't connect to remote %d with IP %s\n",
            remote_id, remotes_ips[remote_id]);
        return false;
    }

    char reply[1024];
    if (recv(socket_handle, reply, 1024, 0) < 0)
    {
        fprintf(stderr, "Failed to receive.");
    }

    if (strncmp(reply, "24HC21 remote", 13) != 0)
    {
        fprintf(stderr, "Failed to connect.\n");
        return false;
    }

    // @Todo: handle deconnections.
    rs->connected |= (1 << remote_id);
    rs->contexts[remote_id].socket_handle = socket_handle;

    printf("Remote %d connected!\n", remote_id);
    return true;
}

uint16_t wait_for_state_change(RemoteSystem* rs, uint32_t remote_id)
{
    if ((rs->connected & (1 << remote_id)) == 0)
    {
        fprintf(stderr, "Remote %d not connected\n", remote_id);
        return 0;
    }

    if (remote_id >= MAX_NUM_REMOTES)
    {
        fprintf(stderr, "Invalid remote id %d\n", remote_id);
        return 0;
    }

    char reply[32];
    // This is blocking.
    if (recv(rs->contexts[remote_id].socket_handle, reply, sizeof(reply), 0) < 0)
    {
        fprintf(stderr, "Failed to receive.");
        return 0;
    }

    return std::strtoul(reply, NULL, 10);
}

void toggle_led(RemoteSystem* rs, uint8_t remote_id, uint8_t led_data)
{
    if (led_data > 7)
    {
        fprintf(stderr, "Invalid led data.\n");
        return;
    }

    char msg[2];
    msg[0] = '0' + led_data;
    msg[1] = 0;

    if (send(rs->contexts[remote_id].socket_handle, msg, strlen(msg), 0) < 0)
    {
        fprintf(stderr, "Couldn't send remote led data.\n");
    }
}
} // namespace remote
