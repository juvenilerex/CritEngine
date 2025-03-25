#include "FileAccessor.h"
#include "../Logging/Logger.h"

namespace Engine {

    FileAccessor::FileAccessor() 
        : bigEndianEnabled(IsBigEndian())
    { 
    }

    void FileAccessor::OpenFile(const std::filesystem::path& filePath)
    {
        this->file = std::ifstream(filePath, std::ios::binary);

        if (this->file.is_open()) {

            this->file.seekg(0, std::ios::end);
            std::streamsize size = this->file.tellg();
            this->file.seekg(0, std::ios::beg);
            this->buffer.resize(size); // Pre-allocates memory for the entire file

            if (this->file.read(this->buffer.data(), size)) {
                this->position = this->buffer.data();
                // Successful read
            }
            this->file.close();
        }
    }

    // If the value of the first byte is 1, we are in little endian since the least significant byte is stored first
    bool FileAccessor::IsBigEndian() const {
        int val = 1;
        return (!*(char*)&val == 1);
    }

    void FileAccessor::Seek(const uint64_t offset) {
        ASSERT(CheckOffsetBounds(offset), "Position is out of bounds")

        this->position = this->buffer.data() + offset;
    }

    void FileAccessor::SeekEnd()
    {
        this->position = buffer.data() + buffer.size();
    }

    const char* FileAccessor::GetPosition()
    {
        return this->position;
    }

    size_t FileAccessor::GetSize()
    {
        return this->buffer.size();
    }

    void FileAccessor::ReserveBuffer(const uint64_t length)
    {
        this->buffer.reserve(length);
    }

    void FileAccessor::FreeBuffer()
    {
        this->buffer.clear();
        this->position = nullptr;
    }

    bool FileAccessor::IsAtEof() const
    {
        return position >= buffer.data() + buffer.size();
    }

    void FileAccessor::SetBigEndian(const bool val) {
        this->bigEndianEnabled = val;
    }

    void FileAccessor::SetFileEndianness(const bool isBigEndian) {
        this->fileIsBigEndian = isBigEndian;
    }

    // This section reads binary data from a char* pointer and stores it for us in a specific interger type
    // After the read, we use pointer arithmetic to ensure the next read starts at the correct position
 
    // For a single byte type, we're simply using a static_cast to avoid unnecessary overhead from memcpy
    uint8_t FileAccessor::ReadUINT8() {
        ASSERT(CheckBounds(sizeof(uint8_t)), "Can not read out of buffer's bounds");

        uint8_t value = static_cast<uint8_t>(*this->position);
        this->position += sizeof(uint8_t);
        return value;
    }

    uint16_t FileAccessor::ReadUINT16() {
        ASSERT(CheckBounds(sizeof(uint16_t)), "Can not read out of buffer's bounds");

        uint16_t value;
        std::memcpy(&value, this->position, sizeof(uint16_t));
        this->position += sizeof(uint16_t);

        if (fileIsBigEndian != bigEndianEnabled)
            value = BSWAP_16(value);
        return value;
    }

    uint32_t FileAccessor::ReadUINT32() {
        ASSERT(CheckBounds(sizeof(uint32_t)), "Can not read out of buffer's bounds");

        uint32_t value;
        std::memcpy(&value, this->position, sizeof(uint32_t));
        this->position += sizeof(uint32_t);

        if (this->fileIsBigEndian != this->bigEndianEnabled)
            value = BSWAP_32(value);      
        return value;
    }

    uint64_t FileAccessor::ReadUINT64() {
        ASSERT(CheckBounds(sizeof(uint64_t)), "Can not read out of buffer's bounds");

        uint64_t value;
        std::memcpy(&value, this->position, sizeof(uint64_t));
        this->position += sizeof(uint64_t);

        if (this->fileIsBigEndian != this->bigEndianEnabled)
            value = BSWAP_64(value);      
        return value;
    }

    int8_t FileAccessor::ReadINT8() {
        ASSERT(CheckBounds(sizeof(int8_t)), "Can not read out of buffer's bounds");

        int8_t value = static_cast<int8_t>(*this->position);
        this->position += sizeof(int8_t);
        return value;
    }

    int16_t FileAccessor::ReadINT16() {
        ASSERT(CheckBounds(sizeof(int16_t)), "Can not read out of buffer's bounds");

        int16_t value;
        std::memcpy(&value, this->position, sizeof(int16_t));
        this->position += sizeof(int16_t);

        if (this->fileIsBigEndian != this->bigEndianEnabled)
            value = BSWAP_16(value);      
        return value;
    }

    int32_t FileAccessor::ReadINT32() {
        ASSERT(CheckBounds(sizeof(int32_t)), "Can not read out of buffer's bounds");

        int32_t value;
        std::memcpy(&value, this->position, sizeof(int32_t));
        this->position += sizeof(int32_t);

        if (this->fileIsBigEndian != this->bigEndianEnabled)
            value = BSWAP_32(value);     
        return value;
    }

    int64_t FileAccessor::ReadINT64() {
        ASSERT(CheckBounds(sizeof(int64_t)), "Can not read out of buffer's bounds");

        int64_t value;
        std::memcpy(&value, this->position, sizeof(int64_t));
        this->position += sizeof(int64_t);

        if (this->fileIsBigEndian != this->bigEndianEnabled)
            value = BSWAP_64(value);   
        return value;
    }

    // Updated to check for upper and lower bounds, then checking if the bytes are in range
    // This is a bit more robust
    bool FileAccessor::CheckBounds(size_t bytes) const {
        if (this->position < this->buffer.data() || this->position >= GetBufferEnd())
            return false;
        if (bytes > static_cast<size_t>(GetBufferEnd() - this->position))
            return false;
        return true;
    }

    bool FileAccessor::CheckOffsetBounds(uint64_t offset) const
    {
        return offset <= this->buffer.size();
    }

    const char* FileAccessor::GetBufferEnd() const
    {
        return this->buffer.data() + this->buffer.size();
    }

} 