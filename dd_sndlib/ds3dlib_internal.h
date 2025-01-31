/*
 * $Source: $
 * $Revision: 9 $
 * $Author: Samir $
 * $Date: 8/23/99 5:29p $
 *
 * Win32 Sound Library Internal Header.
 *
 * $Log: /DescentIII/Main/dd_sndlib/ds3dlib_internal.h $
 * 
 * 9     8/23/99 5:29p Samir
 * incremental EAX 2.0 checkin
 * 
 * 8     5/03/99 3:12a Samir
 * fixed up aureal so it works (a little slow though...)
 * 
 * 7     4/29/99 3:01p Samir
 * added code for direct sound mixers only (and function for Aureal
 * really) that will use direct sound looping for simple loops.
 * 
 * 6     4/22/99 10:33p Samir
 * modifications so that DirectSound mixers use one thread for all looping
 * and streaming sounds.   It worked without crashing for about twenty
 * minutes of playing from level 1 to level 2 of D3.  We'll see.
 * 
 * 5     4/13/99 4:09p Samir
 * more priority stuff.
 * 
 * 4     4/12/99 7:14p Samir
 * prioritization code added.
 * 
 * 3     4/10/99 5:08p Samir
 * took out obsolete data from play_information structure that should save
 * around 70 bytes per instance.
 * 
 * 2     4/06/99 8:29p Samir
 * added error check system.
 * 
 * 1     4/06/99 8:16p Samir
 * Initial rev.
 * 
 */

#ifndef DS3DLIB_INTERNAL_H
#define DS3DLIB_INTERNAL_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#include "win\directx\dsound.h"
#include "ssl_lib.h"

// Sound Library Internal Error Codes
#define SSL_OK								0
#define SSL_ERROR_GENERIC				-1
#define SSL_ERROR_SAMPLE_NODATA		-2
#define SSL_ERROR_STREAMMIXER			-3

// Sound Status
#define SSF_UNUSED			0
#define SSF_PLAY_NORMAL		1
#define SSF_PLAY_LOOPING	2
#define SSF_PAUSED			4
#define SSF_PLAY_STREAMING 8
#define SSF_BUFFERED_LOOP	64
#define SSF_BUFFERED_STRM	128


#define SBT_PRIMARY		0
#define SBT_2D				1
#define SBT_3D				2

// looping methods
#define DSLOOP_SMART_METHOD		0
#define DSLOOP_BUFFER_METHOD		1
#define DSLOOP_STREAM_METHOD		2

#define DSBUFLOOP_INIT_STEP		-1
#define DSBUFLOOP_LOOP_STEP		0
#define DSBUFLOOP_FINISH_STEP		1

// used to time threads.
#define DSPB_TICK_INTERVAL      .01 // Primary buffer update rate (in seconds)
#define DSPB_TICK_MILLISECONDS  (DSPB_TICK_INTERVAL * 1000)

class win_llsSystem;

typedef struct DSLOOPSTREAM
{
	ubyte method;						// method 0 for streamed method, 1 for emulated method
	ubyte playing;
	ubyte please_close;

	union									// dependant on method.
	{
		ubyte close_on_next;
		sbyte loop_step;				// method 1, = 0 for start loop, 1 = mid loop, 2 = end loop.
	};

	union									// dependant on method. 
	{
		DWORD bytes_left;
		float loop_timer;				// method 1, time spent in current loop step
	};	

	char *current_position;			// used for streamed method
	HANDLE hEvent;						// used for event notification.
	DWORD half_buffer_point;
	DWORD last_half;
	int num_written;
	ubyte time_slice;
	bool kill_me;
	bool f_sample_16bit;
	char silence_byte;
} DSLOOPSTREAM;

typedef struct DSSTREAMTAG 
{
	// These 2 pointers are the same on good sound cards
	LPDIRECTSOUNDBUFFER		m_lp_primary_buffer; // pointer to the primary sound buffer
	LPDIRECTSOUNDBUFFER		m_lp_looping_buffer; // pointer to the looping sound buffer
	bool m_f_secondary_looping;

	int BufferSize;  // Size of the primary buffer

	int MaxWriteSamples;
	int MaxWriteBytes;

	unsigned long thread_handle;
//	unsigned thread_id;
	volatile bool thread_request_kill;
	volatile bool thread_alive;

	// Updating state variables
	int NextWritePos;			// Last bit position we wrote sound data to
	int LastPlayPos;			// Last bit position that the play pointer was recorded at

} DSSTREAM;


typedef struct tPSBInfo 
{
	float volume;
	union {
		float pan;
		pos_state *cur_pos;
	};
	float reverb;
	ushort freq;
	ushort priority;
	bool looping;
}
tPSBInfo;

// Sound item info (cache list)
class sound_buffer_info 
{
public:
	sound_buffer_info() {m_status = SSF_UNUSED;m_sound_buffer = NULL;m_sound_buffer_3d = NULL;s=NULL; }
	
	play_information *play_info;

	int				m_sound_index;		 // Index of sound
	int				m_unique_id;		 // Unique id for the currently playing sample

	// Not needed by the software mixer
	volatile DSLOOPSTREAM*	s;			// Streaming info for a looping sample
	union {
		LPDIRECTSOUNDBUFFER	m_sound_buffer;	// May be a pointer to a 2d or 3d sound buffer
		void * m_snd_obj;								// used for non direct sound systems.
	};
	LPDIRECTSOUND3DBUFFER	m_sound_buffer_3d; // Used for 3d interface
	LPKSPROPERTYSET			m_lpksps;	// used mainly for EAX 2.0 but is a direct sound object.

	short m_mixer_type;					// aureal, ds3d, ds_8?
	short m_buffer_type;					// Buffer type 2d or 3d			 

	char *sample_data;
	int sample_length;					// used for storage purposes.

	bool stereo;
	sbyte bps;
	unsigned char	m_status;			 // Sound status
	unsigned char pad;

	float m_volume;						// kept for priority.
};


// stop a sound buffer.
void sb_stop_buffer(sound_buffer_info *sb);

// frees a sound buffer.
void sb_free_buffer(sound_buffer_info *sb);

// load data into sound buffer
bool sb_load_buffer(sound_buffer_info *sb, void *sample_data, int length);

// update current position of sound buffer
int sb_get_current_position(sound_buffer_info *sb, uint *writep);

// locks a sound buffer
bool sb_lock_buffer(sound_buffer_info *sb, uint dwWriteCursor, uint dwWriteBytes, 
  void **lplpvAudioPtr1,  uint *lpdwAudioBytes1, 
  void **lplpvAudioPtr2,  uint *lpdwAudioBytes2);

// unlocks an allocated sound buffer
bool sb_unlock_buffer(sound_buffer_info *sb, void *ptr1, uint len1, void *ptr2, uint len2);

// this will initialize the looping thread
bool sb_loop_thread_init(win_llsSystem *lls);

//	kill looped thread
void sb_loop_thread_kill();

// initializes a loop for the loop thread.
bool sb_loop_element_init(sound_buffer_info *sb, char *sample_ptr, int sound_length, int buffer_size);

// initialize streaming audio.
bool sb_stream_element_init(sound_buffer_info *sb, char *sample_ptr, int sound_length, int buffer_size);

// these work on both loops and streams.
void sb_loop_element_kill(sound_buffer_info *sb);
void sb_loop_element_wait_until_dead(sound_buffer_info *sb);

// these work on buffered streams only
void sb_stream_element_kill(sound_buffer_info *sb);
void sb_stream_buffered_update(sound_buffer_info *sb);

// used for buffered loops.
char *sb_get_loop_info(const sound_buffer_info *sb, int *loop_byte_start, int *loop_byte_end, bool *is_16_bit);
char *sb_get_loop_step_info(const sound_buffer_info *sb, int step, bool is16bit, int *length);
void sb_buffered_loop_step(win_llsSystem *lls, sound_buffer_info *sb, int force_next_step=-2);

#include "ds3dlib.h"
#include "ddsndgeometry.h"

#endif