#include <iostream>
#include <string.h>

int main(int argc, char* argv[])
{
    const size_t BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE] = "{\"id\":0,\"i\":0,\"n\":1,\"d\":\"00S(I00S(C00J/E00A-y%n0fm000Cf00S(G00S(A00J/C00A-x%n0fk000Ce00S(G00S(A00J/C00A-x%n0fk000Cd00S(H~";

    size_t length = strlen(buffer); 
    std::cout << "before " << length << std::endl; 

    snprintf(buffer + length, BUFFER_SIZE - length, "\"}");

    std::cout << "after " << strlen(buffer) << std::endl; 

    std::cout << buffer << std::endl;    
}