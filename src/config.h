#ifndef __CONFIG_H__
#define __CONFIG_H__


#define RESOLUTION	4
#define	BLUR	16
#define THRESHOLD	(RESOLUTION*64)
#define CAM_WIDTH	1024
#define CAM_HEIGHT	768
#define NOTES_WIDTH	(CAM_WIDTH/RESOLUTION*2)
#define NOTES_HEIGHT	(CAM_HEIGHT/RESOLUTION)
#define SOUND_RANGE	4

#define EFFECT_TTL	1000
#define EFFECT_SIZE	8

#define MEASURE	6000
#define SAMPLE_RATE	44100
#define MIN_FREQ	60.0L
#define MAX_FREQ	3000.0L
#define VOLUME	0.01L


#endif
