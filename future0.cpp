#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>


int ask_the_question()
{
    // Start doing a really long calculation.
    std::this_thread::sleep_for((std::chrono::seconds(4)));

    return 42;
}


void use_the_answer(int answer)
{
    std::cout << "The answer to life, the universe, and everything is ";
    std::cout << answer << std::endl;
}


int main()
{
    // Start the long calculation.
    int answer = ask_the_question();

    // Do some other stuff.
    std::cout << "Doing stuff..." << std::endl;
    std::this_thread::sleep_for((std::chrono::seconds(1)));    
    std::cout << "More stuff..." << std::endl;

    // Now we really need the answer...
    use_the_answer(answer);
    
    return 0;
}