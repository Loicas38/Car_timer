#include "Checkpoint.hpp"


Checkpoint::Checkpoint(int id) : m_id(id), m_on(true), m_used(true)
{
}

Checkpoint::Checkpoint(int id, bool on, bool used): m_id(id), m_on(on), m_used(used)
{

}

void Checkpoint::update_id(int id){
    m_id = id;
}

void Checkpoint::envoieMessage(String message) const{

    String mes = "#";
    mes += String(m_id);
    mes += ":";
    mes += message;
    Serial2.println(mes);
}


int Checkpoint::get_id() const {
    return m_id;
}

void Checkpoint::switch_on(){
    m_on = true;
    envoieMessage("on");
}

void Checkpoint::switch_off(){
    m_on = false;
    envoieMessage("off");
}

bool Checkpoint::is_on(){
    return m_on;
}

void Checkpoint::measure_distance() const{
    envoieMessage("measure_distance");
}


Checkpoint::~Checkpoint()
{
}