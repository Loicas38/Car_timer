#include "Checkpoints.hpp"

int Checkpoints::first_free_id() const{
    maillon* m = m_checkpoints_lst;
    int res = 0;

    while (m != NULL){
        if (res != m->checkpoint->get_id()){
            return res;
        }

        m = m->next;
    }

    return res;
}


int Checkpoints::smallest_greater_id() const{
    maillon* m = m_checkpoints_lst;

    while (m != NULL){
        if (m->next == NULL){
            return m->checkpoint->get_id() + 1;
        }

        m = m->next;
    }

    // if we get there, this means the list was empty
    return 0;
}


void Checkpoints::select_used_checkpoints(Screen* screen) {

    if(m_nb_checkpoints_connected == 0){
        return;
    }

    String ids[m_nb_checkpoints_connected];

    maillon* m = m_checkpoints_lst;

    for(int i = 0; i < m_nb_checkpoints_connected; i++){
        ids[i] = String(m->checkpoint->get_id());
        m = m->next;
    }

    bool* selected = screen->multiple_options("Checkpoints to use", ids, m_nb_checkpoints_connected);

    m = m_checkpoints_lst;
    m_nb_objective_checkpoints = 0;
    
    for(int i = 0; i < m_nb_checkpoints_connected; i++){
        if(selected[i]){
            m->checkpoint->switch_on();
            m_nb_objective_checkpoints++;
        } else {
            m->checkpoint->switch_off();
        }

        m = m->next;
    }

    free(selected);
}


void Checkpoints::initial_get_checkpoints(Screen* screen){
    m_nb_checkpoints_connected = 0;
    m_nb_objective_checkpoints = 0;
    screen->display("The acquisition of the checkpoints has not been implemented yet", "");
}


Checkpoints::Checkpoints(Screen* screen){
    Serial.println("Begining checkpoint init");
    initial_get_checkpoints(screen);
    Serial.println("Stage 1 finished");
    select_used_checkpoints(screen);
}


Checkpoints::~Checkpoints(){
    while(m_checkpoints_lst != NULL){
        maillon* m = m_checkpoints_lst->next;
        delete m_checkpoints_lst->checkpoint;
        free(m_checkpoints_lst);
        m_checkpoints_lst = m;
    }

}


void Checkpoints::change_order(Screen* screen){
    screen->display("Not yet implemented !", "");
}

void Checkpoints::measure_distance(Screen* screen){
    screen->display("Not yet implemented !", "");
}

void Checkpoints::connect_new_checkpoints(Screen* screen){
    screen->display("Not yet implemented !", "");
}