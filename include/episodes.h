#ifndef EPISODES_H_
#define EPISODES_H_

class Cube;

enum Episode
{
    MULTIPLE_REMOTES = 0,
    S01E01,
    S01E02,
    S01E03,
    S01E04,

    S02E01,
    S02E02,
    S02E03,

    FUN_01,
};

namespace episodes
{
void start_episode(Cube*, RemoteSystem*, Episode);
} // namespace episodes

#endif // EPISODES_H_


