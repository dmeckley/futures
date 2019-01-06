#include <iostream>
#include <thread>
#include <chrono>
#include <future>


int ask_the_question()
{
    // Start doing a really long calculation.
    std::this_thread::sleep_for((std::chrono::seconds(4)));

    return 42;
}


void use_the_answer(int answer)
{
    std::cout << "The answer to life, the universe, and everything is "
              << answer << std::endl;
}


int main()
{
	// std::async which creates a packaged future:
    std::future<int> answer = std::async(ask_the_question);

    // Do some stuff in the meantime.
    std::cout << "Doing stuff..." << std::endl;
    std::this_thread::sleep_for((std::chrono::seconds(4)));    
    std::cout << "More stuff..." << std::endl;

    // Now we really need the answer...
    use_the_answer(answer.get());

    return 0;
}