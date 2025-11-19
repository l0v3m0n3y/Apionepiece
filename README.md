# Apionepiece
api for api-onepiece.com one piece anime site

# main
```cpp
#include "Apionepiece.h"
#include <iostream>

int main() {
   Apionepiece api;

    auto chapters = api.chapters_list().then([](json::value result) {
        std::cout << result<< std::endl;
    });
    chapters.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```

