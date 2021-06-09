#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

// Riff Chunk
const string chunk_id = "RIFF";
const string chunk_size = "----";
const string format = "WAVE";

// FMT Sub Chunk
const string subchunk1_id = "fmt ";
const int subchunk1_size = 16;
const int audio_format = 1;
const int num_channels = 2;
const int sample_rate = 44100;
const int byte_rate = sample_rate * num_channels * (subchunk1_size / 8);
const int block_align = num_channels * (subchunk1_size / 8);
const int bits_per_sample = 16;

// Data Sub Chunk
const string subchunk2_id = "data";
const string subchunk2_size = "----";

const int duration = 2;
const int max_amp = 32767;
const double freq_value = 250;

void write_as_bytes(ofstream&, int, int);

int main()
{
	ofstream wav;
	wav.open("test.wav", ios::binary);

	if(wav.is_open()) {
		wav << chunk_id;
		wav << chunk_size;
		wav << format;

		wav << subchunk1_id;
		write_as_bytes(wav, subchunk1_size, 4);
		write_as_bytes(wav, audio_format, 2);
		write_as_bytes(wav, num_channels, 2);
		write_as_bytes(wav, sample_rate, 4);
		write_as_bytes(wav, byte_rate, 4);
		write_as_bytes(wav, block_align, 2);
		write_as_bytes(wav, bits_per_sample, 2);
		
		wav << subchunk2_id;
		wav << subchunk2_size;

		int start_audio = wav.tellp();

		for (int i = 0; i < sample_rate * duration; i++) {
			// Respect max amplitude
			// Operate as a wave
			double amplitude = (double)i / sample_rate * max_amp;
			double value = sin((2 * 3.14 * i * freq_value) / sample_rate);

			double channel1 = amplitude * value / 2;
			double channel2 = (max_amp - amplitude)* value;

			write_as_bytes(wav, channel1, 2);
			write_as_bytes(wav, channel2, 2);

		}

		int end_audio = wav.tellp();
		wav.seekp(start_audio - 4);
		write_as_bytes(wav, end_audio - start_audio, 4);

		wav.seekp(4, ios::beg);
		write_as_bytes(wav, end_audio - 8, 4);
	}

	wav.close();

	return 0;
};

void write_as_bytes(ofstream &file, int value,  int byte_size)
{
	file.write(reinterpret_cast<const char*>(&value), byte_size);
}
