#include "Movie.h"

Movie::Movie(std::string m_name, int m_rate, int watch_count){
    m_movie_name = m_name;
    m_rate =
}

void Movie::set_movie_name(std::string m_name){
    m_movie_name = m_name;
}
void Movie::set_rate(int rate){
    m_rate = rate;
}

void Movie::set_watch_count(int watch_count){
    m_watch_count = watch_count;
}

std::string Movie::get_movie_name(){
    return m_movie_name;
}

int Movie::get_rate(){
    return m_rate;
}

int Movie::get_watch_count(){
    return m_watch_count;
}
