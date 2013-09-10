#ifndef NAMEGEN_H
#define NAMEGEN_H
#define ship_alien_name() ship_alien_name_limited(NameLength)
#define ship_alien_name_limited(l) name_generator(l, "VesselNameManifest.txt", "VesselNamePatterns.txt")
#define generic_location_name() generic_location_name_limited(NameLength)
#define generic_location_name_limited(l) name_generator(l, "GenericLocationManifest.txt", "GenericLocationPatterns.txt")
int NameLength;
char* name_generator(unsigned int maximumOutputLength, char* manifestFile, char* patternFile);
#endif
