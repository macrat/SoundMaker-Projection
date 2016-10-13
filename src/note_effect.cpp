#include "note_effect.h"


void NoteEffect::draw() const {
	const float level = (float)(ofGetElapsedTimeMillis() - stime) / EFFECT_TTL;
	ofSetColor(255, 255, 255, 8 * (1 - level));
	ofDrawCircle(x * ofGetWidth(), y * ofGetHeight(), RESOLUTION + (float)RESOLUTION*level*(EFFECT_SIZE-1));
}


bool NoteEffect::shouldRemove(const std::shared_ptr<NoteEffect> ne) {
	return ofGetElapsedTimeMillis() - ne->stime > EFFECT_TTL;
}
