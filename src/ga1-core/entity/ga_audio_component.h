#include "entity/ga_component.h"
#include "entity/ga_entity.h"
#include "framework/ga_audio_system.h"

class ga_audio_component : public ga_component
{
public:
	ga_audio_component(ga_entity* ent, ga_audio_system* system, GA_SOUND_TYPE type);
	virtual ~ga_audio_component();
	virtual void update(struct ga_frame_params* params) override;
	
	void pause();
	void play();
	void set_sound(char* file);
	void set_looping(bool loop);
	void sound_init(char* file, bool loop, bool start_playing);

	bool is_playing() { return _playing; }
	bool is_looping() { return _looping; }
private:
	FMOD::Sound* _sound;
	FMOD::Channel* _channel;
	ga_audio_system* _system;
	GA_SOUND_TYPE _type;
	bool _looping;
	bool _playing;
};
