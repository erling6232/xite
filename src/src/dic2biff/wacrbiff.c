/*C*

________________________________________________________________

	module: wri_acr.c
	Prints out all ACR-NEMA-groups to stdout.
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

/*#include "e:\local\include\biff.h"
#include "e:\local\include\readarg.h"
#include "e:\local\src\include\acrspi.h" */

#include <xite/biff.h>
#include <xite/readarg.h>
#include "acrspi.h"
/* #include "sie2phi.h" */

/*(Non-global) External Fuctions:*/

/*int Iappend_line(outimage, l_string)long int length, char text[80]);*/

/*(Non-global) External Variables*/

/* Added by arvid on SGI  960202 */
extern char l_string[256];
extern IUS_IMAGE outimage;

void write_group0008()
{

    sprintf(l_string,"Group Length (0008,0000): %ld", group_length_0008);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    sprintf(l_string,"Length To End (of file): %ld", length_to_end);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (recognition_code.count != 0) {
    sprintf(l_string,"Recognition Code:                %s", recognition_code.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (study_date.count != 0) {
    sprintf(l_string,"Study Date:                      %s", study_date.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (series_date.count != 0) {
    sprintf(l_string,"Series Date:                     %s", series_date.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (acquisition_date.count != 0) {
    sprintf(l_string,"Acquisition Date:                %s", acquisition_date.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (image_date.count != 0) {
    sprintf(l_string,"Image Date:                      %s", image_date.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (study_time.count != 0) {
    sprintf(l_string,"Study Time:                      %s", study_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (series_time.count != 0) {
    sprintf(l_string,"Series Time:                     %s", series_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (acquisition_time.count != 0) {
    sprintf(l_string,"Acquisition Time:                %s", acquisition_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (image_time.count != 0) {
    sprintf(l_string,"Image Time:                      %s", image_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    sprintf(l_string,"Data Set Type:                   %d", data_set_type);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (data_set_subtype.count != 0) {
    sprintf(l_string,"Data Set Subtype:                %s", data_set_subtype.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (modality.count != 0) {
    sprintf(l_string,"Modality:                        %s", modality.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (manufacturer.count != 0) {
    sprintf(l_string,"Manufacturer:                    %s", manufacturer.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (institution_id.count != 0) {
    sprintf(l_string,"Institution ID:                  %s", institution_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };
    
#ifndef RESTRICTED
    if (referring_physician.count != 0) {
    sprintf(l_string,"Referring Physician:             %s", referring_physician.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };
#endif

    if (network_id.count != 0) {
    sprintf(l_string,"Network ID:                      %s", network_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (station_id.count != 0) {
    sprintf(l_string,"Station ID:                      %s", station_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (procedure_description.count != 0) {
    sprintf(l_string,"Procedure Description:           %s", procedure_description.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (institutional_department.count != 0) {
    sprintf(l_string,"Institutional Depertment:        %s", institutional_department.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (attending_physician.count != 0) {
    sprintf(l_string,"Attending Physician:             %s", attending_physician.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (radiologist.count != 0) {
    sprintf(l_string,"Radiologist:                     %s", radiologist.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (operator_identification.count != 0) {
    sprintf(l_string,"Operator Identification:         %s", operator_identification.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

#ifndef RESTRICTED
    if (admitting_diagnosis.count != 0) {
    sprintf(l_string,"Admitting Diagnosis:             %s", admitting_diagnosis.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };
#endif

    if (manufacturer_model.count != 0) {
    sprintf(l_string,"Manufacturer Model:              %s", manufacturer_model.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (comments_0008.count != 0) {
    sprintf(l_string,"Comments:                        %s", comments_0008.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };


}



int write_group0010()
{

    sprintf(l_string,"Group Length (0010,0000):        %ld  ", group_length_0010);
    Iappend_line((IUB_IMAGE) outimage, l_string);
    
#ifndef RESTRICTED
    if (patient_name.count != 0) {
    sprintf(l_string,"Patient Name:                    %s", patient_name.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_id.count != 0) {
    sprintf(l_string,"Patient ID:                      %s", patient_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);   };

    if (patient_birthday.count != 0) {
    sprintf(l_string,"Patient Birthday:                %s", patient_birthday.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_sex.count != 0) {
    sprintf(l_string,"Patient Sex:                     %s", patient_sex.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);   };

    if (other_patient_id.count != 0) {
    sprintf(l_string,"Other Patient ID:                %s", other_patient_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (other_patient_name.count != 0) {
    sprintf(l_string,"Other Patient Name:              %s", other_patient_name.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_maiden_name.count != 0) {
    sprintf(l_string,"Patient Maiden Name:             %s", patient_maiden_name.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_age.count != 0) {
    sprintf(l_string,"Patient Age:                     %s", patient_age.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_size.count != 0) {
    sprintf(l_string,"Patient Size:                    %s", patient_size.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_weight.count != 0) {
    sprintf(l_string,"Patient Weight:                  %s", patient_size.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_address.count != 0) {
    sprintf(l_string,"Patient Address:                 %s", patient_address.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (insurance_plan_id.count != 0) {
    sprintf(l_string,"Insurance Plan ID:               %s", insurance_plan_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);   };

    if (patient_mother_maiden_name.count != 0) {
    sprintf(l_string,"Patient Mother Maiden Name:      %s", patient_mother_maiden_name.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);   };

    if (comments.count != 0) {
    sprintf(l_string,"Comments:                        %s", comments.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);      };
    
#endif
   return 0;

}

int write_group0018()
{

    sprintf(l_string,"Group Length (0018,0000):        %ld  ", group_length_0018);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (contrast_bolus_agent.count != 0) {
    sprintf(l_string,"Contrast Bolus Agent:            %s", contrast_bolus_agent.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (scanning_sequence.count != 0) {
    sprintf(l_string,"Scanning Sequence:               %s", scanning_sequence.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (radionuclide.count != 0) {
    sprintf(l_string,"Radionuclide:                    %s", radionuclide.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };
    
    if (cine_rate.count != 0) {
    sprintf(l_string,"Cine Rate:                       %s", cine_rate.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (slice_thickness.count != 0) {
    sprintf(l_string,"Slice Thickness:                 %s", slice_thickness.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (repetition_time.count != 0) {
    sprintf(l_string,"Repetition Time:                 %s", repetition_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (echo_time.count != 0) {
    sprintf(l_string,"Echo Time:                       %s", echo_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (inversion_time.count != 0) {
    sprintf(l_string,"Inversion Time:                  %s", inversion_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (number_of_averages.count != 0) {
    sprintf(l_string,"Number of Averages:              %s", number_of_averages.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (imaging_frequency.count != 0) {
    sprintf(l_string,"Imaging Frequency:               %s", imaging_frequency.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (imaged_nucleus.count != 0) {
    sprintf(l_string,"Imaged Nucleus:                  %s", imaged_nucleus.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (echo_number.count != 0) {
    sprintf(l_string,"Echo Number:                     %s", echo_number.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (data_collection_diameter.count != 0) {
    sprintf(l_string,"Data Collection Diameter:        %s", data_collection_diameter.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (device_serial_number.count != 0) {
    sprintf(l_string,"Device Serial Number:            %s", device_serial_number.tword);
    /*sprintf(l_string,"number: %d",device_serial_number.tword);; */
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (software_version.count != 0) {
    sprintf(l_string,"Software Version:                %s", software_version.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (protocol.count != 0) {
    sprintf(l_string,"Protocol:                        %s", protocol.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (date_of_last_calibration.count != 0) {
    sprintf(l_string,"Date of Last Calibration:        %s", date_of_last_calibration.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (time_of_last_calibration.count != 0) {
    sprintf(l_string,"Time of Last Calibration:        %s", time_of_last_calibration.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (data_acquisition_duration.count != 0) {
    sprintf(l_string,"Data Acquisition Duration:       %s", data_acquisition_duration.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (receiving_coil.count != 0) {
    sprintf(l_string,"Receiving Coil:                  %s", receiving_coil.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (transmitting_coil.count != 0) {
    sprintf(l_string,"Transmitting Coil:               %s", transmitting_coil.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (comments_0018.count != 0) {
    sprintf(l_string,"Comments:                        %s", comments_0018.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_position.count != 0) {
    sprintf(l_string,"Patient Position:                %s", patient_position.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

   return 0;

}

int write_group0020()
{

    sprintf(l_string,"Group Length (0020,0000):        %ld", group_length_0020);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (study.count != 0) {
    sprintf(l_string,"Study:                           %s", study.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (series.count != 0) {
    sprintf(l_string,"Series:                          %s", series.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (acquisition.count != 0) {
    sprintf(l_string,"Acquisition:                     %s", acquisition.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (image.count != 0) {
    sprintf(l_string,"Image:                           %s", image.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_orientation.count != 0) {
    sprintf(l_string,"Patient Orientation:             %s", patient_orientation.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (image_position.count != 0) {
    sprintf(l_string,"Image Position:                  %s", image_position.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (image_orientation.count != 0) {
    sprintf(l_string,"Image Orientation:               %s", image_orientation.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (location.count != 0) {
    sprintf(l_string,"Location:                        %s", location.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (laterality.count != 0) {
    sprintf(l_string,"Laterality:                      %s", laterality.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (image_geometry_type.count != 0) {
    sprintf(l_string,"Image Geometry Type:             %s", image_geometry_type.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (masking_image.count != 0) {
    sprintf(l_string,"Masking Image.:                    %s", masking_image.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (series_in_study.count != 0) {
    sprintf(l_string,"Series in Study.:                 %s", series_in_study.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (acquisitions_in_series.count != 0) {
    sprintf(l_string,"Acquisitions in Series :         %s", acquisitions_in_series.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (images_in_acquisition.count != 0) {
    sprintf(l_string,"Images in Acquisitions:          %s", images_in_acquisition.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };
    
#ifndef RESTRICTED
    if (reference.count != 0) {
    sprintf(l_string,"Reference:                       %s", reference.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };
#endif

    if (source_image_ids.count != 0) {
    sprintf(l_string,"Source Image IDs:                %s", source_image_ids.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (modifying_device_id.count != 0) {
    sprintf(l_string,"Modifying Device:                %s", modifying_device_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (modified_image_id.count != 0) {
    sprintf(l_string,"Modified Image ID:               %s", modified_image_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (modified_image_date.count != 0) {
    sprintf(l_string,"Modified Image Data:             %s", modified_image_date.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (modifying_manufacturer.count != 0) {
    sprintf(l_string,"Modified Image Data:             %s", modifying_manufacturer.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (modified_image_time.count != 0) {
    sprintf(l_string,"Modified Image Time:             %s", modified_image_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (modified_image_description.count != 0) {
    sprintf(l_string,"Modified Image Description:      %s", modified_image_description.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (comments_0020.count != 0) {
    sprintf(l_string,"Comments:                        %s", comments_0020.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (original_image_identification.count != 0) {
    sprintf(l_string,"Original Image Identification:   %d  ", ioriginal_image_identification);
    /*Iappend_line((IUB_IMAGE) outimage, l_string)original_image_identification.count, original_image_identification.tword); */  };

    if (original_image_identification_nomenclature.count != 0) {
    sprintf(l_string,"Original Image Id. Nomenclature: %s", original_image_identification_nomenclature.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };


    return 0;

}




int write_group0028()
{

    sprintf(l_string,"Group Length (0028,0000):        %ld", group_length_0028);
    Iappend_line((IUB_IMAGE) outimage, l_string);


    sprintf(l_string,"Image Dimension:                 %3d", image_dimension);
    Iappend_line((IUB_IMAGE) outimage, l_string);
      
    sprintf(l_string,"Rows:                            %3d", rows);
    Iappend_line((IUB_IMAGE) outimage, l_string);
    
    
    sprintf(l_string,"Columns:                         %3d  ", columns);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (pixel_size.count != 0) {
    sprintf(l_string,"Pixel Size:                      %s", pixel_size.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (image_format.count != 0) {
    sprintf(l_string,"Image Format:                    %s", image_format.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (manipulated_image.count != 0) {
    sprintf(l_string,"Manipulated Image.:              %s", manipulated_image.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (compression_code.count != 0) {
    sprintf(l_string,"Compression Code:                %s", compression_code.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    sprintf(l_string,"Bits Allocated:                  %3d", bits_allocated);
    Iappend_line((IUB_IMAGE) outimage, l_string);
    
    sprintf(l_string,"Bits Stored:                     %3d", bits_stored);
    Iappend_line((IUB_IMAGE) outimage, l_string);
    
    sprintf(l_string,"High Bit:                        %3d", high_bit);
    Iappend_line((IUB_IMAGE) outimage, l_string);
    
    sprintf(l_string,"Pixel Representation:            %3d", pixel_representation);
    Iappend_line((IUB_IMAGE) outimage, l_string);
    
    sprintf(l_string,"Smallest Pixel Value:            %3d", smallest_pixel_value);
    Iappend_line((IUB_IMAGE) outimage, l_string);
    
    sprintf(l_string,"Largest Pixel Value:             %3d  ", largest_pixel_value);
    Iappend_line((IUB_IMAGE) outimage, l_string);
    
    sprintf(l_string,"Image Location :                  %4x %d  ", image_location, image_location);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (window_center.count != 0) {
    sprintf(l_string,"Window Center:                   %s", window_center.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (window_width.count != 0) {
    sprintf(l_string,"Window Width:                    %s", window_width.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (rescale_intercept.count != 0) {
    sprintf(l_string,"Rescale Intercept:               %s", rescale_intercept.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (rescale_slope.count != 0) {
    sprintf(l_string,"Rescale Slope:                   %s", rescale_slope.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (comments_0028.count != 0) {
    sprintf(l_string,"Comments:                        %s", comments_0028.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    return 0;

}

