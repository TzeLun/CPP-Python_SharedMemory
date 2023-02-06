#include "interprocess.h"

// Demonstration of inter-script communication between C++ and Python 
int main() {
    SharedMemory* shm = new SharedMemory("force", 4096, true);

    if (shm->fail()) {
        delete shm;
        return 1;
    }

    shm->write("shm created");

    // on hold until the python side accesses this shared memory
    PROCESS::wait(shm, "accessed");

    std::cout << "CONNECTION WITH PYTHON SIDE ESTABLISHED. DO INPUT A MESSAGE (HIT 'ENTER' KEY TO QUIT):" << std::endl;

    // sending input message continuously to shared memory
    std::string text;
    std::getline(std::cin, text);
    while (! text.empty()) {
        shm->write(text);
        std::getline(std::cin, text);
    }

    // Notify python side for termination
    shm->write("terminate");

    // Wait till python side acknowledge the termination
    PROCESS::wait(shm, "affirmative");

    // Sleep for 1 second to allow proper memory clean up at Python's side
    // requires #include <unistd.h> - included in interprocess.h
    sleep(1);

    delete shm;

    return 0;
}