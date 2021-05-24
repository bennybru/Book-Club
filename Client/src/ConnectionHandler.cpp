#include <ConnectionHandler.h>

using boost::asio::ip::tcp;

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

ConnectionHandler::ConnectionHandler() :
        host_(), port_(), io_service_(), socket_(io_service_) {}

ConnectionHandler::~ConnectionHandler() {
    close();
}

void ConnectionHandler::setHostPort(std::string host, short port) {
    host_ = host;
    port_ = port;
}

bool ConnectionHandler::connect() {
    std::cout << "Starting connect to "
              << host_ << ":" << port_ << std::endl;
    try {
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_);
        boost::system::error_code error;
        socket_.connect(endpoint, error);
        if (error)
            throw boost::system::system_error(error);
    }
    catch (std::exception &e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
    boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp) {
            tmp += socket_.read_some(boost::asio::buffer(bytes + tmp, bytesToRead - tmp), error);
        }
        if (error)
            throw boost::system::system_error(error);
    } catch (std::exception &e) {
        std::cerr << "recv failed1 (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {
    int tmp = 0;
    boost::system::error_code error;
    try {
        while (!error && bytesToWrite > tmp) {
            tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
        }
        if (error)
            throw boost::system::system_error(error);
    } catch (std::exception &e) {
        return false;
    }
    return true;
}


bool ConnectionHandler::getLine(std::string &line) {
    return getFrameAscii(line, '\0');
}


bool ConnectionHandler::sendLine(std::string &line) {
    return sendFrameAscii(line, '\0');
}

bool ConnectionHandler::getFrameAscii(std::string &frame, char delimiter) {
    char ch;
    try {
        do {
            if (!getBytes(&ch, 1)) {
                return false;
            }
            if (ch != '\0')
                frame.append(1, ch);
        } while (delimiter != ch);
    } catch (std::exception &e) {
        return false;
    }
    return true;
}

bool ConnectionHandler::sendFrameAscii(const std::string &frame, char delimiter) {
    sendLock.lock();
    bool result = sendBytes(frame.c_str(), frame.length());
    if (!result) {
        sendLock.unlock();
        return false;
    }
    result = sendBytes(&delimiter, 1);
    sendLock.unlock();
    return result;
}

void ConnectionHandler::close() {
    try {
        socket_.close();
    } catch (...) {
    }
}
