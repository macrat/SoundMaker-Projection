#ifndef __NOTE_EFFECT_H__
#define __NOTE_EFFECT_H__

#include <ofMain.h>

#include "config.h"


class NoteEffect {
private:
	const float x, y;
	const uint64_t stime;

public:
	NoteEffect(const int x, const int y) : x((float)x/NOTES_WIDTH), y((float)y/NOTES_HEIGHT), stime(ofGetElapsedTimeMillis()) { }

	void draw() const;

	static bool shouldRemove(const std::shared_ptr<NoteEffect> ne);
};


#endif
