#pragma once

#include <boost/asio.hpp>
#include <string>

class SocketConnection {
public:
    SocketConnection() = default;

    bool initialize(boost::asio::ip::tcp::socket&& socket);

    bool send(const std::string& data);

    bool receive(std::string& outData,
                 size_t maxBytes = 4096);

    bool close();

    bool isConnected() const;

private:
    boost::asio::ip::tcp::socket m_socket{
        *(new boost::asio::io_context)
    };

    bool m_connected = false;
};

