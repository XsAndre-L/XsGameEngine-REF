// #include <arpa/inet.h>
// #include <cstring>
// #include <iostream>
// #include <netinet/in.h>
// #include <unistd.h>

// int main()
// {
//     int server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     sockaddr_in address {};
//     int opt = 1;
//     setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = inet_addr("127.0.0.1");
//     address.sin_port = htons(3306);

//     if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
//         perror("Bind failed");
//         return 1;
//     }

//     listen(server_fd, 1);
//     std::cout << "Fake MySQL server listening on 127.0.0.1:3306..." << std::endl;

//     int addrlen = sizeof(address);
//     int client_fd = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

//     if (client_fd < 0) {
//         perror("Accept failed");
//         return 1;
//     }

//     // --- Step 1: Send MySQL Handshake Packet ---
//     unsigned char handshake[] = {
//         0x0A, // Protocol version
//         '5', '.', '7', '.', '0', '-', 'f', 'a', 'k', 'e', 0x00, // Server version + null
//         0x01, 0x00, 0x00, 0x00, // Connection ID
//         'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', // Auth plugin data part 1
//         0x00, // Filler
//         0xFF, 0xF7, // Capability flags (lower)
//         0x21, // Character set (utf8_general_ci)
//         0x00, 0x02, // Status flags
//         0xFF, 0xFF, // Capability flags (upper)
//         0x00, // Auth plugin length
//         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Reserved
//         'm', 'y', 's', 'q', 'l', '_', 'n', 'a', 't', 'i', 'v', 'e', '_', 'p', 'a', 's', 's', 'w', 'o', 'r', 'd', 0x00
//     };

//     // Prefix with 3-byte length + 1-byte packet number (0)
//     unsigned char header[] = {
//         static_cast<unsigned char>(sizeof(handshake)), 0x00, 0x00, 0x00
//     };

//     send(client_fd, header, sizeof(header), 0);
//     send(client_fd, handshake, sizeof(handshake), 0);
//     std::cout << "Sent handshake." << std::endl;

//     // --- Step 2: Receive Client Login Packet (ignore contents) ---
//     unsigned char buffer[1024] = { 0 };
//     read(client_fd, buffer, sizeof(buffer));
//     std::cout << "Received login packet." << std::endl;

//     // --- Step 3: Send OK Packet ---
//     unsigned char ok[] = {
//         0x07, 0x00, 0x00, 0x02, // packet header: length=7, seq=2
//         0x00, // OK header
//         0x00, 0x00, // affected rows
//         0x00, 0x00, // last insert ID
//         0x02, 0x00, // server status
//         0x00, 0x00 // warnings
//     };

//     send(client_fd, ok, sizeof(ok), 0);
//     std::cout << "Sent OK packet. Connection should appear successful." << std::endl;

//     close(client_fd);
//     close(server_fd);
//     return 0;
// }