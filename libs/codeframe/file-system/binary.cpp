#include "binary.h"

// bool isBinaryFile(const string& filePath)
// {
//     std::size_t dotPos = filePath.rfind('.');
//     if (dotPos != string::npos) {
//         string extension = filePath.substr(dotPos);
//         return binaryExtensions.find(extension) != binaryExtensions.end();
//     }

//     return false;
// }

bool isBinary(const string& extension)
{
    return binaryExtensions.find(extension) != binaryExtensions.end();
}

vector<char> readBinaryFile(const string& filePath)
{
    // Open the file for binary reading
    std::ifstream file(filePath, std::ios::binary);

    // Check if file is open
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    // Seek to the end to find the file size
    file.seekg(0, std::ios::end);
    std::size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file into a vector
    vector<char> buffer(fileSize);
    if (fileSize > 0) {
        file.read(buffer.data(), fileSize);
    }

    return buffer;
}

bool writeBinaryFile(const fs::path& filePath, const string& data)
{
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile) {
        console.error("Unable to open file for binary writing: " + filePath.generic_string());
        return false;
    }
    outFile.write(data.data(), data.size());
    outFile.flush();
    outFile.close();
    return outFile.good();
}

string binaryToBase64(const vector<char>& binaryData)
{
    string base64;
    int i = 0, j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    for (char c : binaryData) {
        char_array_3[i++] = c;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                base64 += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            base64 += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            base64 += '=';
    }

    return base64;
}