

/*C*

________________________________________________________________

	module: read_acr.c
	Reads acrnema-information (even groupes)into variables
	defined in acrspi.h
	Copyright 1993, Lars Ersland, Haukeland Hospital
________________________________________________________________
  

*/

/* EXTERN defined as extern in all other modules. */
#define EXTERN
#define VERSION  19931909
#define int32 long int
#define int16 int

static char * cvs_id="$Id$";


/*Include Files:*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <xite/biff.h>
#include <xite/readarg.h>
#include "acrspi.h" 


/* #include "sie2phi.h" */

/*(Non-global) External Fuctions:*/

int printt(long int length, char text[80]);

/*(Non-global) External Variables*/

    int read_group0008(long bc)
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

	      group_length_0008 = length;
	      break;

	      case (1):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length_to_end = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      break;

	      case (16):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      recognition_code.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < recognition_code.count; i++)
	      {
		recognition_code.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (32):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      study_date.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < study_date.count; i++)
	      {
		study_date.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (34):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      acquisition_date.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < acquisition_date.count; i++)
	      {
		acquisition_date.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (35):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_date.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_date.count; i++)
	      {
		image_date.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (48):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      study_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < study_time.count; i++)
	      {
		study_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (50):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      acquisition_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < acquisition_time.count; i++)
	      {
		acquisition_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (51):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_time.count; i++)
	      {
		image_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (64):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      data_set_type = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      break;

	      case (65):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      data_set_subtype.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < data_set_subtype.count; i++)
	      {
		data_set_subtype.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (96):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      modality.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < modality.count; i++)
	      {
		modality.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (112):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      manufacturer.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < manufacturer.count; i++)
	      {
		manufacturer.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (128):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      institution_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < institution_id.count; i++)
	      {
		institution_id.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (144):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      referring_physician.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < referring_physician.count; i++)
	      {
		referring_physician.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4112):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      station_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < station_id.count; i++)
	      {
		station_id.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4144):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      procedure_description.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < procedure_description.count; i++)
	      {
		procedure_description.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4224):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      admitting_diagnosis.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < admitting_diagnosis.count; i++)
	      {
		admitting_diagnosis.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4240):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      manufacturer_model.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < manufacturer_model.count; i++)
	      {
		manufacturer_model.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	    default:  // printf("Unknown element number detected in group 0008: %04x\n", element_no);
	     break;
	 }

	return bc_group;

     } /* end of read_group0008  */


int read_group0010(long bc)

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
         /*printf("element_no %d\n", element_no);*/

	 switch (element_no) {
	   case (0):

	      b[1] = buffer1[bc_group];
              bc_group++;
              b[2] = buffer1[bc_group];
              bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
              bc_group++;

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];


	      b[1] = buffer1[bc_group];
              bc_group++;
	      b[2] = buffer1[bc_group];
              bc_group++;
	      b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
              bc_group++;

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

              group_length_0010 = length;
	      /*printf("Group Length (0010,0000):   %ld\n", group_length_0010); */

	      break;

	      case (16):  /*printf("Element nr. 16 \n"); */


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      patient_name.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      /*printf("Length (0008, 0010) : %ld  ", length); */

              	     
	      for (i = 0; i < patient_name.count; i++)
	      {
		patient_name.tword[i] = buffer1[bc_group];
		/*printf("%c", patient_name.tword[i]); */

		bc_group++;

                }
	      /* printf("\n"); */


	      break;



	      case (32): 


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      patient_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      /*printf("Length (0008, 0010) : %ld  ", length); */

              	     
	      for (i = 0; i < patient_id.count; i++)
	      {
		patient_id.tword[i] = buffer1[bc_group];
	        bc_group++;

              }
	      break;

	      case (48):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      patient_birthday.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
              for (i = 0; i < patient_birthday.count; i++)
	      {
		patient_birthday.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;

	      case (64):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      patient_sex.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
	      for (i = 0; i < patient_sex.count; i++)
	      {
		patient_sex.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;


	      case (4096):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      other_patient_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
	      for (i = 0; i < other_patient_id.count; i++)
	      {
		other_patient_id.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;



	      case (4097):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      other_patient_name.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
	      for (i = 0; i < other_patient_name.count; i++)
	      {
		other_patient_name.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;


	      case (4101):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      patient_maiden_name.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
	      for (i = 0; i < patient_maiden_name.count; i++)
	      {
		patient_maiden_name.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;


	      case (4112):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      patient_age.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
	      for (i = 0; i < patient_age.count; i++)
	      {
		patient_age.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;


	      case (4128):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      patient_size.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
	      for (i = 0; i < patient_size.count; i++)
	      {
		patient_size.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;


	      case (4144):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      patient_weight.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
	      for (i = 0; i < patient_weight.count; i++)
	      {
		patient_weight.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;

	      case (4160):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      patient_address.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
	      for (i = 0; i < patient_address.count; i++)
	      {
		patient_address.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;


	      case (4176):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      insurance_plan_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
	      for (i = 0; i < insurance_plan_id.count; i++)
	      {
		insurance_plan_id.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;


	      case (4192):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      patient_mother_maiden_name.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
	      for (i = 0; i < patient_mother_maiden_name.count; i++)
	      {
		patient_mother_maiden_name.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;


	      case (16384):


	      b[1] = buffer1[bc_group];
	      bc_group++;
              b[2] = buffer1[bc_group];
	      bc_group++;
              b[3] = buffer1[bc_group];
              bc_group++;
              b[4] = buffer1[bc_group];
	      bc_group++;

	      comments.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      
	      for (i = 0; i < comments.count; i++)
	      {
		comments.tword[i] = buffer1[bc_group];
	        bc_group++;
              }
	      break;


	     default:  // printf("Unknown element number detekted in group 0010: %04x\n", element_no);
             break;

	 }


       return bc_group;

     } /* end of read_group0010  */







    int read_group0018(long bc)

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

	      b[1] = buffer1[bc_group];
	      bc_group++;
	      b[2] = buffer1[bc_group];
	      bc_group++;
	      b[3] = buffer1[bc_group];
	      bc_group++;
	      b[4] = buffer1[bc_group];
	      bc_group++;

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      b[1] = buffer1[bc_group];
	      bc_group++;
	      b[2] = buffer1[bc_group];
	      bc_group++;
	      b[3] = buffer1[bc_group];
	      bc_group++;
	      b[4] = buffer1[bc_group];
	      bc_group++;

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

	      group_length_0018 = length;
	      break;

	      case (16):
	      b[1] = buffer1[bc_group];
	      bc_group++;
	      b[2] = buffer1[bc_group];
	      bc_group++;
	      b[3] = buffer1[bc_group];
	      bc_group++;
	      b[4] = buffer1[bc_group];
	      bc_group++;
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      contrast_bolus_agent.count = length;

	      for (i = 0; i < contrast_bolus_agent.count; i++)
	      {
		contrast_bolus_agent.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (32):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      scanning_sequence.count = length;
	      for (i = 0; i < scanning_sequence.count; i++)
	      {
		scanning_sequence.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (48):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      radionuclide.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < radionuclide.count; i++)
	      {
		radionuclide.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (64):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      cine_rate.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < cine_rate.count; i++)
	      {
		cine_rate.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (80):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      slice_thickness.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < slice_thickness.count; i++)
	      {
		slice_thickness.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

   	      case (128):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
			
	      }
	      repetition_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < repetition_time.count; i++)
	      {
		repetition_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (129):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      echo_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < echo_time.count; i++)
	      {
		echo_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (130):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      inversion_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < inversion_time.count; i++)
	      {
		inversion_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;
              
	      case (131):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      number_of_averages.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < number_of_averages.count; i++)
	      {
		number_of_averages.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (132):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      imaging_frequency.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < imaging_frequency.count; i++)
	      {
		imaging_frequency.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

    	      case (133):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      imaged_nucleus.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < imaged_nucleus.count; i++)
	      {
		imaged_nucleus.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (134):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      echo_number.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < echo_number.count; i++)
	      {
		echo_number.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (144):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      data_collection_diameter.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < data_collection_diameter.count; i++)
	      {
		data_collection_diameter.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4096):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      device_serial_number.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < device_serial_number.count; i++)
	      {
		device_serial_number.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4128):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      software_version.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < software_version.count; i++)
	      {
		software_version.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4144):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      protocol.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < protocol.count; i++)
	      {
		protocol.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4608):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      date_of_last_calibration.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < date_of_last_calibration.count; i++)
	      {
		date_of_last_calibration.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4609):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      time_of_last_calibration.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < time_of_last_calibration.count; i++)
	      {
		time_of_last_calibration.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4674):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      data_acquisition_duration.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < data_acquisition_duration.count; i++)
	      {
		data_acquisition_duration.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4688):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      receiving_coil.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < receiving_coil.count; i++)
	      {
		receiving_coil.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4689):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      transmitting_coil.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < transmitting_coil.count; i++)
	      {
		transmitting_coil.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (16384):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      comments_0018.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < comments_0018.count; i++)
	      {
		comments_0018.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (20736):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      patient_position.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < patient_position.count; i++)
	      {
		patient_position.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;   


	     default:  // printf("Unknown element number detected in group 0018: %04x\n", element_no);
	     break;

	 }


       return bc_group;

     } /* end of read_group0018  */





    int read_group0020(long bc)
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

	      group_length_0020 = length;
	      break;

	      case (16):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };

	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      study.count = length;

	      for (i = 0; i < study.count; i++)
	      {
		study.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (17):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      series.count = length;
	      for (i = 0; i < series.count; i++)
	      {
		series.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (18):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      acquisition.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < acquisition.count; i++)
	      {
		acquisition.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (19):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image.count; i++)
	      {
		image.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (32):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      patient_orientation.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < patient_orientation.count; i++)
	      {
		patient_orientation.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (48):
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

	      case (53):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_orientation.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_orientation.count; i++)
	      {
		image_orientation.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;







	      case (80):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      location.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < location.count; i++)
	      {
		location.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (96):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      laterality.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < laterality.count; i++)
	      {
		laterality.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (112):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_geometry_type.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_geometry_type.count; i++)
	      {
		image_geometry_type.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (128):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      masking_image.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < masking_image.count; i++)
	      {
		masking_image.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4096):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      series_in_study.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < series_in_study.count; i++)
	      {
		series_in_study.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4097):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      acquisitions_in_series.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < acquisitions_in_series.count; i++)
	      {
		acquisitions_in_series.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4098):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      images_in_acquisition.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < images_in_acquisition.count; i++)
	      {
		images_in_acquisition.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4128):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      reference.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < reference.count; i++)
	      {
		reference.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (12544):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      source_image_ids.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < source_image_ids.count; i++)
	      {
		source_image_ids.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (13313):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      modifying_device_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < modifying_device_id.count; i++)
	      {
		modifying_device_id.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (13314):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      modified_image_id.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < modified_image_id.count; i++)
	      {
		modified_image_id.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (13315):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      modified_image_date.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < modified_image_date.count; i++)
	      {
		modified_image_date.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (13316):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      modifying_manufacturer.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < modifying_manufacturer.count; i++)
	      {
		modifying_manufacturer.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (13317):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      modified_image_time.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < modified_image_time.count; i++)
	      {
		modified_image_time.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (13318):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      modified_image_description.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < modified_image_description.count; i++)
	      {
		modified_image_description.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (16384):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      comments_0020.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < comments_0020.count; i++)
	      {
		comments_0020.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (20480):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      original_image_identification.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      b[1] = buffer1[bc_group];
	      bc_group++;
	      b[2] = buffer1[bc_group];
	      bc_group++;
	      ioriginal_image_identification =  c256 * b[2] + b[1];
	      for (i = 0; i < original_image_identification.count; i++)
	      {
		original_image_identification.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (20482):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      original_image_identification_nomenclature.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < original_image_identification_nomenclature.count; i++)
	      {
		original_image_identification_nomenclature.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	    default:  // printf("Unknown element number detected in group 0020: %04x\n", element_no);
	     break;
	 }

	return bc_group;

     } /* end of read_group0020  */

    int read_group0028(long bc)
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

	      group_length_0028 = length;
	      break;

	      case (5):
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
	      image_dimension = c256 * b[2] + b[1];
	      break;


	      case (16):
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
	      rows = c256 * b[2] + b[1];
	      break;

	      case (17):
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
	      columns = c256 * b[2] + b[1];
	      break;

	      case (48):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      pixel_size.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < pixel_size.count; i++)
	      {
		pixel_size.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (64):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_format.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < image_format.count; i++)
	      {
		image_format.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (80):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      manipulated_image.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < manipulated_image.count; i++)
	      {
		manipulated_image.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (96):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      compression_code.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < compression_code.count; i++)
	      {
		compression_code.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (256):
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
	      bits_allocated = c256 * b[2] + b[1];
	      break;

	      case (257):
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
	      bits_stored = c256 * b[2] + b[1];
	      break;

	      case (258):
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
	      high_bit = c256 * b[2] + b[1];
	      break;

	      case (259):
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
	      pixel_representation = c256 * b[2] + b[1];
	      break;

	      case (260):
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
	      smallest_pixel_value = c256 * b[2] + b[1];
	      break;

	      case (261):
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
	      largest_pixel_value = c256 * b[2] + b[1];
	      break;

	      case (512):
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
	      image_location = c256 * b[2] + b[1];
	      break;

	      case (4176):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      window_center.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < window_center.count; i++)
	      {
		window_center.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4177):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      window_width.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < window_width.count; i++)
	      {
		window_width.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4178):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      rescale_intercept.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < rescale_intercept.count; i++)
	      {
		rescale_intercept.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (4179):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      rescale_slope.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < rescale_slope.count; i++)
	      {
		rescale_slope.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	      case (16384):
	      for (i = 1; i < 5; i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      }
	      comments_0028.count = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
	      for (i = 0; i < comments_0028.count; i++)
	      {
		comments_0028.tword[i] = buffer1[bc_group];
		bc_group++;
	      }
	      break;

	     default:  // printf("Unknown element number detected in group 0028: %04x\n", element_no);
	     break;
	 }

	return bc_group;

     } /* end of read_group0028  */


    int read_image(long bc, char title1[80])
    {
       long i, j, l, bc_group, bc_pix, length, c256;
       int msb_gr, lsb_gr, msb_el, lsb_el, element_no;
       int b[16];
       char image_nr;

       float rescale_a, rescale_b;
       int use_rescale=0;
       if ((rescale_intercept.count != 0) && (rescale_slope.count != 0)) {
	       char *buf = malloc(17);
	       char *buf1;
	       buf1 = buf;
	       memset(buf1, 0, 17);
	       strncpy(buf1, rescale_intercept.tword, rescale_intercept.count);
	       sscanf(buf1, "%f", &rescale_b);
	       buf1 = buf;
	       memset(buf1, 0, 17);
	       strncpy(buf1, rescale_slope.tword, rescale_slope.count);
	       sscanf(rescale_slope.tword, "%f", &rescale_a);
	       free(buf);
	       if (rescale_a-1.0 > 1e-4 || fabsf(rescale_b) > 1e-4) 
		       use_rescale=1;
       }

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

	      group_length_7fe0 = length;
	      break;


	      case (16):
	      for (i = 1; i < (5); i++) {
		b[i] = buffer1[bc_group];
		bc_group++;
	      };
	      length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];
              position_1=bc_group;
	      for (i = 0; i < (rows*columns*2); i++)
	      {
		buffer2[i] = buffer1[bc_group];
		bc_group++;
	      }
	      image_dimension = c256 * b[2] + b[1];  
		
	    /*  ptr=fopen("rawtest","w+b");
	      fwrite("P5 ",1,3,ptr);
	      fwrite("256",1,3,ptr);
	      fwrite(" ",1,1,ptr);
	      fwrite("256",1,3,ptr);
	      fwrite(" 4000 ",1,6,ptr);
              fwrite(buffer2, 1, (rows*columns*2), ptr); */

    /*printf("---------------------------------------------------\n");
    printf("                IMAGE INFORMATION:\n");
    printf("---------------------------------------------------\n");

    printf("Group length:                    %ld\n", group_length_7fe0);
    printf("Image Dimension:                 %3d\n", image_dimension);
    printf("Rows:                            %3d\n", rows);
    printf("Columns:                         %3d\n", columns);

    if (image_format.count != 0) {
    printf("Image Format:                    ");
    printt(image_format.count, image_format.tword);  };

    printf("Bits Allocated:                  %3d\n", bits_allocated);
    printf("Bits Stored:                     %3d\n", bits_stored);
    printf("High Bit:                        %3d\n", high_bit);
    printf("Pixel Representation:            %3d\n", pixel_representation);
    printf("Smallest Pixel Value:            %3d\n", smallest_pixel_value);
    printf("Largest Pixel Value:             %3d\n", largest_pixel_value);
    printf("Image Location:                  %4x %d\n", image_location, image_location);
    printf("   \n");
    printf("---------------------------------------------------\n");*/

    /* writing image-data to BIFF-format ....*/
    
    bc_pix=0;
    image_nr=(char) atoi(image.tword);
    /* image_nr=image.tword; */
    sprintf(l_string,"%d",(int) image_nr);

    /*    outimage = (IUS_IMAGE) Imake_image(1, image_nr, 5, rows, columns); */
    outimage = (IUS_IMAGE) Imake_image(1, l_string, 5, columns, rows);
    
    if (use_rescale) {
      for (i = 1; i <= (rows); i++)
      for (j = 1; j <= (columns); j++)
     
      {
	b[1] = buffer2[bc_pix];
	bc_pix++;
	b[2] = buffer2[bc_pix];
	bc_pix++;
	unsigned short pv = (unsigned short) (256 * b[2] + b[1]);
	outimage[1][i][j] = (unsigned short) rescale_a * pv + rescale_b;
	
       }
    } else {
      for (i = 1; i <= (rows); i++)
      for (j = 1; j <= (columns); j++)
     
      {
	b[1] = buffer2[bc_pix];
	bc_pix++;
	b[2] = buffer2[bc_pix];
	bc_pix++;
	outimage[1][i][j] = (unsigned short) (256 * b[2] + b[1]);
	
       }
    }

        break;
	 }





        return 1;
     }
