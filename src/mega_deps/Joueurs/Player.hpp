#ifndef DEF_JOUEUR
#define DEF_JOUEUR

#include <Arduino.h>
#include "Times.hpp"

class Player {
    private :
        String m_name;
        Time m_times;
        uint8_t m_player_id;
        uint8_t m_nb_checkpoints;
        Time* m_checkpoint_times;

    public :
        // constructeur 
        Player();
        // Surcharge pour initialisation à la création 
        Player(String m_nom, uint8_t nb_checkpoints, uint8_t player_id);
        ~Player();
        
        uint32_t getBestTime() const;
        uint32_t getBestTimeSeconds() const;
        uint32_t getBestTimeMinutes() const;

        void update_best(uint32_t new_best);
        void update_best(uint32_t new_best, uint16_t race_id);

        uint32_t get_last_time() const;
        uint16_t get_last_time_seconds() const;
        uint16_t get_last_time_minutes() const;

        uint8_t get_nb_times_saved() const;

        void add_new_time(uint32_t new_time, uint16_t race_id);
        void add_new_time(uint32_t new_time);

        void add_new_time_checkpoint(uint32_t new_time, uint8_t checkpoint_id);
        void add_new_time_all_checkpoints(uint32_t* new_times, uint8_t nb_checkpoints);
        uint32_t get_time_checkpoint(uint8_t checkpoint_id) const;

        String get_name() const;
        uint8_t get_id() const;
        uint8_t set_id(uint8_t new_id);

        void reset_times();

        void delete_best_time();
};


#endif