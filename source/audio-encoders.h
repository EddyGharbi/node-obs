#include <obs.hpp>

#include <map>

#include "OBS_API.h"

const std::map<int, const char*> &GetAACEncoderBitrateMap();
const char *GetAACEncoderForBitrate(int bitrate);
int FindClosestAvailableAACBitrate(int bitrate);