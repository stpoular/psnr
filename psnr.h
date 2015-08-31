/**
psnr library
psnr.h
Purpose: psnr library: declarations.

@author Stergios Poularakis
@version 1.0 31/08/2015
*/

#define SSE_MAX_VALUE_IMAGE 65025 //255 * 255 = 65025

struct video_psnr
{
	double mean_of_means_y;
	double mean_of_means_u;
	double mean_of_means_v;

	double total_psnr_y;
	double total_psnr_u;
	double total_psnr_v;
};


/* **********************************************************************************
** Computes and returns Y, U, V PSNR (in dB) for a test video and a reference video.
** **********************************************************************************/
int compute_psnr_yuv420_videos(char *filename, char *filename_ref, int WIDTH, int HEIGHT, int *nr_frames, struct video_psnr *psnr_info);


/* **********************************************************************************
** Computes and returns PSNR (in dB) given the SSE and the number of elements.
** SSE_MAX_VALUE is the maximum possible value of SSE (255*255 for images).
** **********************************************************************************/
double compute_psnr_core(double sse, double count, double SSE_MAX_VALUE);


/* **********************************************************************************
** Computes and returns bitrate (in Mbps) of a video file.
** **********************************************************************************/
double compute_bitrate_for_video(char *video_filename, int nr_frames, double fps);


/* **********************************************************************************
** Computes and returns the size of a file (in bytes).
** **********************************************************************************/
double find_file_size(char *filename);

