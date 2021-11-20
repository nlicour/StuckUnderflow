#ifndef REMOTE_H_
#define REMOTE_H_

#include <cstdint>

class RemoteSystem;

namespace remote
{
RemoteSystem* create_system();
void destroy_system(RemoteSystem*);
bool connect(RemoteSystem*, uint32_t remote_id);
bool poll_remote(RemoteSystem*, uint32_t remote_id, uint16_t* buttons);
} // namespace anonymous

#endif // REMOTE_H_


