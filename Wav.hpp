#ifndef SIMPLE_WAV_ENCODER_H
#define SIMPLE_WAV_ENCODER_H

#include <vector>
#include <iostream>
#include <fstream>


template<class R = std::int16_t>
class Wav
{
public:
    Wav();
    Wav(int sampleRate, int channelCount, int bitsPerSample);
    ~Wav() {}

    bool loadFromWav(std::string filename);
    void loadFromVector(const std::vector<R>& data, int sampleRate, int numChannel);

    bool saveToWav(std::string& filename) const;

    int getSampleRate(void) const;
    int getNumChannel(void) const;

    const std::vector<R>& getData(void) const;
private:

    int mSampleRate;
    int mNumChannel;
    int bitsPerSample;

    std::vector<R> mData;


    template <class T>
    static void write(std::ofstream& ofs,const T& data)
    {
        ofs.write( (char*)&data , sizeof(T));
    }

    template <class T>
    static T read(std::ifstream& ifs)
    {
        char* data = new char[sizeof(T)];

        ifs.read(data,sizeof(T));

        const T result = *(T*)data;

        delete data;

        return result;
    }

    static std::string extractStr(std::ifstream& ifs, const int size);
    static void encodeStr(std::ofstream& ofs, const std::string& str);
};

#endif // SIMPLE_WAV_ENCODER_H
