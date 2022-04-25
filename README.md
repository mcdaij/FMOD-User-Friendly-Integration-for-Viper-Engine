# RPI Final Project Repository

  * build - Files to generate a build of the engine.
  * src - GA engine framework, FMOD libraries, and integration code.

# ga_audio_system
This is an interface system for FMOD, it handles most of FMOD's backend functionality and simplifies the interface for ease of use in the Viper engine.<br>
`ga_audio_system` is a singleton that handles the audio for the entire game. It can be accessed with the function:<br>
`ga_audio_system* audio_system = ga_audio_system::get_instance();`<br>
It can also be updated in the same way as the camera or simulation already in place in the viper engine:<br>
`audio_system->update(&params);`<br>
It is declared on the stack so it will be cleared out automatically when the program ends.

# ga_audio_component
This is a `ga_component` object capable of handling spatial or static audio of any type. It incorporates standard Viper component methods, and it can be declared in the same way as any other component. In its constructor it needs a reference to its attached entity, a reference to the `ga_audio_system` object, and an enum of type `GA_SOUND_TYPE` which contains `GA_STATIC` or `GA_SPATIAL`.<br>
Ex:<br>
`ga_audio_component a_comp(&a_entity, audio_system, GA_SPATIAL);`<br><br>
After construction it needs to be provided an audio file to play either through `sound_init` or `set_sound`.
* `set_sound` takes a char* for a file name. It defaults to playing immediately and not looping. 
* `sound_init` takes a char* for a file name, a boolean for whether the sound file should loop, and a boolean for whether the sound file should start playing immediately.

`ga_audio_components` also implement several relatively self-explanatory functions<br>
* `set_looping` takes a boolean which will set whether the file will loop.
* `play` plays the current sound clip.
* `pause` pauses the current sound clip.
* `is_playing` returns a boolean for whether the clip is playing.
* `is_looping` returns a boolean for whether the clip is looping.

All spatial audio and FMOD interactions are abstracted away from the end user. 
