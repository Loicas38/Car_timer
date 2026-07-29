#ifndef DEF_TIME
#define DEF_TIME

#include <Arduino.h>

// Pour faire uen liste doublement chainée 
typedef struct time_data {
    uint32_t time;
    uint16_t race_id;
    time_data* next;
    time_data* previous;
} time_data;


class Time {
    private :
        uint8_t m_nb_times;
        // the list is sorted, with the first element containing the min time 
        time_data* m_times_lst;
        // This is the last struct of the list above
        time_data* m_end_list;
        uint8_t m_nb_times_max;

    public:
        Time();
        // constructeur de copie, pour éviter les problème avec les pointeurs
        Time(Time const& TimeACopier);
        Time& operator=(Time const& TimeACopier);
        ~Time();

        uint32_t minimum_time() const;
        uint32_t minimum_time_seconds() const;
        uint32_t minimum_time_minutes() const;

        // adds a new time in the list of times, sorted by time.  
        // This doesn't check if the number of times exceeds m_nb_times_max
        void add_new_time(uint32_t new_time, uint16_t race_id);
        // Same thing but adds an auto race_id, which is the max of the existing one + 1 
        void add_new_time(uint32_t new_time);

        // adds a new time and deletes the worst time if the number of times exceeds m_nb_times_max
        void add_new_time_delete_longest(uint32_t new_time, uint16_t race_id);
        // Same with auto race_id 
        void add_new_time_delete_longest(uint32_t new_time);


        void delete_longest_time();
        void delete_all_times();
        void delete_shortest_time();


        // sets the maximum number of times that can be recorded for a player. 
        // If the number of times already recorded exceeds this number, the worst times are deleted until the number of times is equal to the new maximum
        void set_nb_times_max(uint8_t new_nb_times_max);
        uint8_t get_nb_times() const;


        // returns the index th best time recorded, with 0 being the best. 
        uint32_t get_time(uint8_t index) const;
        uint32_t get_time_highest_race_id() const;

        
        uint16_t get_max_race_id() const;
};


#endif