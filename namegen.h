#ifndef NAMEGEN_H
#define NAMEGEN_H
#define NAMEGEN_AA2_MAX_LENGTH 24
#define ship_alien_name() ship_alien_name_limited(NAMEGEN_AA2_MAX_LENGTH)
#define ship_alien_name_limited(l) name_generator(l, "VesselNameManifest.txt", "VesselNamePatterns.txt")
#define generic_location_name() generic_location_name_limited(NAMEGEN_AA2_MAX_LENGTH)
#define generic_location_name_limited(l) name_generator(l, "GenericLocationManifest.txt", "GenericLocationPatterns.txt")
char* name_generator(unsigned int maximumOutputLength, char* manifestFile, char* patternFile);
#endif
