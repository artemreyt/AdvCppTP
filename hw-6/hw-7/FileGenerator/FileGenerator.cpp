#include "Logger.hpp"
#include "Data.hpp"
#include <fstream>
#include <string>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <numeric>

using std::string_literals::operator""s;
static const size_t GIGABYTE = 1UL << 30;

static const size_t MAX_GIGABYTES = 20;
static const size_t MAX_BYTES_VECTOR = GIGABYTE / 2;
static const size_t MAX_ELEMENTS_IN_VECTOR = MAX_BYTES_VECTOR / sizeof(Key_t);

class FileGenerator {
public:
    explicit FileGenerator(std::string filename, size_t filesize):
        stream_(filename, std::ios::out | std::ios::binary),
        filename_(std::move(filename)), filesize_(filesize),
        generator_(std::random_device()()) {

        if (stream_.is_open()) {
            Logger::info(filename + " successfully opened");
        } else {
            std::string msg = "Fail to open "s.append(filename);
            Logger::error(msg);
            throw std::runtime_error(msg);
        }
    }

    void run() {
        size_t elem_number = filesize_ / sizeof(std::pair<Key_t, Data>);

        Logger::info("Need to create "s.append(std::to_string(elem_number).append(" elements")));

        size_t written = 0;
        double next_percent = 0.01;
        size_t elems_to_create = elem_number > MAX_ELEMENTS_IN_VECTOR ? MAX_ELEMENTS_IN_VECTOR
                    : elem_number;

        Logger::info("Start writing to "s.append(filename_));
        while (elem_number - written > 0) {

            Logger::info("Preparing vector with "s.append(std::to_string(elems_to_create)
                                                        .append(" elements")));
            auto keys = generate_vector(elems_to_create);
            Logger::info("Vector with "s.append(std::to_string(elems_to_create)
                                                        .append(" elements created")));
            for (auto &key: keys) {
                Data data{};
                randomInitData(data);

                auto pair = std::make_pair(key, data);

                try {
                    stream_.write(reinterpret_cast<char *>(&pair), sizeof(pair));
                } catch (const std::ios_base::failure &err) {
                    std::string msg = "Fail to write pair";
                    Logger::error(msg);
                    throw std::runtime_error(msg);
                }

                double current_percent = static_cast<double>(++written) / elem_number;
                if (current_percent >= next_percent) {
                    Logger::info("Written "s.append(std::to_string(written).append(" elements (")
                                .append(std::to_string(current_percent * 100)).append("%)")));
                    next_percent += 0.01;
                }
            }
            elems_to_create = elem_number - written > MAX_ELEMENTS_IN_VECTOR ? MAX_ELEMENTS_IN_VECTOR :
                    elem_number - written;
        }
    }

private:

    void randomInitData(Data &data) {
        for (auto &elem: data.payload) {
            elem = generator_();
        }
    }

    std::vector<Key_t> generate_vector(size_t elem_number) {
        static size_t start = 0;
        std::vector<Key_t> vec(elem_number, Key_t());

        std::iota(vec.begin(), vec.end(), start);
        start += elem_number;

        std::shuffle(vec.begin(), vec.end(), generator_);

        return vec;
    }


    std::ofstream   stream_;
    std::string     filename_;
    size_t          filesize_;
    std::mt19937    generator_;
};


int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: "<< argv[0] << " [filename] [number(of GB)]" << std::endl;
        return 1;
    } else {
        std::string filename = argv[1];
        size_t      gigabytes = std::stoull(argv[2]);

        if (!gigabytes) {
            std::cerr << "gygabytes must be a number and 0 < n <= " << MAX_GIGABYTES << std::endl;
            return 2;
        }

        FileGenerator file_generator(filename, gigabytes * GIGABYTE);
        file_generator.run();
    }
}
