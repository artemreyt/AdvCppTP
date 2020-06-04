#ifndef HTTPFRAMEWORK_EPOLLMANAGER_HPP
#define HTTPFRAMEWORK_EPOLLMANAGER_HPP

#include "Server.hpp"

namespace HttpFramework::Server {

    class EpollManager {
    public:
        explicit EpollManager(Server &server);

        EpollManager(EpollManager &&other) = default;

        EpollManager(const EpollManager &other) = delete;

        void run();

        void operator()();

        void operator=(const EpollManager &other) = delete;

    private:
        void addNewConnection(Connection &&connection);

        void deleteConnection(int id);

        void addEvent(int fd, epoll_event &Event);

        void changeEvent(int id, uint32_t new_event);

        void acceptClients();

        void handleClient(const epoll_event &event);

        void clientRoutine();

        void checkTimeouts();


        struct RoutineInfo {
            Connection con;
            uint32_t current_event;
            high_resolution_clock::time_point timeout;
        };

        Descriptor::Descriptor epollObject_;
        std::unordered_map<int, RoutineInfo> routines_;
        Server &server_;
    };
}
#endif //HTTPFRAMEWORK_EPOLLMANAGER_HPP
