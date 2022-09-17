// Copyright (c) 2022 Perets Dmytro
// Author: Perets Dmytro <imperator999mcpe@gmail.com>
//
// Personal usage is allowed only if this comment was not changed or deleted.
// Commercial usage must be agreed with the author of this comment.


#include "percarch.h"
#include "constants.hpp"
#include "perceptron.hpp"
#include <cstdarg>
#include <sys/stat.h>


inline static void error(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
}

#define error(args) if (fprintf(stderr, "[percarch] :: " __FILE_NAME__ ":%d:" MACRO_STR(__func__) " -> ", __LINE__)) error args


inline static constexpr neural::neuron_t sigm(neural::neuron_t x)
{
	return 1 / (1 + std::exp(-0.3 * x));
}

inline static constexpr neural::neuron_t sigm_custom_activation(neural::neuron_t x)
{
	return 10 * sigm(x) - 1;
}

inline static constexpr neural::neuron_t sigm_custom_deactivation(neural::neuron_t x)
{
	return 3 * sigm(x) * (1 - sigm(x));
}


inline static void save_vector(FILE* file, const std::vector<neural::neuron_t>& vec);

inline static void save_vector(FILE* file, const std::vector<std::vector<neural::neuron_t>>& vec);

inline static void save_network(FILE* file, const std::vector<neural::core_data>& data);

inline static void load_vector(FILE* file, std::vector<neural::neuron_t>& vec);

inline static void load_vector(FILE* file, std::vector<std::vector<neural::neuron_t>>& vec);

inline static void load_network(FILE* file, std::vector<neural::core_data>& data);


inline static std::vector<neural::neuron_t> make_binary_vector(size_t num);

inline static std::vector<neural::neuron_t> make_char_vector(FILE* file, size_t pos);

inline static bool is_equal(const std::vector<neural::neuron_t>& vec1, const std::vector<neural::neuron_t>& vec2);


inline static void print_vecs(const std::vector<neural::neuron_t>& results, const std::vector<neural::neuron_t>& samples)
{
	printf(" > res = ( ");
	for (auto& r : results)
		printf("%Lf ", r);
	printf(")\n > sam = ( ");
	for (auto& s : samples)
		printf("%Lf ", s);
	printf(")\n");
}


void percarch_compress(const std::string& input, const std::string& output, long power)
{
	FILE* ifile = fopen(input.c_str(), "rb");
	if (!ifile)
	{
		error(("Can't open file \"%s\" for reading.", input.c_str()));
		return;
	}
	
	FILE* ofile = fopen(output.c_str(), "wb");
	if (!ofile)
	{
		error(("Can't open file \"%s\" for writing.", output.c_str()));
		return;
	}
	
	struct stat st{ };
	fstat(ifile->_fileno, &st);
	
	if (!power)
		power = st.st_size;
	
	std::vector<size_t> sizes;
	
	sizes.push_back(INPUTS_COUNT);
	
	while (power > 0)
	{
		long cnt = power % 0x10;
		power >>= 3;
		sizes.push_back((cnt + 1) << 3);
	}
	
	sizes.push_back(OUTPUTS_COUNT);
	
	neural::perceptron perc(sizes, -5, 1, sigm_custom_activation, sigm_custom_deactivation);
	
	bool passes = false;
	uint8_t cnt = 0;
	while (!passes)
	{
		++cnt;
		passes = true;
		for (size_t i = 0; i < st.st_size; i += OUTPUTS_COUNT)
		{
			auto samples = make_char_vector(ifile, i);
			auto results = perc.use(make_binary_vector(i));
			//			print_vecs(results, samples);
			bool tmp = is_equal(results, samples);
			passes = passes && tmp;
			if (!tmp)
				perc.teach(samples, LEARNING_RATE);
		}
		//		printf("\n");
	}
	
	save_network(ofile, perc.get_core_data());
}


inline static void save_vector(FILE* file, const std::vector<neural::neuron_t>& vec)
{
	size_t size = vec.size();
	fwrite(&size, sizeof size, 1, file);
	for (auto& v : vec)
		fwrite(&v, sizeof v, 1, file);
}

inline static void save_vector(FILE* file, const std::vector<std::vector<neural::neuron_t>>& vec)
{
	size_t size = vec.size();
	fwrite(&size, sizeof size, 1, file);
	for (auto& v : vec)
		save_vector(file, v);
}

inline static void save_network(FILE* file, const std::vector<neural::core_data>& data)
{
	size_t size;
	fwrite(&size, sizeof size, 1, file);
	for (auto& d : data)
	{
		save_vector(file, d.biases);
		save_vector(file, d.weights);
	}
}

inline static void load_vector(FILE* file, std::vector<neural::neuron_t>& vec)
{
	size_t size = 0;
	fread(&size, sizeof size, 1, file);
	vec.resize(size);
	
	neural::neuron_t x;
	for (size_t i = 0; i < size; ++i)
	{
		fread(&x, sizeof x, 1, file);
		vec[i] = x;
	}
}

inline static void load_vector(FILE* file, std::vector<std::vector<neural::neuron_t>>& vec)
{
	size_t size = 0;
	fread(&size, sizeof size, 1, file);
	vec.resize(size);
	
	std::vector<neural::neuron_t> x;
	for (size_t i = 0; i < size; ++i)
	{
		load_vector(file, x);
		vec[i] = std::move(x);
	}
}

inline static void load_network(FILE* file, std::vector<neural::core_data>& data)
{
	size_t size = 0;
	fread(&size, sizeof size, 1, file);
	data.resize(size);
	for (auto& d : data)
	{
		load_vector(file, d.biases);
		load_vector(file, d.weights);
	}
}


inline static std::vector<neural::neuron_t> make_binary_vector(size_t num)
{
	decltype(make_binary_vector(num)) result;
	while (num)
	{
		result.push_back(num & 1);
		num >>= 1;
	}
	result.resize(INPUTS_COUNT);
	return std::move(result);
}

inline static int to_char(neural::neuron_t num)
{
	if (num < 0) return -1;
	return static_cast<unsigned char>(std::round(num * UINT8_MAX / 10));
}

inline static neural::neuron_t from_char(unsigned char c)
{
	return static_cast<neural::neuron_t>(c) * 10 / UINT8_MAX;
}

inline static std::vector<neural::neuron_t> make_char_vector(FILE* file, size_t pos)
{
	fseek(file, static_cast<long>(pos), SEEK_SET);
	
	decltype(make_char_vector(file, pos)) result;
	result.resize(OUTPUTS_COUNT);
	
	unsigned char x;
	for (size_t i = 0; i < OUTPUTS_COUNT; ++i)
	{
		fread(&x, sizeof x, 1, file);
		if (feof(file))
		{
			result[i] = -0.5;
			return std::move(result);
		}
		result[i] = from_char(x);
	}
	
	return std::move(result);
}

inline static bool is_equal(const std::vector<neural::neuron_t>& vec1, const std::vector<neural::neuron_t>& vec2)
{
	if (vec1.size() != vec2.size())
	{
		error(("Invalid comparison. vec1.size() != vec2.size(): %zu != %zu.", vec1.size(), vec2.size()));
		return false;
	}
	
	for (size_t i = 0; i < vec1.size(); ++i)
	{
		if (to_char(vec1[i]) != to_char(vec2[i]))
			return false;
	}
	return true;
}