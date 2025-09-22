//play music from wav using SFML
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>

int main()
{
    // Load a music to play

    sf::Music music;
    if (!music.openFromFile("../src/test.wav")) {
	    std::cout << "bruh" << std::endl;
	    system("pwd");
	    return 2;
    }

    // Play the music
    music.play();

    // Loop while the music is playing
    while (music.getStatus() == sf::Music::Playing)
    {
	// Leave some CPU time for other processes
	sf::sleep(sf::seconds(1));

	// Display the playing position
	std::cout << "\rPlaying... " << floor(music.getPlayingOffset().asSeconds() + 0.5) << " sec";
	std::cout << std::flush;
    }
    std::cout << std::endl << std::endl;

    return EXIT_SUCCESS;
}