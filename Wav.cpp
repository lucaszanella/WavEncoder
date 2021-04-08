#include "Wav.hpp"
#include <fstream>

template<typename R>
Wav<R>::Wav() : mSampleRate(44100), mNumChannel(2), bitsPerSample(16) {}

template<typename R>
Wav<R>::Wav(int sampleRate, int channelCount, int bitsPerSample) : mSampleRate(sampleRate), mNumChannel(channelCount), bitsPerSample(bitsPerSample) {}

template<typename R>
std::string Wav<R>::extractStr(std::ifstream &ifs, const int size) {
    std::string data(size, ' ');

    ifs.read((char *) &data[0], size);

    return data;
}

template<typename R>
void Wav<R>::encodeStr(std::ofstream &ofs, const std::string &str) {
    for (std::size_t i = 0; i < str.size(); i++) {
        write(ofs, str[i]);
    }
}

template<typename R>
bool Wav<R>::loadFromWav(const std::string filename) {
    std::ifstream ifs(filename, std::ifstream::out | std::ifstream::binary | std::ifstream::ate);

    if (!ifs) {
        std::cerr << "Error: bool Wav::loadFromWav(std::string) failed to open file |" << filename << "|." << std::endl;
        ifs.close();
        return false;
    }

    const std::size_t filesize = ifs.tellg();

    ifs.clear();
    ifs.seekg(0, std::ifstream::beg);

    if (extractStr(ifs, 4) != "RIFF") ///4
    {
        std::cerr << "Error: bool Wav::loadFromWav(std::string) RIFF not present in the header for file |" << filename << "|." << std::endl;
        ifs.close();
        return false;
    }

    const std::uint32_t chunkSize = read<std::uint32_t>(ifs); ///8

    if (extractStr(ifs, 4) != "WAVE") ///12
    {
        std::cerr << "Error: bool Wav::loadFromWav(std::string) WAVE not present in the header for file |" << filename << "|." << std::endl;
        ifs.close();
        return false;
    }

    const std::string fmt = extractStr(ifs, 4); ///16

    const std::uint32_t subchunck1size = read<std::uint32_t>(ifs); ///20
    const std::uint16_t audioFormat = read<std::uint16_t>(ifs); ///22

    mNumChannel = read<std::uint16_t>(ifs); ///24
    mSampleRate = read<std::uint32_t>(ifs); ///28

    const std::uint32_t byteRate = read<std::uint32_t>(ifs); ///32
    const std::uint16_t blockAlign = read<std::uint16_t>(ifs); ///34
    const std::uint16_t bitsPerSample = read<std::uint16_t>(ifs); ///36

    std::string chunkName = extractStr(ifs, 4); ///40

    while (chunkName != "data") {
        const std::uint32_t subChunck2Size = read<std::uint32_t>(ifs);
        extractStr(ifs, subChunck2Size);
        chunkName = extractStr(ifs, 4);
    }

    const std::uint32_t subChunck2Size = read<std::uint32_t>(ifs); ///44

    const int remaining = filesize - 44; ///NumSamples * NumChannels * BitsPerSample/8

    mData.clear();
    for (int i = 0; i < remaining / 2; i++) {
        mData.push_back(read<std::int16_t>(ifs));
    }

    ifs.close();

    return true;
}

template<typename R>
void Wav<R>::loadFromVector(const std::vector<R> &data, const int sampleRate, const int numChannel) {
    mData = data;
    mSampleRate = sampleRate;
    mNumChannel = numChannel;
}

template<typename T>
bool Wav<T>::saveToWav(std::string& filename) const {
    std::ofstream ofs(filename, std::ofstream::out | std::ofstream::binary);

    if (!ofs) {
        std::cerr << "Error: bool Wav::saveToWav(std::string) failed to create file |" << filename << "|." << std::endl;
        ofs.close();
        return false;
    }

    //RIFF chunk descriptor
    encodeStr(ofs, "RIFF");

    //std::cout << 44+mData.size()*2-1*8 << std::endl;
    write<std::int32_t>(ofs, 44 + mData.size() * sizeof(T) - 1 * 8); ///byte number in the file

    encodeStr(ofs, "WAVE");

    /// fmt  sub chunk ///
    encodeStr(ofs, "fmt ");
    /// 16 for PCM
    write<std::int32_t>(ofs, 16);
    /// Audio format (1 for PCM)
    write<std::int16_t>(ofs, int16_t(1));
    write<std::int16_t>(ofs, int16_t(mNumChannel));
    write<std::int32_t>(ofs, int32_t(mSampleRate));
    //ByteRate (SampleRate * NumChannels * BitsPerSample/8)
    write<std::int32_t>(ofs, int32_t(mSampleRate * mNumChannel * (bitsPerSample / 8)));
    /// Block align (NumChannels * BitsPerSample/8)
    write<std::int16_t>(ofs, int16_t(mNumChannel * bitsPerSample / 8)); //block align
    write<std::int16_t>(ofs, int16_t(bitsPerSample)); //bitsPerSample

    /// DATA sub chunk ///
    encodeStr(ofs, "data");
    //Subchunk2Size == NumSamples * NumChannels * BitsPerSample/8 (or simply: data size in bytes)
    write<std::int32_t>(ofs, int32_t(mData.size() * sizeof(T)));

    for (std::size_t i = 0; i < mData.size(); i++) {
        write<T>(ofs, mData[i]);
    }

    ofs.close();

    return true;
}

template<typename T>
int Wav<T>::getSampleRate() const {
    return mSampleRate;
}

template<typename T>
int Wav<T>::getNumChannel() const {
    return mNumChannel;
}

template<typename T>
const std::vector<T> &Wav<T>::getData() const {
    return mData;
}
