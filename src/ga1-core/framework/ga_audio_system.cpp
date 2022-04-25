#include "ga_audio_system.h"



#include <iostream>

void ga_audio_system::ERRCHECK_fn(FMOD_RESULT result, const char* file, int line)
{
    if (result != FMOD_OK)
    {
		std::cerr << file<<"("<<line<<"): FMOD error "<<result<<" - "<<FMOD_ErrorString(result)<< std::endl;
    }
}


ga_audio_system::ga_audio_system() {
	FMOD::System_Create(&_system);
	FMOD_RESULT result;
	result = _system->init(32, FMOD_INIT_3D_RIGHTHANDED, 0);
    ERRCHECK(result);
}

void ga_audio_system::update(ga_frame_params* params)
{
	_system->set3DListenerAttributes(0, (FMOD_VECTOR*)&params->_view.get_translation(), new FMOD_VECTOR({ 0.0f,0.0f,0.0f }), (FMOD_VECTOR*)&params->_view.get_forward(), (FMOD_VECTOR*)&params->_view.get_up());
	_system->update();
}

void ga_audio_system::pause(FMOD::Channel** channel)
{
	if (*channel)
	{
		(*channel)->setPaused(true);
	}
}
	

void ga_audio_system::play(FMOD::Channel** channel)
{
	if (*channel)
	{
		(*channel)->setPaused(false);
	}
}

void ga_audio_system::set_looping(FMOD::Sound** sound, bool loop)
{
	FMOD_RESULT result;
	if (*sound) 
	{
		if (loop)
		{
			FMOD_RESULT result;
			result = (*sound)->setMode(FMOD_LOOP_NORMAL);
			ERRCHECK(result);
			result = (*sound)->setLoopCount(-1);
			ERRCHECK(result);
		}
		else
		{
			result = (*sound)->setMode(FMOD_LOOP_OFF);
			ERRCHECK(result);
			result = (*sound)->setLoopCount(0);
			ERRCHECK(result);
		}
	}
	else 
	{
		std::cerr << "You can't make audio that doesn't exist loop." << std::endl;
	}
}

void ga_audio_system::set_file(FMOD::Sound** sound, FMOD::Channel** channel, char* file, GA_SOUND_TYPE type)
{

	FMOD_RESULT result;
	bool* was_paused = new bool();
	if(*channel)//if it had a channel, remember its state for later
	{
		result = (*channel)->isPlaying(was_paused);
		ERRCHECK(result);
		result = (*channel)->setPaused(true);
		ERRCHECK(result);
		
	}

	if (*sound)//if it had a sound it doesn't need that sound anymore.
	{
		result = (*sound)->release();
		ERRCHECK(result);
	}

	//make a sound and update the channel with its info
	result = _system->createSound(file, type, 0, sound);
	ERRCHECK(result);
	result = _system->playSound((*sound), 0, !(*was_paused), channel);
	ERRCHECK(result);
}

