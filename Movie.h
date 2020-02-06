#ifndef MOVIE_H
#define MOVIE_H

#include <iostream>
#include <string>

class Movie{
    private:
        std::string m_movie_name;
        int m_rate;
        int m_watch_count;
    public:
        Movie(std::string m_name, int m_rate, int watch_count);
        void set_movie_name(std::string m_name);
        void set_rate(int rate);
        void set_watch_count(int watch_count);
        std::string get_movie_name();
        int get_rate();
        int get_watch_count();
};

#endif // MOVIES_H
