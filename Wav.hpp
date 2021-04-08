#ifndef SIMPLE_WAV_ENCODER_H
#define SIMPLE_WAV_ENCODER_H

#include <vector>
#include <iostream>
#include <fstream>


template<class T = std::int16_t>
class Wav
{
public:
    Wav();
    Wav(int sampleRate, int channelCount, int bitsPerSample);
    ~Wav() {}

    bool loadFromWav(std::string filename);
    void loadFromVector(const std::vector<T>& data, int sampleRate, int numChannel);

    bool saveToWav(std::string& filename) const;
    static bool saveToWav(std::string& filename, int sampleRate, int channelCount, std::vector<T> data);
    static bool writeHeader(std::string& filename, int sampleRate, int channelCount, size_t size);
    int getSampleRate(void) const;
    int getNumChannel(void) const;

    const std::vector<T>& getData(void) const;
private:

    int mSampleRate;
    int mNumChannel;
    int bitsPerSample;

    std::vector<T> mData;


    template <class R>
    static void write(std::ofstream& ofs,const R& data)
    {
        ofs.write( (char*)&data , sizeof(R));
    }

    template <class R>
    static T read(std::ifstream& ifs)
    {
        char* data = new char[sizeof(R)];

        ifs.read(data,sizeof(R));

        const R result = *(R*)data;

        delete data;

        return result;
    }

    static std::string extractStr(std::ifstream& ifs, const int size);
    static void encodeStr(std::ofstream& ofs, const std::string& str);
};

#endif // SIMPLE_WAV_ENCODER_H
