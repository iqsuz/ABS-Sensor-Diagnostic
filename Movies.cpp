#include "Movies.h"

void Movies::add_movie(std::string name, int rate, int watch_count){
    Movie new_movie(name,rate,watch_count);
    v_Movies.push_back(new_movie);
}
void Movies::get_movies(){
    for(Movie temp_mov : v_Movies){
        std::cout << temp_mov.get_movie_name() << std::endl;
    }
}
