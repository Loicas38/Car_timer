#ifndef Def_TEAM 
#define Def_TEAM

#include <Arduino.h>
#include "Player.hpp"

struct team_data {
    Player player;
    team_data* next;
    team_data* previous;
};


class Team {
    private :
        team_data* m_players;
        uint8_t m_nb_players;
        uint8_t m_id_active_player;
        uint8_t m_max_id_player;
        uint8_t m_nb_max_players;

        uint8_t m_nb_checkpoints;
        
    public :
        // constructeur 
        Team();
        // Surcharge pour initialisation à la création 
        ~Team();

        uint8_t get_nb_players() const;
        void add_player(String player_name);
        void delete_player(String player_name);
        void delete_player_id(uint8_t player_id);
        bool player_in_team(String player_name) const;

        uint8_t get_nb_max_players() const;
        void set_nb_max_players(uint8_t new_nb_max_players);

        bool is_full() const;

        void reset_times();

        Player* get_players() const;

        uint32_t get_best_time() const;
        Player* get_best_player() const;

        Player* getActivePlayer() const;
        void nextActivePlayer();
        void previousActivePlayer();

        Player* getPlayerById(uint8_t player_id) const;

        void display_data() const;
};

#endif