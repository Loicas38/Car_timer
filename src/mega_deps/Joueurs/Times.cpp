// Classe qui permet de gérer des temps, conversions, modifs et compagnie 

#include "Times.hpp"

Time::Time(): m_nb_times(0), m_times_lst(NULL), m_end_list(NULL), m_nb_times_max(5)
{

}

// constructeur de copie
Time ::Time(Time const& timeACopier) : m_nb_times(timeACopier.m_nb_times), m_nb_times_max(timeACopier.m_nb_times_max)
{
    m_times_lst = NULL;
    m_end_list = NULL;

    time_data* actuel = timeACopier.m_times_lst;

    if (m_nb_times > 0){
        m_times_lst = new time_data;
        m_times_lst->next = NULL;
        m_times_lst->previous = NULL;
        m_times_lst->time = actuel->time;
        m_times_lst->race_id = actuel->race_id;
        m_end_list = m_times_lst;
    }

    actuel = actuel->next;

    while(actuel != NULL){
        time_data* temp = new time_data;

        temp->race_id = actuel->race_id;
        temp->time = actuel->time;
        temp->next = NULL;
        temp->previous = m_times_lst;

        m_end_list->next = temp;
        m_end_list = temp;

        actuel = actuel->next;

    }
}


Time& Time::operator=(Time const& timeACopier){
    if(this != &timeACopier){
        m_nb_times = timeACopier.m_nb_times;
        m_nb_times_max = timeACopier.m_nb_times_max;

        // on libère les maillons 
        time_data* actuel = m_times_lst;

        while(actuel != NULL){
            time_data* suivant = actuel->next;
            delete actuel;
            actuel = suivant;
        }

        m_times_lst = NULL;
        m_end_list = NULL;

        actuel = timeACopier.m_times_lst;

        while(actuel != NULL){
            time_data* temp = new time_data;
            temp->time = actuel->time;
            temp->race_id = actuel->race_id;
            temp->next = NULL;
            temp->previous = m_end_list;

            if(m_end_list != NULL){
                m_end_list->next = temp;
            }

            // n'est validé que pour le premier maillon
            if(m_times_lst == NULL){
                m_times_lst = temp;
            }

            m_end_list = temp;
        }
    }

    return *this;
}


uint32_t Time::minimum_time() const{
    if(m_nb_times == 0){
        return 0;
    }

    return m_times_lst->time;
}


uint32_t Time::minimum_time_seconds() const{
    if(m_nb_times == 0){
        return 0;
    }

    return m_times_lst->time / 1000;
}


uint32_t Time::minimum_time_minutes() const{
    if(m_nb_times == 0){
        return 0;
    }

    return m_times_lst->time / 60000;
}



void Time::add_new_time(uint32_t new_time, uint16_t race_id){
    time_data* maillon = (time_data*)malloc(sizeof(time_data));

    maillon->race_id = race_id;
    maillon->time = new_time;
    maillon->next = NULL;
    maillon->previous = NULL;

    if(m_nb_times == 0){
        m_times_lst = maillon;
        m_end_list = maillon;
        m_nb_times++;
        return;
    }
    
    time_data* temp = m_times_lst;

    while (temp != NULL && temp->time < new_time){
        temp = temp->next;
    }

    if (temp == NULL){
        maillon->previous = m_end_list;
        m_end_list->next = maillon;
        m_end_list = maillon;
        m_nb_times++;
        return;
    }

    maillon->next = temp;
    maillon->previous = temp->previous;
    if(temp->previous != NULL){
        temp->previous->next = maillon;
    }
    m_nb_times++;

}

void Time::add_new_time(uint32_t new_time){
    uint16_t new_race_id = get_max_race_id() + 1;
    add_new_time(new_time, new_race_id);
}

void Time::delete_longest_time(){
    if(m_nb_times == 0){
        return;
    }

    time_data* temp = m_end_list;

    if(temp->previous != NULL){
        temp->previous->next = NULL;
    }
    m_end_list = temp->previous;
    free(temp);
    m_nb_times--;
}


void Time::add_new_time_delete_longest(uint32_t new_time, uint16_t race_id){
    add_new_time(new_time, race_id);
    if(m_nb_times > m_nb_times_max){
        delete_longest_time();
    }
}


void Time::add_new_time_delete_longest(uint32_t new_time){
    uint16_t new_race_id = get_max_race_id() + 1;
    add_new_time_delete_longest(new_time, new_race_id);
}


void Time::delete_all_times(){
    while (m_nb_times > 0){
        delete_longest_time();
    }
}


void Time::set_nb_times_max(uint8_t new_nb_times_max){
    m_nb_times_max = new_nb_times_max;
    while (m_nb_times > m_nb_times_max){
        delete_longest_time();
    }
}

uint8_t Time::get_nb_times() const{
    return m_nb_times;
}


uint32_t Time::get_time(uint8_t index) const{
    if (index >= m_nb_times){
        return 0;
    }

    time_data* temp = m_times_lst;

    for(int i = 0; i < index; i++){
        temp = temp->next;
    }

    return temp->time;
}

uint32_t Time::get_time_highest_race_id() const{
    if(m_nb_times == 0){
        return 0;
    }

    time_data* max_race_id = m_times_lst;
    time_data* temp = m_times_lst;

    while(temp->next != NULL){
        if(temp->race_id > max_race_id->race_id){
            max_race_id = temp;
        }
        temp = temp->next;
    }

    return max_race_id->time;
}


uint16_t Time::get_max_race_id() const{
    if(m_nb_times == 0){
        return 0;
    }

    uint16_t max_race_id = m_times_lst->race_id;
    time_data* temp = m_times_lst;

    while(temp->next != NULL){
        if(temp->race_id > max_race_id){
            max_race_id = temp->race_id;
        }
        temp = temp->next;
    }

    return max_race_id;
}



Time::~Time(){
    
    while (m_times_lst != NULL){
        time_data* temp = m_times_lst;
        m_times_lst = m_times_lst->next;
        free(temp);
    }
}

void Time::delete_shortest_time(){
    if(m_nb_times == 0){
        return;
    }

    time_data* temp = m_times_lst;

    if(temp->next != NULL){
        temp->next->previous = NULL;
    }
    m_times_lst = temp->next;
    free(temp);
    m_nb_times--;
}