// Rename this file to BlynkCreds.h and then fill in the auth key

#define INDOOR 1 // Change this to 0 when compiling for the outdoor board
#define OUTDOOR !INDOOR

#define INDOOR_AUTH ""
#define OUTDOOR_AUTH ""

#if INDOOR
const char BLYNK_AUTH[] = INDOOR_AUTH;
#else
const char BLYNK_AUTH[] = OUTDOOR_AUTH;
#endif
