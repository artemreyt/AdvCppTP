#include "Server.hpp"
#include "Logger.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include <string>

const std::string ip = "0.0.0.0";
const uint16_t port = 8000;
const std::string log_file = "log.txt";
Logger::StdoutLogger stdout_logger(Logger::t_level::WARNING);
//Logger::FileLogger file_logger("log.txt", Logger::t_level::INFO);

class MyServer: public HttpFramework::Server {
public:
    MyServer(): HttpFramework::Server(ip, port, stdout_logger) {}

    HttpFramework::HttpResponse onRequest(const HttpFramework::HttpRequest &request) override {
        if (request.get_method() == "GET") {
            std::string comment;
            for (const auto &[key, value]: request.get_params()) {
                comment += key + ":" + value + " ";
            }
            logger_.debug("[USER]: GET request got with params: " + comment);

            HttpFramework::HttpResponse response(request.get_version(), 200);
            response.setHeader("Content-Type", "text/plain");
            std::string msg = "Hello, world!";
            response.setHeader("Content-Length", std::to_string(msg.length()));
            response.setBody(msg);
            return response;
        }
        return HttpFramework::HttpResponse("1.1");
    }
};

int main() {
    MyServer server;
    server.run();
}