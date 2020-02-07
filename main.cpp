#include <iostream>
#include "Movies.h"

int main(){
    int selection {1};
    Movies MyFav;

    while(selection >=1 & selection <3){
        std::cout << "What you want to do?" << std::endl;
        std::cout << "Do you want to add new film? Press 1" << std::endl;
        std::cout << "To list all film, please press 2" << std::endl;

        std::cin >> selection;

        switch(selection){
        case 1:
            {
            std::string movie_name;
            int movie_rate;
            int watch_count;

            std::cout << "Type the movie name" << std::endl;
            std::cin >> movie_name;
            std::cout << "What is your rate for this movie? 0 to 10?" << std::endl;
            std::cin >> movie_rate;
            std::cout << "How many times have you watched this film?" << std::endl;
            std::cin >> watch_count;

            MyFav.add_movie(movie_name, movie_rate, watch_count);

            break;
            }
        case 2:
            {
                MyFav.get_movies();
                break;
            }
        default:
            std::cout << "Unexpected value!";
            break;
        }
    }

    return 0;
}
