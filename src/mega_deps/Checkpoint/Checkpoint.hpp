#include <Arduino.h>

#ifndef CHECKPOINT
#define CHECKPOINT

class Checkpoint{


    
private:
    // the unique id of the checkpoint. This is chosen by the system
    int m_id;
    // the order in which the checkpoint is. This is chosen by the user
    int m_order;
    // to store if the checkpoint is on (wich is equivalent to connected to the serveur)
    bool m_on;
    // to store if this checkpoint has to be taken into consideration for the race
    bool m_used;

    void envoieMessage(String message) const;

public:
    Checkpoint(int id);
    Checkpoint(int id, bool m_on, bool m_used);
    ~Checkpoint();

    void update_id(int id);
    int get_id() const;

    // It doesn't really switches on or off, it just puts it waiting or not (may allow to make the batteries last longer)
    void switch_on();
    void switch_off();

    bool is_on();

    // Asks the cjeckpoint to make his distance measure again
    void measure_distance() const;

};





#endif