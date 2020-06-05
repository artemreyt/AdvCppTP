#include "Server.hpp"
#include "Logger.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "constants.hpp"
#include <string>

using HttpFramework::http_structures::HttpResponse, HttpFramework::http_structures::HttpRequest;

const std::string ip = "0.0.0.0";
const uint16_t port = 8000;
const std::string log_file = "log.txt";
Logger::StdoutLogger stdout_logger(Logger::t_level::WARNING);

class MyServer: public HttpFramework::Server::Server {
public:
    MyServer(): HttpFramework::Server::Server(ip, port, stdout_logger) {}

    HttpResponse onRequest(const HttpRequest &request) override {

        if (request.get_first_line().get_method() == HttpFramework::constants::RequestMethod::GET) {
            std::string comment;
            for (const auto &[key, value]: request.get_params()) {
                comment += (key + " -> ").append(value + " | ");
            }
            logger_.debug("[USER]: GET request got with params: " + comment);

            HttpResponse response(request.get_first_line().get_version(), 200);
            response.setHeader("Content-Type", "text/plain");
            std::string msg = "Hello, world!";
            response.setBody(msg);
            return response;
        }
        return HttpResponse(HttpFramework::constants::HttpVersion::V1_1);
    }
};

int main() {
    MyServer server;
    server.run(4);
}