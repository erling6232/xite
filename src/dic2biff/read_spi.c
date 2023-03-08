/*C*

________________________________________________________________

	module: read_spi.c
	Reads spi-information into variables defined in acrspi.h
	Copyright 1993, Lars Ersland, Haukeland Hospital
________________________________________________________________
  

*/

/* EXTERN defined as extern in all other modules. */
#define EXTERN
#define VERSION  19930309
#define int32 long int
#define int16 int

static char * cvs_id="$Id$";


/*Include Files:*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*#include "e:\local\include\biff.h"
#include "e:\local\include\readarg.h"
#include "e:\local\src\include\acrspi.h" */
#include <xite/biff.h>
#include <xite/readarg.h>
#include "acrspi.h"

/* #include "sie2phi.h" */

/*(Non-global) External Fuctions:*/

int printt(long int length, char text[80]);

/*(Non-global) External Variables*/

    int read_group0009(long bc)
    {
       long i, j, l, bc_group, length;
       int msb_gr, lsb_gr, msb_el, lsb_el, element_no, c256;
       int b[16];

	 bc_group = bc-6;
	 c256 = 256;
	 lsb_el = buffer1[bc_group];
	 bc_group++;
	 msb_el = buffer1[bc_group];
	 bc_group++;

	 element_no = c256 * msb_el + lsb_el;

	 switch (element_no) {
	   case (0):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      group_length_0009 = length;
	      break;

	      case (16):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_spi_0009.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_spi_0009.count; i++)
	      {
		shadow_owner_code_spi_0009.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (18):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_cms_0009.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_cms_0009.count; i++)
	      {
		shadow_owner_code_cms_0009.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (19):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_label_id_0009.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_label_id_0009.count; i++)
	      {
		shadow_owner_code_label_id_0009.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4112):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      comments_0009.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < comments_0009.count; i++)
	      {
		comments_0009.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4117):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      unique_identifier.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < unique_identifier.count; i++)
	      {
		unique_identifier.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4160):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      for (i = 1; i < 3; i++)
	      {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      data_object_type = c256 * b[2] + b[1];
	      break;

	      case (4161):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      data_object_subtype.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < data_object_subtype.count; i++)
	      {
		data_object_subtype.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4608):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_measurements.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_measurements.count; i++)
	      {
		number_of_measurements.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4624):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      storage_mode.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < storage_mode.count; i++)
	      {
		storage_mode.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4626):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      for (i = 1; i < 5; i++)
	      {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      evaluation_mask_image = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      break;

	      case (4646):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      last_move_date.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < last_move_date.count; i++)
	      {
		last_move_date.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4647):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      last_move_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < last_move_time.count; i++)
	      {
		last_move_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4880):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      generator_identification_label.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < generator_identification_label.count; i++)
	      {
		generator_identification_label.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4881):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      gantry_identification_label.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < gantry_identification_label.count; i++)
	      {
		gantry_identification_label.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4882):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      xray_tube_identification_label.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < xray_tube_identification_label.count; i++)
	      {
		xray_tube_identification_label.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4883):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      detector_identification_label.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < detector_identification_label.count; i++)
	      {
		detector_identification_label.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4884):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      das_identification_label.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < das_identification_label.count; i++)
	      {
		das_identification_label.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4885):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      smi_identification_label.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < smi_identification_label.count; i++)
	      {
		smi_identification_label.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4886):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      cpu_identification_label.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < cpu_identification_label.count; i++)
	      {
		cpu_identification_label.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4896):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      header_version.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < header_version.count; i++)
	      {
		header_version.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;



	     default:  // printf("Unknown element number detected in group 0009: %04x\n", element_no);
	     break;
	 }

	return bc_group;

     } /* end of read_group0009  */


    int read_group0011(long bc)
    {
       long i, j, l, bc_group, length;
       int msb_gr, lsb_gr, msb_el, lsb_el, element_no, c256;
       int b[16];

	 bc_group = bc-6;
	 c256 = 256;
	 lsb_el = buffer1[bc_group];
	 bc_group++;
	 msb_el = buffer1[bc_group];
	 bc_group++;

	 element_no = c256 * msb_el + lsb_el;

	 switch (element_no) {
	   case (0):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      group_length_0011 = length;
	      break;

	      case (16):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_spi_0011.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_spi_0011.count; i++)
	      {
		shadow_owner_code_spi_0011.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (17):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_cms_0011.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_cms_0011.count; i++)
	      {
		shadow_owner_code_cms_0011.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4112):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      organ.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < organ.count; i++)
	      {
		organ.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4368):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      registration_date.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < registration_date.count; i++)
	      {
		registration_date.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4369):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      registration_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < registration_time.count; i++)
	      {
		registration_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4387):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      used_patient_weight.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < used_patient_weight.count; i++)
	      {
		used_patient_weight.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4416):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      organ_code.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < organ_code.count; i++)
	      {
		organ_code.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	     default:  // printf("Unknown element number detected in group 0011: %04x\n", element_no);
	     break;
	 }

	return bc_group;

     } /* end of read_group0011  */


    int read_group0019(long bc)
    {
       long i, j, l, bc_group, length;
       int msb_gr, lsb_gr, msb_el, lsb_el, element_no, c256;
       int b[16];

	 bc_group = bc-6;
	 c256 = 256;
	 lsb_el = buffer1[bc_group];
	 bc_group++;
	 msb_el = buffer1[bc_group];
	 bc_group++;

	 element_no = c256 * msb_el + lsb_el;

	 switch (element_no) {
	   case (0):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      group_length_0019 = length;
	      break;

	      case (16):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_cms_0019.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_cms_0019.count; i++)
	      {
		shadow_owner_code_cms_0019.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (17):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_ct_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_ct_id.count; i++)
	      {
		shadow_owner_code_siemens_ct_id.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (18):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_mr_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_mr_id.count; i++)
	      {
		shadow_owner_code_siemens_mr_id.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (19):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_ct_adjust_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_ct_adjust_id.count; i++)
	      {
		shadow_owner_code_siemens_ct_adjust_id.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (20):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_mr_adjust_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_mr_adjust_id.count; i++)
	      {
		shadow_owner_code_siemens_mr_adjust_id.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (21):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_acquisition_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_acquisition_id.count; i++)
	      {
		shadow_owner_code_siemens_acquisition_id.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4112):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      net_frequency.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < net_frequency.count; i++)
	      {
		net_frequency.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4128):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      measurement_mode.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < measurement_mode.count; i++)
	      {
		measurement_mode.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4144):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      calculation_mode.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < calculation_mode.count; i++)
	      {
		calculation_mode.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4176):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      noise_level.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < noise_level.count; i++)
	      {
		noise_level.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4192):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_data_bytes.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_data_bytes.count; i++)
	      {
		number_of_data_bytes.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;      

	      case (4624):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      total_measurement_time_nominal.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < total_measurement_time_nominal.count; i++)
	      {
		total_measurement_time_nominal.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4625):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      total_measurement_time_current.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < total_measurement_time_current.count; i++)
	      {
		total_measurement_time_current.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4626):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      start_delay_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < start_delay_time.count; i++)
	      {
		start_delay_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4627):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      dwell_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < dwell_time.count; i++)
	      {
		dwell_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4628):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_phases.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_phases.count; i++)
	      {
		number_of_phases.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4630):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      for (i = 1; i < 5; i++)
	      {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      sequence_control_mask = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      break;

	      case (4632):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      for (i = 1; i < 5; i++)
	      {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      measurement_status_mask = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      break;

	      case (4640):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_fourier_lines_nominal.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_fourier_lines_nominal.count; i++)
	      {
		number_of_fourier_lines_nominal.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4641):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_fourier_lines_current.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_fourier_lines_current.count; i++)
	      {
		number_of_fourier_lines_current.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4646):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_fourier_lines_after_zero.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_fourier_lines_after_zero.count; i++)
	      {
		number_of_fourier_lines_after_zero.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4648):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      first_measured_fourier_line.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < first_measured_fourier_line.count; i++)
	      {
		first_measured_fourier_line.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4656):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      acquisition_columns.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < acquisition_columns.count; i++)
	      {
		acquisition_columns.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4657):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      reconstruction_columns.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < reconstruction_columns.count; i++)
	      {
		reconstruction_columns.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4688):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_averages_current.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_averages_current.count; i++)
	      {
		number_of_averages_current.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4704):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      flip_angle.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < flip_angle.count; i++)
	      {
		flip_angle.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4720):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_prescans.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_prescans.count; i++)
	      {
		number_of_prescans.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4737):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      filter_type_for_raw_data.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < filter_type_for_raw_data.count; i++)
	      {
		filter_type_for_raw_data.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4738):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      filter_parameter_for_raw_data.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < filter_parameter_for_raw_data.count; i++)
	      {
		filter_parameter_for_raw_data.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4739):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      filter_type_for_image_data.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < filter_type_for_image_data.count; i++)
	      {
		filter_type_for_image_data.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4740):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      filter_parameter_for_image_data.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < filter_parameter_for_image_data.count; i++)
	      {
		filter_parameter_for_image_data.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4741):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      filter_type_for_phase_correction_data.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < filter_type_for_phase_correction_data.count; i++)
	      {
		filter_type_for_phase_correction_data.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4742):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      filter_parameter_for_phase_correction_data.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < filter_parameter_for_phase_correction_data.count; i++)
	      {
		filter_parameter_for_phase_correction_data.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4752):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_saturation_regions.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_saturation_regions.count; i++)
	      {
		number_of_saturation_regions.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4756):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_rotation_angle.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_rotation_angle.count; i++)
	      {
		image_rotation_angle.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4758):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      for (i = 1; i < 5; i++)
	      {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      coil_id_mask = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      break;

	      case (4760):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      coil_position.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < coil_position.count; i++)
	      {
		coil_position.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4768):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      epi_reconstruction_phase.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < epi_reconstruction_phase.count; i++)
	      {
		epi_reconstruction_phase.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4769):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      epi_reconstruction_slope.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < epi_reconstruction_slope.count; i++)
	      {
		epi_reconstruction_slope.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5138):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      magnetic_field_strength.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < magnetic_field_strength.count; i++)
	      {
		magnetic_field_strength.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5140):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      adc_voltage.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < adc_voltage.count; i++)
	      {
		adc_voltage.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5142):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      adc_offset.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < adc_offset.count; i++)
	      {
		adc_offset.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5152):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      transmitter_amplitude.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < transmitter_amplitude.count; i++)
	      {
		transmitter_amplitude.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5153):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_transmitter_amplitudes.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_transmitter_amplitudes.count; i++)
	      {
		number_of_transmitter_amplitudes.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5154):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      transmitter_attenuator.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < transmitter_attenuator.count; i++)
	      {
		transmitter_attenuator.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5156):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      transmitter_calibration.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < transmitter_calibration.count; i++)
	      {
		transmitter_calibration.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5158):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      transmitter_reference.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < transmitter_reference.count; i++)
	      {
		transmitter_reference.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5200):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      receiver_total_gain.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < receiver_total_gain.count; i++)
	      {
		receiver_total_gain.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5201):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      receiver_amplifier_gain.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < receiver_amplifier_gain.count; i++)
	      {
		receiver_amplifier_gain.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5202):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      receiver_preamplifier_gain.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < receiver_preamplifier_gain.count; i++)
	      {
		receiver_preamplifier_gain.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5204):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      receiver_cable_attenuation.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < receiver_cable_attenuation.count; i++)
	      {
		receiver_cable_attenuation.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5205):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      receiver_reference_gain.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < receiver_reference_gain.count; i++)
	      {
		receiver_reference_gain.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5206):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      receiver_filter_frequency.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < receiver_filter_frequency.count; i++)
	      {
		receiver_filter_frequency.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5216):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      reconstruction_scale_factor.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < reconstruction_scale_factor.count; i++)
	      {
		reconstruction_scale_factor.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5218):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      reference_scale_factor.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < reference_scale_factor.count; i++)
	      {
		reference_scale_factor.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5232):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      phase_gradient_amplitude.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < phase_gradient_amplitude.count; i++)
	      {
		phase_gradient_amplitude.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5233):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      readout_gradient_amplitude.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < readout_gradient_amplitude.count; i++)
	      {
		readout_gradient_amplitude.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5234):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      selection_gradient_amplitude.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < selection_gradient_amplitude.count; i++)
	      {
		selection_gradient_amplitude.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5248):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      gradient_delay_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < gradient_delay_time.count; i++)
	      {
		gradient_delay_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5250):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      total_gradient_delay_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < total_gradient_delay_time.count; i++)
	      {
		total_gradient_delay_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5264):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      sensitivity_correction_lable.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < sensitivity_correction_lable.count; i++)
	      {
		sensitivity_correction_lable.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5280):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      for (i = 1; i < 5; i++)
	      {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      rf_watchdog_mask = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      break;

	      case (5282):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      rf_power_error_indicator.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < rf_power_error_indicator.count; i++)
	      {
		rf_power_error_indicator.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5285):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      sar_whole_body.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < sar_whole_body.count; i++)
	      {
		sar_whole_body.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5286):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      specific_energy_dose.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < specific_energy_dose.count; i++)
	      {
		specific_energy_dose.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5296):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      for (i = 1; i < 5; i++)
	      {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      adjustment_status_mask = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      break;

	      case (5313):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      epi_capacity.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < epi_capacity.count; i++)
	      {
		epi_capacity.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5314):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      epi_inductance.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < epi_inductance.count; i++)
	      {
		epi_inductance.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5315):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      epi_switch_configuration_code.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < epi_switch_configuration_code.count; i++)
	      {
		epi_switch_configuration_code.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5316):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      epi_switch_hardware_code.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < epi_switch_hardware_code.count; i++)
	      {
		epi_switch_hardware_code.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5317):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      epi_switch_delay_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < epi_switch_delay_time.count; i++)
	      {
		epi_switch_delay_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5392):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      parameter_file_name.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < parameter_file_name.count; i++)
	      {
		parameter_file_name.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5393):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      sequence_file_name.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < sequence_file_name.count; i++)
	      {
		sequence_file_name.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5394):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      sequence_file_owner.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < sequence_file_owner.count; i++)
	      {
		sequence_file_owner.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5395):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      sequence_description.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < sequence_description.count; i++)
	      {
		sequence_description.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (5396):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      epi_file_name.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < epi_file_name.count; i++)
	      {
		epi_file_name.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	     default:  // printf("Unknown element number detected in group 0019: %04x %d\n", element_no, element_no);
	     break;
	 }

	return bc_group;

     } /* end of read_group0019  */







    int read_group0021(long bc)
    {
       long i, j, l, bc_group, length;
       int msb_gr, lsb_gr, msb_el, lsb_el, element_no, c256;
       int b[16];

	 bc_group = bc-6;
	 c256 = 256;
	 lsb_el = buffer1[bc_group];
	 bc_group++;
	 msb_el = buffer1[bc_group];
	 bc_group++;

	 element_no = c256 * msb_el + lsb_el;

	 switch (element_no) {
	   case (0):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      group_length_0021 = length;
	      break;

	      case (16):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_ubmed_0021.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_ubmed_0021.count; i++)
	      {
		shadow_owner_code_siemens_ubmed_0021.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (17):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_cms_0021.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_cms_0021.count; i++)
	      {
		shadow_owner_code_siemens_cms_0021.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (18):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_ct_0021.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_ct_0021.count; i++)
	      {
		shadow_owner_code_siemens_ct_0021.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (19):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_mr_0021.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_mr_0021.count; i++)
	      {
		shadow_owner_code_siemens_mr_0021.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (34):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_ct_raw_0021.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_ct_raw_0021.count; i++)
	      {
		shadow_owner_code_siemens_ct_raw_0021.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (35):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_mr_raw_0021.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_mr_raw_0021.count; i++)
	      {
		shadow_owner_code_siemens_mr_raw_0021.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (80):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_mr_spect_0021.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_mr_spect_0021.count; i++)
	      {
		shadow_owner_code_siemens_mr_spect_0021.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4112):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      zoom.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < zoom.count; i++)
	      {
		zoom.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4113):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      target.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < target.count; i++)
	      {
		target.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4128):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      for (i = 1; i < 3; i++)
	      {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      roi_mask =  c256 * b[2] + b[1];
	      break;

	      case (4384):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      field_of_view.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < field_of_view.count; i++)
	      {
		field_of_view.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4386):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_magnification_factor.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_magnification_factor.count; i++)
	      {
		image_magnification_factor.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4400):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      view_direction.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < view_direction.count; i++)
	      {
		view_direction.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4402):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      patient_rest_direction.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < patient_rest_direction.count; i++)
	      {
		patient_rest_direction.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4448):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_position.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_position.count; i++)
	      {
		image_position.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4449):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_normal.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_normal.count; i++)
	      {
		image_normal.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4451):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_distance.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_distance.count; i++)
	      {
		image_distance.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4453):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      for (i = 1; i < 3; i++)
	      {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_positioning_history_mask =  c256 * b[2] + b[1];
	      break;

	      case (4458):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_row.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_row.count; i++)
	      {
		image_row.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4459):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_column.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_column.count; i++)
	      {
		image_column.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4464):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      patient_orientation_set_1.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < patient_orientation_set_1.count; i++)
	      {
		patient_orientation_set_1.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4465):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      patient_orientation_set_2.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < patient_orientation_set_2.count; i++)
	      {
		patient_orientation_set_2.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4480):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      study_name.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < study_name.count; i++)
	      {
		study_name.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4482):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      study_type.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < study_type.count; i++)
	      {
		study_type.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4896):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      phase_correction_rows_sequence.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < phase_correction_rows_sequence.count; i++)
	      {
		phase_correction_rows_sequence.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4897):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      phase_correction_columns_sequence.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < phase_correction_columns_sequence.count; i++)
	      {
		phase_correction_columns_sequence.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4898):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      phase_correction_rows_reconstruction.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < phase_correction_rows_reconstruction.count; i++)
	      {
		phase_correction_rows_reconstruction.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4900):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      phase_correction_columns_reconstruction.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < phase_correction_columns_reconstruction.count; i++)
	      {
		phase_correction_columns_reconstruction.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4912):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_3d_raw_partitions_nominal.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_3d_raw_partitions_nominal.count; i++)
	      {
		number_of_3d_raw_partitions_nominal.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4913):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_3d_raw_partitions_current.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_3d_raw_partitions_current.count; i++)
	      {
		number_of_3d_raw_partitions_current.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4916):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_3d_image_partitions.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_3d_image_partitions.count; i++)
	      {
		number_of_3d_image_partitions.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4921):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      slab_thickness.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < slab_thickness.count; i++)
	      {
		slab_thickness.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4918):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      actual_3d_image_partition_number.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < actual_3d_image_partition_number.count; i++)
	      {
		actual_3d_image_partition_number.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4928):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_slices_nominal.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_slices_nominal.count; i++)
	      {
		number_of_slices_nominal.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4929):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_slices_current.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_slices_current.count; i++)
	      {
		number_of_slices_current.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4930):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      current_slice_number.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < current_slice_number.count; i++)
	      {
		current_slice_number.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4931):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      current_group_number.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < current_group_number.count; i++)
	      {
		current_group_number.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4932):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      current_slice_distance_factor.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < current_slice_distance_factor.count; i++)
	      {
		current_slice_distance_factor.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4933):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      mip_start_row.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < mip_start_row.count; i++)
	      {
		mip_start_row.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4934):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      mip_stop_row.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < mip_stop_row.count; i++)
	      {
		mip_stop_row.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4935):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      mip_start_column.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < mip_start_column.count; i++)
	      {
		mip_start_column.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4936):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      mip_stop_column.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < mip_stop_column.count; i++)
	      {
		mip_stop_column.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4937):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      mip_start_slice.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < mip_start_slice.count; i++)
	      {
		mip_start_slice.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4938):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      mip_stop_slice.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < mip_stop_slice.count; i++)
	      {
		mip_stop_slice.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4943):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      order_of_slices.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < order_of_slices.count; i++)
	      {
		order_of_slices.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4944):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      for (i = 1; i < 5; i++)
	      {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      signal_mask = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      break;

	      case (4946):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      delay_time_after_trigger.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < delay_time_after_trigger.count; i++)
	      {
		delay_time_after_trigger.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4947):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      rr_interval.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < rr_interval.count; i++)
	      {
		rr_interval.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4948):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_trigger_pulse.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_trigger_pulse.count; i++)
	      {
		number_of_trigger_pulse.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4950):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      repetition_time_effective.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < repetition_time_effective.count; i++)
	      {
		repetition_time_effective.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4951):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      gate_phase.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < gate_phase.count; i++)
	      {
		gate_phase.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4952):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      gate_threshold.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < gate_threshold.count; i++)
	      {
		gate_threshold.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4953):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      gated_ratio.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < gated_ratio.count; i++)
	      {
		gated_ratio.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4960):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_interpolated_images.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_interpolated_images.count; i++)
	      {
		number_of_interpolated_images.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4976):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_echoes.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_echoes.count; i++)
	      {
		number_of_echoes.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4978):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      second_echo_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < second_echo_time.count; i++)
	      {
		second_echo_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4992):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      cardiac_code.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < cardiac_code.count; i++)
	      {
		cardiac_code.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	     default:  // printf("Unknown element number detected in group 0021: %04x\n", element_no);
	     break;
	 }

	return bc_group;

     } /* end of read_group0021  */


    int read_group0051(long bc)
    {
       long i, j, l, bc_group, length;
       int msb_gr, lsb_gr, msb_el, lsb_el, element_no, c256;
       int b[16];

	 bc_group = bc-6;
	 c256 = 256;
	 lsb_el = buffer1[bc_group];
	 bc_group++;
	 msb_el = buffer1[bc_group];
	 bc_group++;

	 element_no = c256 * msb_el + lsb_el;

	 switch (element_no) {
	   case (0):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      group_length_0051 = length;
	      break;

	      case (16):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_cms_0051.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_cms_0051.count; i++)
	      {
		shadow_owner_code_siemens_cms_0051.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4112):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_text.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_text.count; i++)
	      {
		image_text.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;


	     default:  // printf("Unknown element number detected in group 0051: %04x\n", element_no);
	     break;
	 }

	return bc_group;

     }
      /* end of read_group0051  */


    int read_group0029(long bc)
    {
       long i, j, l, bc_group, length;
       int msb_gr, lsb_gr, msb_el, lsb_el, element_no, c256;
       int b[16];

	 bc_group = bc-6;
	 c256 = 256;
	 lsb_el = buffer1[bc_group];
	 bc_group++;
	 msb_el = buffer1[bc_group];
	 bc_group++;

	 element_no = c256 * msb_el + lsb_el;

	 switch (element_no) {
	   case (0):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      group_length_0029 = length;
	      break;

	      case (16):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_spi_0029.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_spi_0029.count; i++)
	      {
		shadow_owner_code_siemens_spi_0029.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (17):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      shadow_owner_code_siemens_cms_0029.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < shadow_owner_code_siemens_cms_0029.count; i++)
	      {
		shadow_owner_code_siemens_cms_0029.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4368):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      window_style.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < window_style.count; i++)
	      {
		window_style.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4384):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      pixel_quality_code.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < pixel_quality_code.count; i++)
	      {
		pixel_quality_code.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4386):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      pixel_quality_value.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < pixel_quality_value.count; i++)
	      {
		pixel_quality_value.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4432):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      archive_code.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < archive_code.count; i++)
	      {
		archive_code.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4433):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      exposure_code.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < exposure_code.count; i++)
	      {
		exposure_code.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4434):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      sort_code.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < sort_code.count; i++)
	      {
		sort_code.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4448):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      splash.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < splash.count; i++)
	      {
		splash.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	     default:  // printf("Unknown element number detected in group 0029: %04x\n", element_no);
	     break;
	 }

	return bc_group;

     }
      /* end of read_group0029  */
