// For discussion only; this is not working code!

#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>

// A queue of work for the main thread to execute.
std::mutex m;
std::deque<std::packaged_task<void()>> tasks;


void update_progress_counter(double percent);
void switch_to_menu_display();


void thread_that_loads_things()
{
    // Load the first half of the things.
    load_things();

    // Update the GUI.
    std::packaged_task<void(double)> task{update_progress_counter, 0.5};
    std::unique_lock<std::mutex> lk(m);
    tasks.push_back(std::move(task));
    lk.unlock();

    // Load the second half of the things.
    load_things();

    // Update the GUI again.
    std::packaged_task<void(double)> task{update_progress_counter, 1.0};
    lk.lock();
    tasks.push_back(std::move(task));
    lk.unlock();
}


void thread_that_switches_modes()
{
    // Get input from the user in game mode.
    get_clicks_to_shoot();

    // Switch the screen to menu mode.
    std::packaged_task<void()> task{switch_to_menu_display};
    auto switch_done = task.get_future();
    std::unique_lock<std::mutex> lk(m);
    tasks.push_back(std::move(task));
    lk.unlock();

    // Wait for the switch.
    switch_done.wait();

    // Get input from the user in menu mode;
    get_clicks_for_menu();
}



int main()
{
    // Launch threads to handle disk caching, user input, etc...

    while(!done)
    {
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);
            if (tasks.empty())
                continue;
            task = std::move(tasks.front());
            tasks.pop_front();
        }
        task();
    }
}