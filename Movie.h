#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <string>

class Movie{
    private:
        std::string c_movie_name;
        int c_rate;
        int c_watch_count;
    public:
        Movie(std::string movie_name, int movie_rate = 0, int watch_count = 0);
        Movie();
        void set_movie_name(std::string movie_name);
        void set_rate(int movie_rate);
        void set_watch_count(int watch_count);
        std::string get_movie_name();
        int get_rate();
        int get_watch_count();
};

#endif // MOVIES_H
