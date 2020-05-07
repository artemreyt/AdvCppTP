#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

namespace HttpFramework {

    const std::map<uint16_t, std::string> g_status_info {
            {200, "OK"},
            {404, "Not Found"},
            {400, "Bad Request"}
    };

    class HttpResponse {
    public:
        explicit HttpResponse(const std::string &version, uint16_t status_code);
        void setVersion(const std::string &version);
        void setHeader(const std::string &header_name, const std::string &value);

    private:
        std::string version_;
        uint16_t    status_code_;
        std::map<std::string, std::string> headers_;
    };
}


#endif // HTTPRESPONSE_H
