#ifndef SERVER_H
#define SERVER_H

#include <functional>
#include <thread>

class Server
{
public:
    using Msg_callback = std::function<void(const std::string& msg)>;
    Server(int port);

    void set_msg_callback(Msg_callback msg_callback)
    {
        msg_callback_ = std::move(msg_callback);
    }

private:

    void run() const;

    Msg_callback msg_callback_;
    int port_;
    

};

#endif // SERVER_H