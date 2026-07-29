#include "Team.hpp"

Team::Team() : m_players(NULL), m_nb_players(0), m_id_active_player(0), m_max_id_player(0), m_nb_max_players(10), m_nb_checkpoints(0)
{

}


Team::~Team()
{
    if(m_players != NULL){
        team_data* current = m_players;
        team_data* next;
        for (uint8_t i = 0; i < m_nb_players; i++){
            next = current->next;
            delete current;
            current = next;
        }
    }
}


uint8_t Team::get_nb_players() const{
    return m_nb_players;
}

void Team::add_player(String player_name){
    Serial.println("ajout du joueur (fonction classe Team): " + player_name);
    team_data* new_player = new team_data;
    new_player->player = Player(player_name, m_nb_checkpoints, m_max_id_player);
    m_max_id_player++;

    if (m_players == NULL){
        m_players = new_player;
        new_player->next = NULL;
        new_player->previous = NULL;
    } else {
        new_player->previous = NULL;
        new_player->next = m_players;
        m_players->previous = new_player;
        m_players = new_player;
    }

    m_nb_players++;
}

// changes all ids to avoid to have some missing
void Team::delete_player_id(uint8_t player_id){
    if (m_players == NULL){
        return;
    }

    team_data* current = m_players;

    for(uint8_t i = 0; i < m_nb_players; i++){
        if (current->player.get_id() > player_id){
            current->player.set_id(current->player.get_id() - 1);
        }
    }

    m_max_id_player--;
}


void Team::delete_player(String player_name){
    if (m_players == NULL){
        return;
    }

    team_data* current = m_players;

    for (uint8_t i = 0; i < m_nb_players; i++){
        if (current->player.get_name() == player_name){
            delete_player_id(current->player.get_id());
            if (current->previous != NULL){
                current->previous->next = current->next;
            }
            if (current->next != NULL){
                current->next->previous = current->previous;
            }
            if (current == m_players){
                m_players = current->next;
            }
            delete current;
            m_nb_players--;
            return;
        }
        current = current->next;
    }
}


bool Team::player_in_team(String player_name) const{
    team_data* current = m_players;

    for (uint8_t i = 0; i < m_nb_players; i++){
        if (current->player.get_name() == player_name){
            return true;
        }
        current = current->next;
    }

    return false;
}

uint8_t Team::get_nb_max_players() const{
    return m_nb_max_players;
}

void Team::set_nb_max_players(uint8_t new_nb_max_players){
    m_nb_max_players = new_nb_max_players;
}

bool Team::is_full() const{
    return m_nb_players >= m_nb_max_players;
}

void Team::reset_times(){
    team_data* current = m_players;

    for (uint8_t i = 0; i < m_nb_players; i++){
        current->player.reset_times();
        current = current->next;
    }
}


Player* Team::get_players() const{
    Player* players_array = new Player[m_nb_players];
    team_data* current = m_players;
    for (uint8_t i = 0; i < m_nb_players; i++){
        players_array[i] = current->player;
        current = current->next;
    }
    return players_array;
}


uint32_t Team::get_best_time () const {
    uint32_t best_time = 0;
    team_data* current = m_players;

    for (uint8_t i = 0; i < m_nb_players; i++){
        uint32_t player_best_time = current->player.getBestTime();
        if (player_best_time != 0 && (best_time == 0 || player_best_time < best_time)){
            best_time = player_best_time;
        }
        current = current->next;
    }

    return best_time;
}



Player* Team::get_best_player() const {
    Player* best_player = NULL;
    team_data* current = m_players;

    for (uint8_t i = 0; i < m_nb_players; i++){
        uint32_t player_best_time = current->player.getBestTime();
        if (player_best_time != 0 && (best_player == NULL || player_best_time < best_player->getBestTime())){
            best_player = &(current->player);
        }
        current = current->next;
    }

    return best_player;
}

Player* Team::getActivePlayer() const {
    team_data* current = m_players;
    for (uint8_t i = 0; i < m_nb_players; i++){
        if (current->player.get_id() == m_id_active_player){
            return &(current->player);
        }
        current = current->next;
    }

    return NULL;
}


void Team::nextActivePlayer(){
    if (m_players == NULL){
        return;
    }

    m_id_active_player = (m_id_active_player + 1) % m_max_id_player;
}

void Team::previousActivePlayer(){
    if (m_players == NULL){
        return;
    }

    m_id_active_player = (m_id_active_player - 1) % m_max_id_player;
}


Player* Team::getPlayerById(uint8_t player_id) const {
    team_data* current = m_players;
    for (uint8_t i = 0; i < m_nb_players; i++){
        if (current->player.get_id() == player_id){
            return &(current->player);
        }
        current = current->next;
    }
    return NULL;
}


void Team::display_data() const {
    Serial.println("\n\nTeam data:");
    team_data* current = m_players;
    for (uint8_t i = 0; i < m_nb_players; i++){
        Serial.print("Player name: ");
        Serial.print(current->player.get_name());
        Serial.print(", id: ");
        Serial.print(current->player.get_id());
        Serial.print(", best time: ");
        Serial.println(current->player.getBestTime());
        current = current->next;
    }

    Serial.println(" ");
    Serial.print("Number of players: ");
    Serial.println(m_nb_players);
    Serial.print("Number of max players: ");
    Serial.println(m_nb_max_players);
    Serial.print("Id of active player: ");
    Serial.println(m_id_active_player);
    Serial.print("Max id player: ");
    Serial.println(m_max_id_player);
    Serial.print("Number of checkpoints: ");
    Serial.println(m_nb_checkpoints);

    Serial.println("End of team data\n\n");
}