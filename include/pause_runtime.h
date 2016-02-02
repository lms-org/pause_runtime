#ifndef PAUSE_RUNTIME_H
#define PAUSE_RUNTIME_H

#include <lms/module.h>
#include <thread>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <mutex>

/**
 * @brief LMS module pause_runtime
 *
 * http://stackoverflow.com/questions/1798511/how-to-avoid-press-enter-with-any-getchar
 **/
class PauseRuntime : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    std::string runtimeName;
    std::mutex mutex;
    bool running;
    bool stopNextCycle;

    std::thread thread;
    fd_set fds;
    termios oldt, newt;
};

#endif // PAUSE_RUNTIME_H
