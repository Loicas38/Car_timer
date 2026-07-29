#include "Player.hpp"


Player::Player() : m_times(), m_nb_checkpoints(0), m_checkpoint_times(NULL)
{

}


Player::Player(String name, uint8_t nb_checkpoints, uint8_t player_id): m_name(name), m_times(), m_nb_checkpoints(nb_checkpoints), m_checkpoint_times(NULL), m_player_id(player_id)
{
    m_checkpoint_times = new Time[nb_checkpoints];
    for(int i = 0; i < nb_checkpoints; i++){
        m_checkpoint_times[i] = Time();
    }
}

Player::~Player()
{
 // pas d'allocations dynamiques pour le moment donc pas de nettoyage à faire
}


uint32_t Player::getBestTime() const{
    return m_times.minimum_time();
}


uint32_t Player::getBestTimeSeconds() const{
    return m_times.minimum_time_seconds();
}

uint32_t Player::getBestTimeMinutes() const{
    return m_times.minimum_time_minutes();
}


void Player::update_best(uint32_t new_best){
    m_times.add_new_time_delete_longest(new_best, 0);
}

void Player::update_best(uint32_t new_best, uint16_t race_id){
    m_times.add_new_time_delete_longest(new_best, race_id);
}


uint32_t Player::get_last_time() const{
    return m_times.get_time_highest_race_id();
}

uint16_t Player::get_last_time_seconds() const{
    return m_times.get_time_highest_race_id() / 1000;
}

uint16_t Player::get_last_time_minutes() const{
    return m_times.get_time_highest_race_id() / 60000;
}

void Player::add_new_time(uint32_t new_time, uint16_t race_id){
    m_times.add_new_time(new_time, race_id);
}

void Player::add_new_time(uint32_t new_time){
    m_times.add_new_time(new_time);
}


void Player::add_new_time_checkpoint(uint32_t new_time, uint8_t checkpoint_id){
    if (checkpoint_id >= m_nb_checkpoints){
        return;
    }

    m_checkpoint_times[checkpoint_id].add_new_time(new_time);
}


void Player::add_new_time_all_checkpoints(uint32_t* new_times, uint8_t nb_checkpoints){
    if(nb_checkpoints != m_nb_checkpoints){
        return;
    }
    
    for(int i = 0; i < nb_checkpoints; i++){
        add_new_time_checkpoint(new_times[i], i);
    }
}

String Player::get_name() const{
    return m_name;
}

void Player::reset_times(){
    m_times.delete_all_times();
    for(int i = 0; i < m_nb_checkpoints; i++){
        m_checkpoint_times[i].delete_all_times();
    }

    m_times.delete_all_times();
}


void Player::delete_best_time(){
    m_times.delete_shortest_time();
}

uint8_t Player::get_nb_times_saved() const{
    return m_times.get_nb_times();
}


uint8_t Player::get_id() const{
    return m_player_id;
}

uint8_t Player::set_id(uint8_t new_id){
    m_player_id = new_id;
}

uint32_t Player::get_time_checkpoint(uint8_t checkpoint_id) const{
    if (checkpoint_id >= m_nb_checkpoints){
        return 0;
    }

    return m_checkpoint_times[checkpoint_id].minimum_time();
}