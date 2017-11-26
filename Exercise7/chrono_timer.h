#pragma once

#include <string>
#include <chrono>
#include <iostream>

class chrono_timer {
  public:
    explicit chrono_timer(const std::string& name = "Unnamed")
		: name_(name), start_(std::chrono::high_resolution_clock::now()),
		stopped_(false) {
	}
	~chrono_timer() {
        if(!stopped_) print();
	}
	long long getTime() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_).count();
	}

    void print() const
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_);
        std::cout << name_ << ": " << elapsed.count() << " ms " << std::endl;
    }

    void stop()
    {
        stopped_ = true;
    }

private:
    const std::string name_;
    const std::chrono::time_point<std::chrono::high_resolution_clock> start_;
    bool stopped_;
};
