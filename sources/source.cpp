// Copyright 2018 Your Name <your_email>

#include <client.hpp>

boost::asio::io_service service;
std::recursive_mutex mutex;

void talk_to_svr::connect(boost::asio::ip::tcp::endpoint ep) {
    sock_.connect(ep);
}

void talk_to_svr::loop() {
    write("login " + username_ + "\n");
    read_answer();
    while ( started_)
    {
        write_request();
        read_answer();
        std::srand(std::time(0)); 
        boost::this_thread::sleep
        (boost::posix_time::millisec(std::rand() % 7000));
    }
}

void talk_to_svr::read_answer() {
    for (unsigned i = 0; i < max_msg; i++)
        buff_[i] = ' ';
    read(sock_, boost::asio::buffer(buff_),
         boost::bind(&talk_to_svr::read_complete, this, _1));
    process_msg();
}

void talk_to_svr::process_msg()
{
    std::string msg = "";
    for (unsigned i = 0; i < max_msg; i++) {
        if (buff_[i] == '\n') {
            msg += buff_[i];
            break;
        }
        msg += buff_[i];
    }
    std::lock_guard<std::recursive_mutex> lock(mutex);
    if ( msg.find("login ") == 0) on_login(msg);
    else if ( msg.find("ping") == 0) on_ping(msg);
    else if ( msg.find("clients ") == 0) on_clients(msg);
    else
        std::cerr << "invalid msg " << msg << std::endl;
}

void talk_to_svr::on_ping(const std::string & msg)
{
    std::lock_guard<std::recursive_mutex> lock(mutex);
    std::cout << msg << std::endl;
    std::istringstream in(msg);
    std::string answer;
    in >> answer >> answer;
    if ( answer == "client_list_changed")
        do_ask_clients();
}

void talk_to_svr::on_clients(const std::string & msg)
{
    std::string clients = msg.substr(8);
    std::lock_guard<std::recursive_mutex> lock(mutex);
    std::cout << username_ << ", new client list:"
              << clients << std::endl;
}

void talk_to_svr::do_ask_clients()
{
    write("ask_clients\n");
    read_answer();
}

void talk_to_svr::write(const std::string & msg) {
    std::lock_guard<std::recursive_mutex> lock(mutex);
    sock_.write_some(boost::asio::buffer(msg));
}

size_t talk_to_svr::read_complete(
    const boost::system::error_code & err) {
    if (err) return 0;
    unsigned enter = 0;
    for (unsigned i = 0; i < max_msg; i++) {
        if (buff_[i] == '\n') enter = i;
    }
    bool found = false;
    if (enter != 0) found = true;
    return found ? 0 : 1;
}



