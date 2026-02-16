#include "SmtpSession.hpp"
#include "SmtpParser.hpp"

SmtpSession::SmtpSession()
    : state(SmtpState::WAIT_HELO)
{}

std::string SmtpSession::processCommand(const std::string& input)
{
    if (state == SmtpState::RECEIVING_DATA)
    {
        if (input == ".")
        {
            state = SmtpState::WAIT_MAIL;
            return "250 Message accepted\r\n";
        }

        return "";
    }

    auto cmd = SmtpParser::parse(input);

    switch (state)
    {
        case SmtpState::WAIT_HELO:
            if (cmd.type == SmtpCommandType::HELO ||
                cmd.type == SmtpCommandType::EHLO)
            {
                state = SmtpState::WAIT_MAIL;
                return "250 Hello\r\n";
            }
            return "503 Bad sequence\r\n";

        case SmtpState::WAIT_MAIL:
            if (cmd.type == SmtpCommandType::MAIL)
            {
                state = SmtpState::WAIT_RCPT;
                return "250 OK\r\n";
            }
            return "503 Bad sequence\r\n";

        case SmtpState::WAIT_RCPT:
            if (cmd.type == SmtpCommandType::RCPT)
            {
                return "250 OK\r\n";
            }
            else if (cmd.type == SmtpCommandType::DATA)
            {
                state = SmtpState::RECEIVING_DATA;
                return "354 End data with <CR><LF>.<CR><LF>\r\n";
            }
            return "503 Bad sequence\r\n";

        case SmtpState::CLOSED:
            return "221 Bye\r\n";

        default:
            break;
    }

    if (cmd.type == SmtpCommandType::QUIT)
    {
        state = SmtpState::CLOSED;
        return "221 Bye\r\n";
    }

    return "500 Syntax error\r\n";
}

