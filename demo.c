/**
Demo program.
run_all.c
Purpose: Computes PSNR (in dB) and Bitrate (in Mbps) given original and compressed video files.

@author Stergios Poularakis
@version 1.0 31/08/2015
*/

#include <stdio.h>
#include <stdlib.h>

#include "psnr.h"



int main(int argc, char *argv[])
{
	
	char yuv_filename[1001];
	char yuv_filename_ref[1001];
	char encoded_video_filename[1001];

	int width = 0, height = 0;
	int nr_frames = 0;
	double fps = 0.0;

	struct video_psnr psnr_info;

	if (argc < 7)
	{
		printf("Usage: psnr [yuv_filename_ref] [yuv_filename] [video width] [video height] [encoded_video_filename] [fps] [results_filename (Optional)]\n");
		exit(-1);
	}

	sprintf(yuv_filename_ref, "%s", argv[1]);
	sprintf(yuv_filename, "%s", argv[2]);

	width = atoi(argv[3]);
	height = atoi(argv[4]);

	sprintf(encoded_video_filename, "%s", argv[5]);
	fps = atof(argv[6]);

	compute_psnr_yuv420_videos(yuv_filename, yuv_filename_ref, width, height, &nr_frames, &psnr_info);
	double bitrate = compute_bitrate_for_video(encoded_video_filename, nr_frames, fps);

	printf("Number of frames: %d\n", nr_frames);
	printf("fps: %.2lf\n", fps);
	printf("%.1lf %.1lf %.1lf %.7lf\n", psnr_info.mean_of_means_y, psnr_info.mean_of_means_u, psnr_info.mean_of_means_v, bitrate);

	if (argc > 7)
	{
		FILE *fd;
		char results_filename[1001];
		sprintf(results_filename, "%s", argv[7]);

		fd = fopen(results_filename, "w");
		fprintf(fd, "%.1lf %.1lf %.1lf %.7lf\n", psnr_info.mean_of_means_y, psnr_info.mean_of_means_u, psnr_info.mean_of_means_v, bitrate);
		fclose(fd);
	}
	

	return 0;
}

