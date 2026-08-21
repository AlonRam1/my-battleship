#include "gameplay.h"

#include <unistd.h>

using namespace std;

void gameplay_loop(Player& p1, Player& p2)
{
    while(p1.get_player_board().get_num_of_active_ships() > 0 &&
          p2.get_player_board().get_num_of_active_ships() > 0)
    {
        p1.take_target(p2);

        if(p2.get_player_board().get_num_of_active_ships() == 0)
            break;

        p2.take_target(p1);
    }

    string s;
    usleep(100000);
    if(p1.get_player_board().get_num_of_active_ships() == 0)
        s = "player 2 won!";
    else
        s = "player 1 won!";

    p1.update_screen(s);
    p2.update_screen(s);
}
