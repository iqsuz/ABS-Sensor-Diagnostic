#ifndef MOVIES_H
#define MOVIES_H

#include <iostream>
#include <vector>
#include <string>
#include "Movie.h"


class Movies{
    private:
        std::vector<Movie> v_Movies;
    public:
        void add_movie(std::string name, int rate, int watch_count);
        void get_movies();
};

#endif // MOVIES_H



