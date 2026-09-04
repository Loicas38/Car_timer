#include "Checkpoint.hpp"
#include "mega_deps/Ecran/Screen.hpp"
#include "mega_deps/Utils/Utils.h"

#ifndef CHECKPOINTS
#define CHECKPOINTS

typedef struct s_maillon {
    Checkpoint* checkpoint;
    struct s_maillon* next;
} maillon;


class Checkpoints {
private: 
    const int m_nb_max_checkpoint = 10;
    // The number of checkpoints which are really connected
    int m_nb_checkpoints_connected;
    // the number of checkpoints we would like to get
    int m_nb_objective_checkpoints;

    // this is a lsite chainée of checkpoints, which is sorted by ascendant ids
    maillon* m_checkpoints_lst = NULL;

    // returns the first id which is not yet used
    int first_free_id() const;

    // returns the smallest int which is greater than all ids
    int smallest_greater_id() const;

    // This is intended to let checkpoints being switch on, and to connect. So, this fun will initialize them automatically
    // This is automatically called when the object is initialized
    void initial_get_checkpoints(Screen* screen);

public:
    Checkpoints(Screen* screen);
    ~Checkpoints();

    // Allows to select which checkpoints have to be taken into consideration for the race among the ones 
    // which are connected
    void select_used_checkpoints(Screen* screen);

    // Displays a menu to allow the person to chose a new order for the checkpoints
    void change_order(Screen* screen);

    // Displays a menu to do again the distance measure for some checkpoints
    void measure_distance(Screen* screen);

    // Allows to connect new checkpoints (that is to say, to switch on new checkpoints)
    void connect_new_checkpoints(Screen* screen);

};

#endif