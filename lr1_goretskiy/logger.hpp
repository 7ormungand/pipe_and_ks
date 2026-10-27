#pragma once
#include <iostream>
#include <fstream>
#include <streambuf>

inline std::streambuf* orig_cout = nullptr;
inline std::streambuf* orig_cin = nullptr;
inline std::ofstream log_file_stream;


class TeeBuf : public std::streambuf {
    std::streambuf* sb1;
    std::streambuf* sb2;
protected:
    virtual int overflow(int c) override {
        if (c == EOF) return EOF;
        int r1 = sb1->sputc(c);
        int r2 = sb2->sputc(c);
        return (r1 == EOF || r2 == EOF) ? EOF : c;
    }
    virtual int sync() override {
        int r1 = sb1->pubsync();
        int r2 = sb2->pubsync();
        return (r1 == 0 && r2 == 0) ? 0 : -1;
    }
public:
    TeeBuf(std::streambuf* s1, std::streambuf* s2) : sb1(s1), sb2(s2) {}
};

class TeeInputBuf : public std::streambuf {
    std::streambuf* sb1;
    std::ostream* sb2;
protected:
    virtual std::streamsize xsgetn(char* s, std::streamsize n) override {
        std::streamsize count = sb1->sgetn(s, n);
        if (sb2 && sb2->good() && count > 0) {
            sb2->write(s, count);
            sb2->flush();
        }
        return count;
    }

    virtual int underflow() override {
        return sb1->sgetc();
    }

    virtual int uflow() override {
        int c = sb1->sbumpc();
        if (c == EOF) {
            return EOF;
        }
        if (sb2 && sb2->good()) {
            *sb2 << static_cast<char>(c);
            sb2->flush();
        }
        return c;
    }
public:
    TeeInputBuf(std::streambuf* s1, std::ostream* s2) : sb1(s1), sb2(s2) {}
};

void start_log(const std::string& filename = "log.txt") {
    log_file_stream.open(filename, std::ios::app);
    if (!log_file_stream.is_open()) {
        std::cerr << "Не удалось открыть файл для логов!\n";
        return;
    }

    orig_cout = std::cout.rdbuf();
    static TeeBuf tee_buf(orig_cout, log_file_stream.rdbuf());
    std::cout.rdbuf(&tee_buf);
    std::cout << "Начало логирования\n";

    orig_cin = std::cin.rdbuf();
    static TeeInputBuf tee_in_buf(orig_cin, &log_file_stream);
    std::cin.rdbuf(&tee_in_buf);
}

void stop_logging() {
    if (orig_cout) {
        std::cout.rdbuf(orig_cout);
    }
    
    if (orig_cin) {
        std::cin.rdbuf(orig_cin);
    }

    if (log_file_stream.is_open()) {
        log_file_stream.close();
    }
}