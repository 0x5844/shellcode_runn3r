#include <iostream>
#include <fstream>
#include <vector>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>
#include <cstdlib>

void executeShellcode(const std::vector<unsigned char>& shellcode) {
    // Get the system page size
    size_t pagesize = sysconf(_SC_PAGESIZE);
    size_t shellcode_size = shellcode.size();

    if (shellcode_size == 0) {
        std::cerr << "Shellcode is empty, nothing to execute." << std::endl;
        return;
    }

    // Allocate memory with RW permissions
    void* exec_mem = mmap(nullptr, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (exec_mem == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    // Copy shellcode into the allocated memory
    std::memcpy(exec_mem, shellcode.data(), shellcode_size);

    // Mark memory as executable
    if (mprotect(exec_mem, pagesize, PROT_READ | PROT_EXEC) == -1) {
        perror("mprotect failed");
        munmap(exec_mem, pagesize);
        exit(EXIT_FAILURE);
    }

    // Cast memory to a function pointer and execute it
    auto shellcode_func = reinterpret_cast<void (*)()>(exec_mem);
    std::cout << "Executing shellcode..." << std::endl;
    shellcode_func();

    if (munmap(exec_mem, pagesize) == -1) {
        perror("munmap failed");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <shellcode_file>" << std::endl;
        return EXIT_FAILURE;
    }

    const char* filepath = argv[1];
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return EXIT_FAILURE;
    }

    // Get the size of the file and read its contents
    std::streamsize filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (filesize <= 0) {
        std::cerr << "File is empty or unreadable." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<unsigned char> shellcode(filesize);
    if (!file.read(reinterpret_cast<char*>(shellcode.data()), filesize)) {
        std::cerr << "Failed to read file contents." << std::endl;
        return EXIT_FAILURE;
    }

    file.close();

    // Execute the loaded shellcode
    executeShellcode(shellcode);

    return 0;
}
