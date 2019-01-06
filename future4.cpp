#include <iostream>
#include <thread>
#include <chrono>
#include <future>

// std::future without utilizing a std::promise at all:


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
	// std::packaged_task builds a function object which creates the std::promise:
    std::packaged_task<int()> question_task(ask_the_question);

    std::future<int> answer = question_task.get_future();
    std::thread t{std::move(question_task)};

    // Do some stuff in the meantime.
    std::cout << "Doing stuff..." << std::endl;
    std::this_thread::sleep_for((std::chrono::seconds(1)));    
    std::cout << "More stuff..." << std::endl;

    // Now we really need the answer...
    use_the_answer(answer.get());

    t.join();
    return 0;
}