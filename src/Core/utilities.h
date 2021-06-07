﻿#pragma once
#include "colorwin.hpp"
#define PPK_ASSERT_ENABLED 1
#define PPK_ASSERT_DISABLE_STL
#define PPK_ASSERT_DEFAULT_LEVEL Error
#include "ppk_assert.h"
#include <cstdint>
#include <iostream>
#include <glad/glad.h>
#include <gtc/matrix_transform.hpp>
#include <gtx/compatibility.hpp>
#include <gtc/type_ptr.hpp>
#include "gtc/random.hpp"
#include <vector>
#define LOGGING_ENABLED


#define ASSERT                PPK_ASSERT
#define ASSERT_WARNING        PPK_ASSERT_WARNING
#define ASSERT_DEBUG          PPK_ASSERT_DEBUG
#define ASSERT_ERROR          PPK_ASSERT_ERROR
#define ASSERT_FATAL          PPK_ASSERT_FATAL
#define LOG(message, ...)     PPK_ASSERT_CUSTOM(0, 0, message, __VA_ARGS__)
#define WARN(message, ...)     PPK_ASSERT_CUSTOM(1, 0, message, __VA_ARGS__)
#define OPENGL_LOG(message, ...)     PPK_ASSERT_CUSTOM(2, 0, message, __VA_ARGS__)
#define OPENGL_THROW(message, ...)   PPK_ASSERT_CUSTOM(3, 0, message, __VA_ARGS__)
#define GLFW_LOG(message, ...)     PPK_ASSERT_CUSTOM(4, 0, message, __VA_ARGS__)
#define GLFW_THROW(message, ...)   PPK_ASSERT_CUSTOM(5, 0, message, __VA_ARGS__)
//TODO: Add LOG_ONCE and WARN_ONCE macros!
// std::unordered_set<size_t> hashujesz i dodajesz tutaj jak wyjdzie ze juz tam bylo to nie wyswietlasz loga a jak udalo sie dodac to wyswietlasz

static ppk::assert::implementation::AssertAction::AssertAction assertHandler(const char* file, int line, const char* function,
                                                                              const char* expression, int level, const char* message)
{
    //TODO: DAAAAAAWID zrób to ładne i zrób do tego timestamp
    using namespace ppk::assert::implementation;
    using namespace colorwin;
    switch(level)
    {
    // (13:12:45.245) [OpenGL] messege
    //W funkcji log nie potrzebujemy expression ani fuction
    case 0/*LOG */: std::cout<<file<<" : "<<line<<"\n"<<message<<"\n\n"; return AssertAction::None;
    //Tutaj powinniśmy wypisać wszystko co mamy
    case 1/*WARN*/: std::cout<<color(red)<<file<<" : "<<line<<"\n"<<message<<"\n\n"; return AssertAction::None;

    case 2/*OPENGL_LOG  */: std::cout<<color(cyan)<<"[OpenGL] "<<message<<"\n\n"; return AssertAction::None;
    case 3/*OPENGL_THROW*/: std::cout<<color(red)<<"[OpenGL] "<<message<<"\n\n"; return AssertAction::Throw;
    case 4/*GLFW_LOG    */: std::cout<<color(cyan)<<"[GLFW] "<<message<<"\n\n"; return AssertAction::None;
    case 5/*GLFW_THROW  */: std::cout<<color(red)<<"[GLFW] "<<message<<"\n\n"; return AssertAction::Throw;

    case AssertLevel::Warning: std::cout<<file<<line<<message; return AssertAction::None;
    case AssertLevel::Debug:
    case AssertLevel::Error:
    case AssertLevel::Fatal:
        std::cout<<file<<line<<message;
        return AssertAction::Throw;
    }
    std::cout<<file<<" "<<line<<"Invalid Assert level:"<<level<<"\n\n"<<message;
    return AssertAction::Throw;
}

#define UNUSED(x) (void)(x)


// becouse i hate std random generators
// this code is borrowed from OneLoneCoder
// https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_ProcGen_Universe.cpp
// who in turn borrowed rnd function from https://lemire.me/blog/2019/03/19/the-fastest-conventional-random-number-generator-that-can-pass-big-crush/

static uint32_t ___seed = 1;

inline uint32_t rnd()
{
    ___seed += 0xe120fc15;
    uint64_t tmp;
    tmp = (uint64_t)___seed * 0x4a39b70d;
    uint32_t m1 = (uint32_t)((tmp >> 32) ^ tmp);
    tmp = (uint64_t)m1 * 0x12fad5c9;
    uint32_t m2 = (uint32_t)((tmp >> 32) ^ tmp);
    return m2;
}

inline double rndDouble(double min, double max)
{
    return ((double)rnd() / (double)(0x7FFFFFFF)) * (max - min) + min;
}

inline int rndInt(int min, int max)
{
    if(min-max == 0) return min;
    return (rnd() % (max - min)) + min;
}

inline void rndSeed(uint32_t s)
{
    ___seed = s;
}

template<typename T>
inline T mapToRange(glm::vec<2,T> startRange, glm::vec<2,T> finalRange, T value)
{
    return (value-startRange.x)/(startRange.y-startRange.x)*(finalRange.y-finalRange.x)+finalRange.x;
}

template<typename T>
inline void arrayShuffle(T* array, size_t count)
{
    for(size_t i=0; i<count-1; i++)
    {
        T temp = array[i];
        size_t rand = glm::linearRand(i, count-1);
        array[i] = array[rand];
        array[rand] = temp;
    }
}

inline GLenum textureSizedFormatToFormat(GLenum internalFormat)
{
    switch(internalFormat)
    {
    case GL_R8:             return GL_RED;
    case GL_R8_SNORM:       return GL_RED;
    case GL_R16:            return GL_RED;
    case GL_R16_SNORM:      return GL_RED;
    case GL_RG8:            return GL_RG;
    case GL_RG8_SNORM:      return GL_RG;
    case GL_RG16:           return GL_RG;
    case GL_RG16_SNORM:     return GL_RG;
    case GL_R3_G3_B2:       return GL_RGB;
    case GL_RGB4:           return GL_RGB;
    case GL_RGB5:           return GL_RGB;
    case GL_RGB8:           return GL_RGB;
    case GL_RGB8_SNORM:     return GL_RGB;
    case GL_RGB10:          return GL_RGB;
    case GL_RGB12:          return GL_RGB;
    case GL_RGB16_SNORM:    return GL_RGB;
    case GL_RGBA2:          return GL_RGB;
    case GL_RGBA4:          return GL_RGB;
    case GL_RGB5_A1:        return GL_RGBA;
    case GL_RGBA8:          return GL_RGBA;
    case GL_RGBA8_SNORM:    return GL_RGBA;
    case GL_RGB10_A2:       return GL_RGBA;
    case GL_RGB10_A2UI:     return GL_RGBA;
    case GL_RGBA12:         return GL_RGBA;
    case GL_RGBA16:         return GL_RGBA;
    case GL_SRGB8:          return GL_RGB;
    case GL_SRGB8_ALPHA8:   return GL_RGBA;
    case GL_R16F:           return GL_RED;
    case GL_RG16F:          return GL_RG;
    case GL_RGB16F:         return GL_RGB;
    case GL_RGBA16F:        return GL_RGBA;
    case GL_R32F:           return GL_RED;
    case GL_RG32F:          return GL_RG;
    case GL_RGB32F:         return GL_RGB;
    case GL_RGBA32F:        return GL_RGBA;
    case GL_R11F_G11F_B10F: return GL_RGB;
    case GL_RGB9_E5:        return GL_RGB;
    case GL_R8I:            return GL_RED;
    case GL_R8UI:           return GL_RED;
    case GL_R16I:           return GL_RED;
    case GL_R16UI:          return GL_RED;
    case GL_R32I:           return GL_RED;
    case GL_R32UI:          return GL_RED;
    case GL_RG8I:           return GL_RG;
    case GL_RG8UI:          return GL_RG;
    case GL_RG16I:          return GL_RG;
    case GL_RG16UI:         return GL_RG;
    case GL_RG32I:          return GL_RG;
    case GL_RG32UI:         return GL_RG;
    case GL_RGB8I:          return GL_RGB;
    case GL_RGB8UI:         return GL_RGB;
    case GL_RGB16I:         return GL_RGB;
    case GL_RGB16UI:        return GL_RGB;
    case GL_RGB32I:         return GL_RGB;
    case GL_RGB32UI:        return GL_RGB;
    case GL_RGBA8I:         return GL_RGBA;
    case GL_RGBA8UI:        return GL_RGBA;
    case GL_RGBA16I:        return GL_RGBA;
    case GL_RGBA16UI:       return GL_RGBA;
    case GL_RGBA32I:        return GL_RGBA;
    case GL_RGBA32UI:       return GL_RGBA;
    default: ASSERT(0);
        return 0;
    }
}

const std::vector<glm::vec4> prettyColors =
{
    {0.050383, 0.029803, 0.527975, 1.0},
    {0.063536, 0.028426, 0.533124, 1.0},
    {0.075353, 0.027206, 0.538007, 1.0},
    {0.086222, 0.026125, 0.542658, 1.0},
    {0.096379, 0.025165, 0.547103, 1.0},
    {0.10598, 0.024309, 0.551368, 1.0},
    {0.115124, 0.023556, 0.555468, 1.0},
    {0.123903, 0.022878, 0.559423, 1.0},
    {0.132381, 0.022258, 0.56325, 1.0},
    {0.140603, 0.021687, 0.566959, 1.0},
    {0.148607, 0.021154, 0.570562, 1.0},
    {0.156421, 0.020651, 0.574065, 1.0},
    {0.16407, 0.020171, 0.577478, 1.0},
    {0.171574, 0.019706, 0.580806, 1.0},
    {0.17895, 0.019252, 0.584054, 1.0},
    {0.186213, 0.018803, 0.587228, 1.0},
    {0.193374, 0.018354, 0.59033, 1.0},
    {0.200445, 0.017902, 0.593364, 1.0},
    {0.207435, 0.017442, 0.596333, 1.0},
    {0.21435, 0.016973, 0.599239, 1.0},
    {0.221197, 0.016497, 0.602083, 1.0},
    {0.227983, 0.016007, 0.604867, 1.0},
    {0.234715, 0.015502, 0.607592, 1.0},
    {0.241396, 0.014979, 0.610259, 1.0},
    {0.248032, 0.014439, 0.612868, 1.0},
    {0.254627, 0.013882, 0.615419, 1.0},
    {0.261183, 0.013308, 0.617911, 1.0},
    {0.267703, 0.012716, 0.620346, 1.0},
    {0.274191, 0.012109, 0.622722, 1.0},
    {0.280648, 0.011488, 0.625038, 1.0},
    {0.287076, 0.010855, 0.627295, 1.0},
    {0.293478, 0.010213, 0.62949, 1.0},
    {0.299855, 0.009561, 0.631624, 1.0},
    {0.30621, 0.008902, 0.633694, 1.0},
    {0.312543, 0.008239, 0.6357, 1.0},
    {0.318856, 0.007576, 0.63764, 1.0},
    {0.32515, 0.006915, 0.639512, 1.0},
    {0.331426, 0.006261, 0.641316, 1.0},
    {0.337683, 0.005618, 0.643049, 1.0},
    {0.343925, 0.004991, 0.64471, 1.0},
    {0.35015, 0.004382, 0.646298, 1.0},
    {0.356359, 0.003798, 0.64781, 1.0},
    {0.368733, 0.002724, 0.650601, 1.0},
    {0.374897, 0.002245, 0.651876, 1.0},
    {0.381047, 0.001814, 0.653068, 1.0},
    {0.387183, 0.001434, 0.654177, 1.0},
    {0.393304, 0.001114, 0.655199, 1.0},
    {0.399411, 0.000859, 0.656133, 1.0},
    {0.405503, 0.000678, 0.656977, 1.0},
    {0.41158, 0.000577, 0.65773, 1.0},
    {0.417642, 0.000564, 0.65839, 1.0},
    {0.423689, 0.000646, 0.658956, 1.0},
    {0.429719, 0.000831, 0.659425, 1.0},
    {0.435734, 0.001127, 0.659797, 1.0},
    {0.441732, 0.00154, 0.660069, 1.0},
    {0.447714, 0.00208, 0.66024, 1.0},
    {0.453677, 0.002755, 0.66031, 1.0},
    {0.459623, 0.003574, 0.660277, 1.0},
    {0.46555, 0.004545, 0.660139, 1.0},
    {0.471457, 0.005678, 0.659897, 1.0},
    {0.477344, 0.00698, 0.659549, 1.0},
    {0.48321, 0.00846, 0.659095, 1.0},
    {0.489055, 0.010127, 0.658534, 1.0},
    {0.494877, 0.01199, 0.657865, 1.0},
    {0.500678, 0.014055, 0.657088, 1.0},
    {0.506454, 0.016333, 0.656202, 1.0},
    {0.512206, 0.018833, 0.655209, 1.0},
    {0.517933, 0.021563, 0.654109, 1.0},
    {0.523633, 0.024532, 0.652901, 1.0},
    {0.529306, 0.027747, 0.651586, 1.0},
    {0.534952, 0.031217, 0.650165, 1.0},
    {0.54057, 0.03495, 0.64864, 1.0},
    {0.546157, 0.038954, 0.64701, 1.0},
    {0.551715, 0.043136, 0.645277, 1.0},
    {0.557243, 0.047331, 0.643443, 1.0},
    {0.562738, 0.051545, 0.641509, 1.0},
    {0.568201, 0.055778, 0.639477, 1.0},
    {0.573632, 0.060028, 0.637349, 1.0},
    {0.579029, 0.064296, 0.635126, 1.0},
    {0.584391, 0.068579, 0.632812, 1.0},
    {0.589719, 0.072878, 0.630408, 1.0},
    {0.595011, 0.07719, 0.627917, 1.0},
    {0.600266, 0.081516, 0.625342, 1.0},
    {0.605485, 0.085854, 0.622686, 1.0},
    {0.615812, 0.094564, 0.61714, 1.0},
    {0.620919, 0.098934, 0.614257, 1.0},
    {0.625987, 0.103312, 0.611305, 1.0},
    {0.631017, 0.107699, 0.608287, 1.0},
    {0.636008, 0.112092, 0.605205, 1.0},
    {0.640959, 0.116492, 0.602065, 1.0},
    {0.645872, 0.120898, 0.598867, 1.0},
    {0.650746, 0.125309, 0.595617, 1.0},
    {0.65558, 0.129725, 0.592317, 1.0},
    {0.660374, 0.134144, 0.588971, 1.0},
    {0.665129, 0.138566, 0.585582, 1.0},
    {0.669845, 0.142992, 0.582154, 1.0},
    {0.674522, 0.147419, 0.578688, 1.0},
    {0.67916, 0.151848, 0.575189, 1.0},
    {0.683758, 0.156278, 0.57166, 1.0},
    {0.688318, 0.160709, 0.568103, 1.0},
    {0.69284, 0.165141, 0.564522, 1.0},
    {0.697324, 0.169573, 0.560919, 1.0},
    {0.701769, 0.174005, 0.557296, 1.0},
    {0.706178, 0.178437, 0.553657, 1.0},
    {0.710549, 0.182868, 0.550004, 1.0},
    {0.714883, 0.187299, 0.546338, 1.0},
    {0.719181, 0.191729, 0.542663, 1.0},
    {0.723444, 0.196158, 0.538981, 1.0},
    {0.72767, 0.200586, 0.535293, 1.0},
    {0.731862, 0.205013, 0.531601, 1.0},
    {0.736019, 0.209439, 0.527908, 1.0},
    {0.740143, 0.213864, 0.524216, 1.0},
    {0.744232, 0.218288, 0.520524, 1.0},
    {0.748289, 0.222711, 0.516834, 1.0},
    {0.752312, 0.227133, 0.513149, 1.0},
    {0.756304, 0.231555, 0.509468, 1.0},
    {0.760264, 0.235976, 0.505794, 1.0},
    {0.764193, 0.240396, 0.502126, 1.0},
    {0.76809, 0.244817, 0.498465, 1.0},
    {0.771958, 0.249237, 0.494813, 1.0},
    {0.775796, 0.253658, 0.491171, 1.0},
    {0.779604, 0.258078, 0.487539, 1.0},
    {0.783383, 0.2625, 0.483918, 1.0},
    {0.787133, 0.266922, 0.480307, 1.0},
    {0.790855, 0.271345, 0.476706, 1.0},
    {0.798216, 0.280197, 0.469538, 1.0},
    {0.801855, 0.284626, 0.465971, 1.0},
    {0.805467, 0.289057, 0.462415, 1.0},
    {0.809052, 0.293491, 0.45887, 1.0},
    {0.812612, 0.297928, 0.455338, 1.0},
    {0.816144, 0.302368, 0.451816, 1.0},
    {0.819651, 0.306812, 0.448306, 1.0},
    {0.823132, 0.311261, 0.444806, 1.0},
    {0.826588, 0.315714, 0.441316, 1.0},
    {0.830018, 0.320172, 0.437836, 1.0},
    {0.833422, 0.324635, 0.434366, 1.0},
    {0.836801, 0.329105, 0.430905, 1.0},
    {0.840155, 0.33358, 0.427455, 1.0},
    {0.843484, 0.338062, 0.424013, 1.0},
    {0.846788, 0.342551, 0.420579, 1.0},
    {0.850066, 0.347048, 0.417153, 1.0},
    {0.853319, 0.351553, 0.413734, 1.0},
    {0.856547, 0.356066, 0.410322, 1.0},
    {0.85975, 0.360588, 0.406917, 1.0},
    {0.862927, 0.365119, 0.403519, 1.0},
    {0.866078, 0.36966, 0.400126, 1.0},
    {0.869203, 0.374212, 0.396738, 1.0},
    {0.872303, 0.378774, 0.393355, 1.0},
    {0.875376, 0.383347, 0.389976, 1.0},
    {0.878423, 0.387932, 0.3866, 1.0},
    {0.881443, 0.392529, 0.383229, 1.0},
    {0.884436, 0.397139, 0.37986, 1.0},
    {0.887402, 0.401762, 0.376494, 1.0},
    {0.89034, 0.406398, 0.37313, 1.0},
    {0.89325, 0.411048, 0.369768, 1.0},
    {0.896131, 0.415712, 0.366407, 1.0},
    {0.898984, 0.420392, 0.363047, 1.0},
    {0.901807, 0.425087, 0.359688, 1.0},
    {0.904601, 0.429797, 0.356329, 1.0},
    {0.907365, 0.434524, 0.35297, 1.0},
    {0.910098, 0.439268, 0.34961, 1.0},
    {0.9128, 0.444029, 0.346251, 1.0},
    {0.915471, 0.448807, 0.34289, 1.0},
    {0.918109, 0.453603, 0.339529, 1.0},
    {0.920714, 0.458417, 0.336166, 1.0},
    {0.923287, 0.463251, 0.332801, 1.0},
    {0.925825, 0.468103, 0.329435, 1.0},
    {0.930798, 0.477867, 0.322697, 1.0},
    {0.933232, 0.48278, 0.319325, 1.0},
    {0.93563, 0.487712, 0.315952, 1.0},
    {0.93799, 0.492667, 0.312575, 1.0},
    {0.940313, 0.497642, 0.309197, 1.0},
    {0.942598, 0.502639, 0.305816, 1.0},
    {0.944844, 0.507658, 0.302433, 1.0},
    {0.947051, 0.512699, 0.299049, 1.0},
    {0.949217, 0.517763, 0.295662, 1.0},
    {0.951344, 0.52285, 0.292275, 1.0},
    {0.953428, 0.52796, 0.288883, 1.0},
    {0.95547, 0.533093, 0.28549, 1.0},
    {0.957469, 0.53825, 0.282096, 1.0},
    {0.959424, 0.543431, 0.278701, 1.0},
    {0.961336, 0.548636, 0.275305, 1.0},
    {0.963203, 0.553865, 0.271909, 1.0},
    {0.965024, 0.559118, 0.268513, 1.0},
    {0.966798, 0.564396, 0.265118, 1.0},
    {0.968526, 0.5697, 0.261721, 1.0},
    {0.970205, 0.575028, 0.258325, 1.0},
    {0.971835, 0.580382, 0.254931, 1.0},
    {0.973416, 0.585761, 0.25154, 1.0},
    {0.974947, 0.591165, 0.248151, 1.0},
    {0.976428, 0.596595, 0.244767, 1.0},
    {0.977856, 0.602051, 0.241387, 1.0},
    {0.979233, 0.607532, 0.238013, 1.0},
    {0.980556, 0.613039, 0.234646, 1.0},
    {0.981826, 0.618572, 0.231287, 1.0},
    {0.983041, 0.624131, 0.227937, 1.0},
    {0.984199, 0.629718, 0.224595, 1.0},
    {0.985301, 0.63533, 0.221265, 1.0},
    {0.986345, 0.640969, 0.217948, 1.0},
    {0.987332, 0.646633, 0.214648, 1.0},
    {0.98826, 0.652325, 0.211364, 1.0},
    {0.989128, 0.658043, 0.2081, 1.0},
    {0.989935, 0.663787, 0.204859, 1.0},
    {0.990681, 0.669558, 0.201642, 1.0},
    {0.991365, 0.675355, 0.198453, 1.0},
    {0.991985, 0.681179, 0.195295, 1.0},
    {0.992541, 0.68703, 0.19217, 1.0},
    {0.993032, 0.692907, 0.189084, 1.0},
    {0.993456, 0.69881, 0.186041, 1.0},
    {0.994103, 0.710698, 0.180097, 1.0},
    {0.994324, 0.716681, 0.177208, 1.0},
    {0.994474, 0.722691, 0.174381, 1.0},
    {0.994553, 0.728728, 0.171622, 1.0},
    {0.994561, 0.734791, 0.168938, 1.0},
    {0.994495, 0.74088, 0.166335, 1.0},
    {0.994355, 0.746995, 0.163821, 1.0},
    {0.994141, 0.753137, 0.161404, 1.0},
    {0.993851, 0.759304, 0.159092, 1.0},
    {0.993482, 0.765499, 0.156891, 1.0},
    {0.993033, 0.77172, 0.154808, 1.0},
    {0.992505, 0.777967, 0.152855, 1.0},
    {0.991897, 0.784239, 0.151042, 1.0},
    {0.991209, 0.790537, 0.149377, 1.0},
    {0.990439, 0.796859, 0.14787, 1.0},
    {0.989587, 0.803205, 0.146529, 1.0},
    {0.988648, 0.809579, 0.145357, 1.0},
    {0.987621, 0.815978, 0.144363, 1.0},
    {0.986509, 0.822401, 0.143557, 1.0},
    {0.985314, 0.828846, 0.142945, 1.0},
    {0.984031, 0.835315, 0.142528, 1.0},
    {0.982653, 0.841812, 0.142303, 1.0},
    {0.98119, 0.848329, 0.142279, 1.0},
    {0.979644, 0.854866, 0.142453, 1.0},
    {0.977995, 0.861432, 0.142808, 1.0},
    {0.976265, 0.868016, 0.143351, 1.0},
    {0.974443, 0.874622, 0.144061, 1.0},
    {0.97253, 0.88125, 0.144923, 1.0},
    {0.970533, 0.887896, 0.145919, 1.0},
    {0.968443, 0.894564, 0.147014, 1.0},
    {0.966271, 0.901249, 0.14818, 1.0},
    {0.964021, 0.90795, 0.14937, 1.0},
    {0.961681, 0.914672, 0.15052, 1.0},
    {0.959276, 0.921407, 0.151566, 1.0},
    {0.956808, 0.928152, 0.152409, 1.0},
    {0.954287, 0.934908, 0.152921, 1.0},
    {0.951726, 0.941671, 0.152925, 1.0},
    {0.949151, 0.948435, 0.152178, 1.0},
    {0.946602, 0.95519, 0.150328, 1.0},
    {0.944152, 0.961916, 0.146861, 1.0},
    {0.941896, 0.96859, 0.140956, 1.0}
};