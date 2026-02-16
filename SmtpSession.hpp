#pragma once
#include "SmtpCommand.hpp"

enum class SmtpState {
    WAIT_HELO,
    WAIT_MAIL,
    WAIT_RCPT,
    WAIT_DATA,
    RECEIVING_DATA,
    CLOSED
};

class SmtpSession {
public:
    SmtpSession();

    std::string processCommand(const std::string& input);

private:
    SmtpState state;
};

