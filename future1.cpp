#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>


int answer;
int done = false;
std::mutex m;
std::condition_variable answer_ready;


void ask_the_question()
{
    // Start doing a really long calculation.
    std::this_thread::sleep_for((std::chrono::seconds(4)));

    // The caculation is finished, we're done.
    std::lock_guard<std::mutex> lk(m);
    answer = 42;
    done = true;

    // Wake up anyone waiting for the answer.
    answer_ready.notify_one();
}


void use_the_answer()
{
    std::cout << "The answer to life, the universe, and everything is ";

    std::unique_lock<std::mutex> lk(m);
    answer_ready.wait(lk, []{ return done; });		// Waits for when done is true.
    std::cout << answer << std::endl;
}


int main()
{
    // Start the long calculation.
    std::thread calculator{ask_the_question};

    // Do some stuff in the meantime.
    std::cout << "Doing stuff..." << std::endl;
    std::this_thread::sleep_for((std::chrono::seconds(1)));    
    std::cout << "More stuff..." << std::endl;

    // Now we really need the answer...
    use_the_answer();
    
    calculator.join();
    return 0;
}