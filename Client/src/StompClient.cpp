#include <ConnectionHandler.h>
#include <thread>
#include <LineHandler.h>
#include <StompMessagingProtocol.h>
#include <ServerHandler.h>

using std::string;
using std::thread;

int main(int argc, char *argv[]) {


    ConnectionHandler connectionHandler;
    User user;
    thread keyboardThread([&]() {
        LineHandler lineHandler(user, connectionHandler);

        while (!lineHandler.shouldTerminate()) {
            string line;
            std::getline(std::cin, line);
            lineHandler.process(line);
        }
    });

    thread incomingMsgsThread([&]() {
        StompMessagingProtocol protocol(user, connectionHandler);
        ServerHandler serverHandler(protocol);

        while (!serverHandler.shouldTerminate()) {
            if (user.isLoggedIn()) {
                string answer;
                if (!connectionHandler.getLine(answer)) {
                    std::cout << "Disconnected. Exiting...\n" << std::endl;
                    break;
                }
                Frame frame(answer);
                serverHandler.process(frame);
            }
        }
    });

    keyboardThread.join();
    incomingMsgsThread.join();

    user.clear();
    return 0;
}
