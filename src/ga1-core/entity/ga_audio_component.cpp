#include "ga_audio_component.h"

ga_audio_component::ga_audio_component(ga_entity* ent, ga_audio_system* system, GA_SOUND_TYPE type) :ga_component(ent)
{
	_system = system;
	_looping = false;
	_playing = false;
	_type = type;
	_sound = 0;
	_channel = 0;
}

ga_audio_component::~ga_audio_component()
{
	if (_channel && _sound)
	{
		_channel->setPaused(true);
		_sound->release();//Don't need to error check since we wouldn't handle any errors here
	}
}

void ga_audio_component::update(ga_frame_params* params)
{
	if (_channel)
	{
		ga_entity* entity = get_entity();
		_channel->set3DAttributes((FMOD_VECTOR*)&entity->get_transform(), new FMOD_VECTOR({ 0,0,0 }));
	}
}

void ga_audio_component::pause()
{
	_system->pause(&_channel);
}

void ga_audio_component::play()
{
	_system->play(&_channel);
}

void ga_audio_component::set_sound(char* file)
{
	_system->set_file(&_sound,&_channel,file, _type);
}

void ga_audio_component::set_looping(bool loop)
{
	_system->set_looping(&_sound, loop);
}

void ga_audio_component::sound_init(char* file, bool loop, bool start_playing)
{
	_system->set_file(&_sound,&_channel, file, _type);
	_system->set_looping(&_sound, loop);
	if (start_playing)
	{
		play();
	}
	else
	{
		pause();
	}
}
