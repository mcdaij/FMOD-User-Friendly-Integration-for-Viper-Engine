/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "framework/ga_camera.h"
#include "framework/ga_compiler_defines.h"
#include "framework/ga_input.h"
#include "framework/ga_sim.h"
#include "framework/ga_output.h"
#include "jobs/ga_job.h"

#include "entity/ga_entity.h"
#include "entity/ga_lua_component.h"
#include "entity/ga_audio_component.h"

#include "graphics/ga_cube_component.h"
#include "graphics/ga_program.h"

#include "gui/ga_font.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#if defined(GA_MINGW)
#include <unistd.h>
#endif


#include <iostream>
#include "framework/ga_audio_system.h"

ga_font* g_font = nullptr;
static void set_root_path(const char* exepath);

int main(int argc, const char** argv)
{
	ga_audio_system* audio_system = ga_audio_system::get_instance();

	set_root_path(argv[0]);

	ga_job::startup(0xffff, 256, 256);

	// Create objects for three phases of the frame: input, sim and output.
	ga_input* input = new ga_input();
	ga_sim* sim = new ga_sim();
	ga_output* output = new ga_output(input->get_window());

	// Create the default font:
	g_font = new ga_font("VeraMono.ttf", 16.0f, 512, 512);

	// Create camera.
	ga_camera* camera = new ga_camera({ 0.0f, 7.0f, 20.0f });
	ga_quatf rotation;
	rotation.make_axis_angle(ga_vec3f::y_vector(), ga_degrees_to_radians(180.0f));
	camera->rotate(rotation);
	rotation.make_axis_angle(ga_vec3f::x_vector(), ga_degrees_to_radians(15.0f));
	camera->rotate(rotation);

	//Create some demo cubes!
	ga_entity kick_entity;
	ga_audio_component kick_aud(&kick_entity, audio_system, GA_SPATIAL);
	ga_cube_component kick_cube(&kick_entity, "data/textures/rpi.png");

	ga_entity bass_entity;
	ga_audio_component bass_aud(&bass_entity, audio_system, GA_SPATIAL);
	ga_cube_component bass_cube(&bass_entity, "data/textures/rpi.png");

	ga_entity lead_entity;
	ga_audio_component lead_aud(&lead_entity, audio_system, GA_SPATIAL);
	ga_cube_component lead_cube(&lead_entity, "data/textures/rpi.png");

	kick_aud.sound_init("data/FMOD/kick.aiff", true, true);
	lead_aud.sound_init("data/FMOD/main.wav", true, true);
	bass_aud.sound_init("data/FMOD/base.wav", true, true);
	kick_aud.get_entity()->translate({ -7.5,7,20 });
	lead_aud.get_entity()->translate({ -7.5,7,20 });
	bass_aud.get_entity()->translate({ -7.5,7,20 });

	ga_quatf axis_angle;
	axis_angle.make_axis_angle(ga_vec3f::y_vector(), ga_degrees_to_radians(45) * 2);
	lead_aud.get_entity()->rotate(axis_angle);
	lead_aud.get_entity()->translate({ 7.5,0,7.5 });

	axis_angle.make_axis_angle(ga_vec3f::y_vector(), ga_degrees_to_radians(45) * 4);
	bass_aud.get_entity()->rotate(axis_angle);
	bass_aud.get_entity()->translate({ 14,0,0 });
	
	sim->add_entity(&kick_entity);
	sim->add_entity(&bass_entity);
	sim->add_entity(&lead_entity);

	// Main loop:
	while (true)
	{
		
		// We pass frame state through the 3 phases using a params object.
		ga_frame_params params;

		// Gather user input and current time.
		if (!input->update(&params))
		{
			break;
		}

		// Update the camera.
		camera->update(&params);

		// Run gameplay.
		sim->update(&params);

		//Update audio system accordingly
		audio_system->update(&params);

		// Perform the late update.
		sim->late_update(&params);

		// Draw to screen.
		output->update(&params);
	}

	delete output;
	delete sim;
	delete input;
	delete camera;

	ga_job::shutdown();

	return 0;
}

char g_root_path[256];
static void set_root_path(const char* exepath)
{
#if defined(GA_MSVC)
	strcpy_s(g_root_path, sizeof(g_root_path), exepath);

	// Strip the executable file name off the end of the path:
	char* slash = strrchr(g_root_path, '\\');
	if (!slash)
	{
		slash = strrchr(g_root_path, '/');
	}
	if (slash)
	{
		slash[1] = '\0';
	}
#elif defined(GA_MINGW)
	char* cwd;
	char buf[PATH_MAX + 1];
	cwd = getcwd(buf, PATH_MAX + 1);
	strcpy_s(g_root_path, sizeof(g_root_path), cwd);

	g_root_path[strlen(cwd)] = '/';
	g_root_path[strlen(cwd) + 1] = '\0';
#endif
}
