/**
psnr library
psnr.h
Purpose: psnr library: implementation.

@author Stergios Poularakis
@version 1.0 31/08/2015
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "psnr.h"


double compute_psnr_core(double sse, double count, double SSE_MAX_VALUE)
{
	return 10 * log10(SSE_MAX_VALUE * count / sse);
}




int compute_psnr_yuv420_videos(char *filename, char *filename_ref, int WIDTH, int HEIGHT, int *nr_frames, struct video_psnr *psnr_info)
{
	FILE *fd1;
	FILE *fd2;

	int i, j;

	int y1, y2, u1, u2, v1, v2;
	int d;
	double total_y_error = 0, total_u_error = 0, total_v_error = 0;
	double frame_y_plithos = 0, frame_u_plithos = 0, frame_v_plithos = 0;
	double total_y_plithos = 0, total_u_plithos = 0, total_v_plithos = 0;
	double psnr_y = 0, psnr_u = 0, psnr_v = 0;
	double curr_logos = 0;


	psnr_info->mean_of_means_y = 0;
	psnr_info->mean_of_means_u = 0;
	psnr_info->mean_of_means_v = 0;

	psnr_info->total_psnr_y = 0;
	psnr_info->total_psnr_u = 0;
	psnr_info->total_psnr_v = 0;


	fd1 = fopen(filename, "rb");
	if (!fd1)
	{
		printf("could not open file %s\n", filename);
		return -1;
	}
	fd2 = fopen(filename_ref, "rb");
	if (!fd2)
	{
		printf("could not open file %s\n", filename_ref);
		return -1;
	}


	int WIDTH2 = WIDTH >> 1;
	int HEIGHT2 = HEIGHT >> 1;

	frame_y_plithos = WIDTH*HEIGHT;
	frame_u_plithos = WIDTH2*HEIGHT2;
	frame_v_plithos = WIDTH2*HEIGHT2;

	*nr_frames = 0;

	while ((y1 = fgetc(fd1)) != EOF | (y2 = fgetc(fd2)) != EOF)
	{
		double frame_y_error = 0, frame_u_error = 0, frame_v_error = 0;
		d = y1 - y2;
		frame_y_error += d*d;

		if (feof(fd1) || feof(fd2)) break;


		//read all Ys
		for (i = 0; i<HEIGHT; i++)
		{
			int j0 = 0;
			if (i == 0) j0 = 1;

			for (j = j0; j<WIDTH; j++)
			{
				y1 = fgetc(fd1);
				y2 = fgetc(fd2);
				d = y1 - y2;
				frame_y_error += d*d;
			}
		}
		psnr_y = compute_psnr_core(frame_y_error, frame_y_plithos, SSE_MAX_VALUE_IMAGE);
		psnr_info->mean_of_means_y += psnr_y;
		total_y_error += frame_y_error;
		total_y_plithos += frame_y_plithos;


		//read all Us
		for (i = 0; i<HEIGHT2; i++)
		{
			for (j = 0; j<WIDTH2; j++)
			{
				u1 = fgetc(fd1);
				u2 = fgetc(fd2);
				d = u1 - u2;
				frame_u_error += d*d;
			}
		}
		psnr_u = compute_psnr_core(frame_u_error, frame_u_plithos, SSE_MAX_VALUE_IMAGE);
		psnr_info->mean_of_means_u += psnr_u;
		total_u_error += frame_u_error;
		total_u_plithos += frame_u_plithos;


		//read all Vs
		for (i = 0; i<HEIGHT2; i++)
		{
			for (j = 0; j<WIDTH2; j++)
			{
				v1 = fgetc(fd1);
				v2 = fgetc(fd2);
				d = v1 - v2;
				frame_v_error += d*d;
			}
		}
		psnr_v = compute_psnr_core(frame_v_error, frame_v_plithos, SSE_MAX_VALUE_IMAGE);
		psnr_info->mean_of_means_v += psnr_v;
		total_v_error += frame_v_error;
		total_v_plithos += frame_v_plithos;

		(*nr_frames)++;
		printf("\rNumber of frames: %d", *nr_frames);
	}

	psnr_info->mean_of_means_y /= *nr_frames;
	psnr_info->mean_of_means_u /= *nr_frames;
	psnr_info->mean_of_means_v /= *nr_frames;

	psnr_info->total_psnr_y = compute_psnr_core(total_y_error, total_y_plithos, SSE_MAX_VALUE_IMAGE);
	psnr_info->total_psnr_u = compute_psnr_core(total_u_error, total_u_plithos, SSE_MAX_VALUE_IMAGE);
	psnr_info->total_psnr_v = compute_psnr_core(total_v_error, total_v_plithos, SSE_MAX_VALUE_IMAGE);

	fclose(fd1);
	fclose(fd2);


	printf("\r");

	return 0;
}



double compute_bitrate_for_video(char *video_filename, int nr_frames, double fps)
{
	double time_duration = (double)nr_frames / fps;

	double file_size = 8 * find_file_size(video_filename); // filesize in bits
	double bit_rate = file_size / time_duration / (double)1000000;

	return bit_rate;
}


double find_file_size(char *filename)
{
	FILE *fd;
	double found_size = 0;

	fd = fopen(filename, "r");
	fseek(fd, 0L, SEEK_END);
	found_size = ftell(fd);
	fclose(fd);

	return found_size;
}
