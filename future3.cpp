#include <iostream>
#include <thread>
#include <chrono>
#include <future>

// Utilization of an actual future:

// std::promise utilized:
void ask_the_question(std::promise<int>& p)
{
    // Start doing a really long calculation.
    std::this_thread::sleep_for((std::chrono::seconds(4)));

    p.set_value(42);
}

// std::future utilized:
void use_the_answer(std::future<int>& answer)
{
    std::cout << "The answer to life, the universe, and everything is "
              << answer.get() << std::endl;
}


int main()
{
	// std::promise & std::future for the std::future structure:
    std::promise<int> promise;
    std::future<int> answer = promise.get_future();

    // Start the long calculation.
    std::thread calculator{ask_the_question, std::ref(promise)};

    // Do some stuff in the meantime.
    std::cout << "Doing stuff..." << std::endl;
    std::this_thread::sleep_for((std::chrono::seconds(1)));    
    std::cout << "More stuff..." << std::endl;

    // Now we really need the answer...
    use_the_answer(answer);

    calculator.join();
    return 0;
}