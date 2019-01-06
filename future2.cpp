#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>


// Monitor = Thread-safe object like the following:
class SemiFuture
{
public:
    SemiFuture() : done{false} {}

    void set(int answer)
    {
        std::lock_guard<std::mutex> lk(m);
        value = answer;
        done = true;
        cv.notify_one();
    }

    int get()
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this]{ return done; });
        return value;
    }

private:
    int value;
    bool done;
    mutable std::mutex m;
    mutable std::condition_variable cv;
};


void ask_the_question(SemiFuture& answer)
{
    // Start doing a really long calculation.
    std::this_thread::sleep_for((std::chrono::seconds(4)));

    answer.set(42);
}


void use_the_answer(SemiFuture& answer)
{
    std::cout << "The answer to life, the universe, and everything is "
              << answer.get() << std::endl;
}


int main()
{
    SemiFuture answer;

    // Start the long calculation.
    std::thread calculator{ask_the_question, std::ref(answer)};

    // Do some stuff in the meantime.
    std::cout << "Doing stuff..." << std::endl;
    std::this_thread::sleep_for((std::chrono::seconds(1)));    
    std::cout << "More stuff..." << std::endl;

    // Now we really need the answer...
    use_the_answer(answer);   

    calculator.join();
    return 0;
}