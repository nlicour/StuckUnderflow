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
constexpr uint8_t MAX_EVENT_QUEUE_SIZE = 64;
constexpr uint8_t MAX_NUM_REMOTES = 5;

const char* remotes_ips[] = {
    "192.168.88.241",
    "192.168.88.242",
    "192.168.88.243",
    "192.168.88.244",
    "192.168.88.245",
};

struct RemoteListenerContext
{
    bool run;
    int socket_handle;

    uint8_t queue_head = 0;
    std::vector<uint16_t> event_queue; // Event queue of buttons states.
};

void* remote_listener_task(void* args)
{
    RemoteListenerContext* ctx = (RemoteListenerContext*)args;

    char reply[1024];

    while (ctx->run)
    {
        // This is blocking.
        if (recv(ctx->socket_handle, reply, sizeof(reply), 0) < 0)
        {
            fprintf(stderr, "Failed to receive.");
        }

        uint32_t buttons_state = std::strtoul(reply, NULL, 10);
        if (ctx->event_queue.size() == MAX_EVENT_QUEUE_SIZE)
        {
            ctx->event_queue[ctx->queue_head++] = buttons_state;
        }
        else
        {
            ctx->queue_head = 0;
            ctx->event_queue.push_back(buttons_state);
        }
    }

    pthread_exit(0);
}
} // anonymous namespace

struct RemoteSystem
{
    // @Todo: handle remotes deconnection.
    pthread_t handles[MAX_NUM_REMOTES];
    RemoteListenerContext* contexts[MAX_NUM_REMOTES];
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
        fprintf(stderr, "Coudn't connect to remove with IP %s\n", remotes_ips[remote_id]);
        return false;
    }

    /*
    const char* msg = "hello";
    if (send(socket_handle, msg, strlen(msg), 0) < 0)
    {
        fprintf(stderr, "Fail to send message\n");
    }
    */

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

    // It looks like a remote is already connected.
    if (rs->contexts[remote_id] != NULL)
    {
        rs->handles[remote_id] = 0;
        rs->contexts[remote_id]->run = false;
    }

    RemoteListenerContext* ctx = new RemoteListenerContext();
    ctx->run = true;
    ctx->socket_handle = socket_handle;
    if(pthread_create(&rs->handles[remote_id], NULL, remote_listener_task, ctx) < 0)
    {
        fprintf(stderr, "Couldn't create remote thread.\n");
        return false;
    }
    rs->contexts[remote_id] = ctx;

    printf("Remote %d connected!\n", remote_id);
    return true;
}

bool poll_remote(RemoteSystem* rs, uint32_t remote_id, uint16_t* buttons)
{
    if (remote_id >= MAX_NUM_REMOTES)
    {
        fprintf(stderr, "Invalid remote id %d\n", remote_id);
        return false;
    }

    if (rs->contexts[remote_id] == NULL) return false;

    if (!rs->contexts[remote_id]->event_queue.empty())
    {
        *buttons = rs->contexts[remote_id]->event_queue.back();
        rs->contexts[remote_id]->event_queue.pop_back();
        return true;
    }

    return false;
}
} // namespace remote
