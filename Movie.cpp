#include "Movie.h"

Movie::Movie(std::string movie_name, int movie_rate, int watch_count){
    c_movie_name = movie_name;
    c_rate = movie_rate;
    c_watch_count = watch_count;
}

void Movie::set_movie_name(std::string movie_name){
    c_movie_name = movie_name;
}
void Movie::set_rate(int movie_rate){
    c_rate = movie_rate;
}

void Movie::set_watch_count(int watch_count){
    c_watch_count = watch_count;
}

std::string Movie::get_movie_name(){
    return c_movie_name;
}

int Movie::get_rate(){
    return c_rate;
}

int Movie::get_watch_count(){
    return c_watch_count;
}
