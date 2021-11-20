#ifndef EPISODES_H_
#define EPISODES_H_

class Cube;

enum Episode
{
    S01E01 = 0,
    S01E02,
    S01E03,
    S01E04,

    S02E01,
    S02E02,
    S02E03,
};

namespace episodes
{
void start_episode(Cube*, Episode);
} // namespace episodes

#endif // EPISODES_H_


