#include "entity/ga_component.h"
#include "entity/ga_entity.h"
#include "framework/ga_audio_system.h"

class ga_audio_component : public ga_component
{
public:
	//Constructor, destructor, and entity functionality
	ga_audio_component(ga_entity* ent, ga_audio_system* system, GA_SOUND_TYPE type);
	virtual ~ga_audio_component();
	virtual void update(struct ga_frame_params* params) override;

	//setup functions
	void set_sound(char* file);
	void set_looping(bool loop);
	void sound_init(char* file, bool loop, bool start_playing);

	//runtime functions
	void pause();
	void play();

	//Accessor methods
	bool is_playing() { return _playing; }
	bool is_looping() { return _looping; }

private:
	//FMOD data structures
	FMOD::Sound* _sound;
	FMOD::Channel* _channel;

	//GA_Specific data structures
	ga_audio_system* _system;
	GA_SOUND_TYPE _type;

	//info
	bool _looping;
	bool _playing;
};
