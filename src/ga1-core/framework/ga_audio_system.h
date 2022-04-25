#pragma once
#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>
#include <framework/ga_frame_params.h>

enum GA_SOUND_TYPE
{
	GA_BASIC = FMOD_DEFAULT,//UI and other non-spatial sounds
	GA_SPATIAL = FMOD_3D
};


//This audio system is a singleton wrapper for all of the necessary FMOD system interaction functionality.
class ga_audio_system
{
public:
	static ga_audio_system* get_instance() 
	{
		static ga_audio_system instance;
		return &instance;
	}
	void update(ga_frame_params* params);

	void pause(FMOD::Channel** channel);
	void play(FMOD::Channel** channel);
	void set_looping(FMOD::Sound** sound, bool loop);
	void set_file(FMOD::Sound** sound, FMOD::Channel** channel, char* file, GA_SOUND_TYPE type);

	//these operations are not available for singletons
	ga_audio_system(ga_audio_system const&) = delete;
	void operator=(ga_audio_system const&) = delete;
	
private:
	//privately declaring the constructor
	ga_audio_system();
	
	//this system is based on the FMOD template
	void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line);
	#define ERRCHECK(result) ERRCHECK_fn(result, __FILE__, __LINE__);

	FMOD::System* _system;
};

