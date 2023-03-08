/*C*

________________________________________________________________

	module: wspibiff.c
	Prints out all Siemens Shadow-groups to biff-format.
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


/*(Non-global) External Variables*/


int write_group0009()
{

    sprintf(l_string,"Group Length (0009,0000):         %ld", group_length_0009);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (shadow_owner_code_spi_0009.count != 0) {
    sprintf(l_string,"Shadow Owner (SPI):              %s", shadow_owner_code_spi_0009.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (shadow_owner_code_cms_0009.count != 0) {
    sprintf(l_string,"Shadow Owner Code (CMS):         %s", shadow_owner_code_cms_0009.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (shadow_owner_code_label_id_0009.count != 0) {
    sprintf(l_string,"Shadow Owner Code Label ID:      %s", shadow_owner_code_label_id_0009.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (comments_0009.count != 0) {
    sprintf(l_string,"Comments:                        %s", comments_0009.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (unique_identifier.count != 0) {
    sprintf(l_string,"Unique Identifier:               %s", unique_identifier.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    sprintf(l_string,"Data Object Type:                %3d", data_object_type);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (data_object_subtype.count != 0) {
    sprintf(l_string,"Data Object Subtype:             %s", data_object_subtype.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (number_of_measurements.count != 0) {
    sprintf(l_string,"Number of Measurements:          %s", number_of_measurements.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (storage_mode.count != 0) {
    sprintf(l_string,"Storage Mode:                    %s", storage_mode.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    sprintf(l_string,"Evaluation Mask Image.:         %3d", evaluation_mask_image);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (last_move_date.count != 0) {
    sprintf(l_string,"Last Move Date:                  %s", last_move_date.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (last_move_time.count != 0) {
    sprintf(l_string,"Last Move Time:                  %s", last_move_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (generator_identification_label.count != 0) {
    sprintf(l_string,"Generator Identification Label:  %s", generator_identification_label.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (gantry_identification_label.count != 0) {
    sprintf(l_string,"Gantry Identification Label:     %s", gantry_identification_label.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (xray_tube_identification_label.count != 0) {
    sprintf(l_string,"Xray Tube ID Lable:              %s", xray_tube_identification_label.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (detector_identification_label.count != 0) {
    sprintf(l_string,"Detector:                        %s", detector_identification_label.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (das_identification_label.count != 0) {
    sprintf(l_string,"DAS Identification Label:        %s", das_identification_label.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (smi_identification_label.count != 0) {
    sprintf(l_string,"SMI Identification Label:        %s", smi_identification_label.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (cpu_identification_label.count != 0) {
    sprintf(l_string,"CPU Identification Label:        %s", cpu_identification_label.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (header_version.count != 0) {
    sprintf(l_string,"Header Version:                  %s", header_version.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    return 0;
}


int write_group0011()
{

    sprintf(l_string,"Group Length (0011,0000):         %ld", group_length_0011);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (shadow_owner_code_spi_0011.count != 0) {
    sprintf(l_string,"Shadow Owner (SPI):              %s", shadow_owner_code_spi_0011.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (shadow_owner_code_cms_0011.count != 0) {
    sprintf(l_string,"Shadow Owner Code (CMS):         %s", shadow_owner_code_cms_0011.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);};

    if (organ.count != 0) {
    sprintf(l_string,"Organ:                           %s", organ.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (registration_date.count != 0) {
    sprintf(l_string,"Registration Date:               %s", registration_date.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (registration_time.count != 0) {
    sprintf(l_string,"Registration Time:               %s", registration_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (used_patient_weight.count != 0) {
    sprintf(l_string,"Used Patient Weight:             %s", used_patient_weight.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (organ_code.count != 0) {
    sprintf(l_string,"Organ Code:                      %s", organ_code.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    return 0;
}


int write_group0019()
{

    sprintf(l_string,"Group Length (0019,0000):         %ld", group_length_0019);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (shadow_owner_code_cms_0019.count != 0) {
    sprintf(l_string,"Shadow Owner Code (CMS):         %s", shadow_owner_code_cms_0019.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (shadow_owner_code_siemens_ct_id.count != 0) {
    sprintf(l_string,"Shadow Owner Code Siemens CT-id: %s", shadow_owner_code_siemens_ct_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (shadow_owner_code_siemens_mr_id.count != 0) {
    sprintf(l_string,"Shadow Owner Code Siemens MR-id: %s", shadow_owner_code_siemens_mr_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (shadow_owner_code_siemens_ct_adjust_id.count != 0) {
    sprintf(l_string,"Shadow Owner Code Siemens CT-adj:%s", shadow_owner_code_siemens_ct_adjust_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (shadow_owner_code_siemens_mr_adjust_id.count != 0) {
    sprintf(l_string,"Shadow Owner Code Siemens MR-adj:%s", shadow_owner_code_siemens_mr_adjust_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (shadow_owner_code_siemens_acquisition_id.count != 0) {
    sprintf(l_string,"Shadow Owner Code Siemens Acq-id:%s", shadow_owner_code_siemens_acquisition_id.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (net_frequency.count != 0) {
    sprintf(l_string,"Net Frequency:                   %s", net_frequency.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (measurement_mode.count != 0) {
    sprintf(l_string,"Measurement Mode:                %s", measurement_mode.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (calculation_mode.count != 0) {
    sprintf(l_string,"Calculation Mode:                %s", calculation_mode.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (noise_level.count != 0) {
    sprintf(l_string,"Noise Level:                     %s", noise_level.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (number_of_data_bytes.count != 0) {
    sprintf(l_string,"Number of Data Bytes:            %s", number_of_data_bytes.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (total_measurement_time_nominal.count != 0) {
    sprintf(l_string,"Total Measurement Time Nominal:  %s", total_measurement_time_nominal.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (total_measurement_time_current.count != 0) {
    sprintf(l_string,"Total Measurement Time Current:  %s", total_measurement_time_current.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (start_delay_time.count != 0) {
    sprintf(l_string,"Start Delay Time:                %s", start_delay_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (dwell_time.count != 0) {
    sprintf(l_string,"Dwell Time:                      %s", dwell_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (number_of_phases.count != 0) {
    sprintf(l_string,"Number of Phases:                %s", number_of_phases.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    sprintf(l_string,"Sequence Control Mask:          %3d", sequence_control_mask);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    sprintf(l_string,"Measurement Status Mask:        %3d", measurement_status_mask);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (number_of_fourier_lines_nominal.count != 0) {
    sprintf(l_string,"Number of Fourier Lines (Nominal):%s", number_of_fourier_lines_nominal.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (number_of_fourier_lines_current.count != 0) {
    sprintf(l_string,"Number of Fourier Lines (Current):%s", number_of_fourier_lines_current.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (number_of_fourier_lines_after_zero.count != 0) {
    sprintf(l_string,"No. of Fourier Lines after zero: %s", number_of_fourier_lines_after_zero.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (first_measured_fourier_line.count != 0) {
    sprintf(l_string,"First Measured Fourier Line:     %s", first_measured_fourier_line.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (acquisition_columns.count != 0) {
    sprintf(l_string,"Acquisition Columns:             %s", acquisition_columns.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (reconstruction_columns.count != 0) {
    sprintf(l_string,"Reconstruction Columns:          %s", reconstruction_columns.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (number_of_averages_current.count != 0) {
    sprintf(l_string,"Number of Averages (Current):    %s", number_of_averages_current.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (flip_angle.count != 0) {
    sprintf(l_string,"Flip Angle:                      %s", flip_angle.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (number_of_prescans.count != 0) {
    sprintf(l_string,"Number of Prescans:              %s", number_of_prescans.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (filter_type_for_raw_data.count != 0) {
    sprintf(l_string,"Filter Type for RAW-data:        %s", filter_type_for_raw_data.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (filter_parameter_for_raw_data.count != 0) {
    sprintf(l_string,"Filter Parameter for RAW-data:   %s", filter_parameter_for_raw_data.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (filter_type_for_image_data.count != 0) {
    sprintf(l_string,"Filter Type for IMAGE-data:      %s", filter_type_for_image_data.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (filter_parameter_for_image_data.count != 0) {
    sprintf(l_string,"Filter Parameter for IMAGE-data: %s", filter_parameter_for_image_data.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (filter_type_for_phase_correction_data.count != 0) {
    sprintf(l_string,"Fi-Type for Phase-corr.- data:   %s", filter_type_for_phase_correction_data.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (filter_parameter_for_phase_correction_data.count != 0) {
    sprintf(l_string,"Fi-param. for Phase-corr. data:  %s", filter_parameter_for_phase_correction_data.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (number_of_saturation_regions.count != 0) {
    sprintf(l_string,"Number of Saturation Regions:    %s", number_of_saturation_regions.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (image_rotation_angle.count != 0) {
    sprintf(l_string,"Image Rotation Angle:            %s", image_rotation_angle.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    sprintf(l_string,"Coil ID Mask:          %3d", coil_id_mask);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (coil_position.count != 0) {
    sprintf(l_string,"Coil Position:                   %s", coil_position.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (epi_reconstruction_phase.count != 0) {
    sprintf(l_string,"EPI Reconstruction Phase:        %s", epi_reconstruction_phase.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (epi_reconstruction_slope.count != 0) {
    sprintf(l_string,"EPI Reconstruction Slope:        %s", epi_reconstruction_slope.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (magnetic_field_strength.count != 0) {
    sprintf(l_string,"Magnetic Field Strength:         %s", magnetic_field_strength.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (adc_voltage.count != 0) {
    sprintf(l_string,"ADC Voltage:                     %s", adc_voltage.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (adc_offset.count != 0) {
    sprintf(l_string,"ADC Offset:                      %s", adc_offset.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (transmitter_amplitude.count != 0) {
    sprintf(l_string,"Transmitter Amplitude:           %s", transmitter_amplitude.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (number_of_transmitter_amplitudes .count != 0) {
    sprintf(l_string,"Number of Transmitter Amplitudes:%s", number_of_transmitter_amplitudes .tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (transmitter_attenuator.count != 0) {
    sprintf(l_string,"Transmitter Attenuator:          %s", transmitter_attenuator.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (transmitter_calibration.count != 0) {
    sprintf(l_string,"Transmitter Calibration:         %s", transmitter_calibration.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (transmitter_reference.count != 0) {
    sprintf(l_string,"Transmitter Reference:           %s", transmitter_reference.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (receiver_total_gain.count != 0) {
    sprintf(l_string,"Receiver Total Gain:             %s", receiver_total_gain.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (receiver_amplifier_gain.count != 0) {
    sprintf(l_string,"Receiver Amplifier Gain:         %s", receiver_amplifier_gain.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);};

    if (receiver_preamplifier_gain.count != 0) {
    sprintf(l_string,"Receiver Preamplifier Gain:      %s", receiver_preamplifier_gain.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (receiver_cable_attenuation.count != 0) {
    sprintf(l_string,"Receiver Cable Attenuation:      %s", receiver_cable_attenuation.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (receiver_reference_gain.count != 0) {
    sprintf(l_string,"Receiver Reference Gain:         %s", receiver_reference_gain.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (receiver_filter_frequency.count != 0) {
    sprintf(l_string,"Receiver Filter Frequency:       %s", receiver_filter_frequency.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (reconstruction_scale_factor.count != 0) {
    sprintf(l_string,"Reconstruction Scale Factor:     %s", reconstruction_scale_factor.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (reference_scale_factor.count != 0) {
    sprintf(l_string,"Reference Scale Factor:          %s", reference_scale_factor.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (phase_gradient_amplitude.count != 0) {
    sprintf(l_string,"Phase Gradient Amplitude:        %s", phase_gradient_amplitude.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (readout_gradient_amplitude.count != 0) {
    sprintf(l_string,"Readout Gradient Amplitude:      %s", readout_gradient_amplitude.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (selection_gradient_amplitude.count != 0) {
    sprintf(l_string,"Selection Gradient Amplitude:    %s", selection_gradient_amplitude.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (gradient_delay_time.count != 0) {
    sprintf(l_string,"Gradient Delay Time:             %s", gradient_delay_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (total_gradient_delay_time.count != 0) {
    sprintf(l_string,"Total Gradient Delay Time:       %s", total_gradient_delay_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (sensitivity_correction_lable.count != 0) {
    sprintf(l_string,"Sensitivity Correction Lable:    %s", sensitivity_correction_lable.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    sprintf(l_string,"RF Watchdog Mask:           %3d", rf_watchdog_mask);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (rf_power_error_indicator.count != 0) {
    sprintf(l_string,"RF Power Error Indicator:        %s", rf_power_error_indicator.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (sar_whole_body.count != 0) {
    sprintf(l_string,"SAR Whole Body:                  %s", sar_whole_body.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (specific_energy_dose.count != 0) {
    sprintf(l_string,"Specific Energy Dose:            %s", specific_energy_dose.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    sprintf(l_string,"Adjustment Status Mask:      %3d", adjustment_status_mask);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (epi_capacity.count != 0) {
    sprintf(l_string,"EPI Capacity:                    %s", epi_capacity.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (epi_inductance.count != 0) {
    sprintf(l_string,"EPI Inductance:                  %s", epi_inductance.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (epi_switch_configuration_code.count != 0) {
    sprintf(l_string,"EPI Switch Configuration Code:   %s", epi_switch_configuration_code.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (epi_switch_hardware_code.count != 0) {
    sprintf(l_string,"EPI Switch Hardware Code:        %s", epi_switch_hardware_code.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (epi_switch_delay_time.count != 0) {
    sprintf(l_string,"EPI Switch Delay Time:           %s", epi_switch_delay_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (parameter_file_name.count != 0) {
    sprintf(l_string,"Parameter File Name:             %s", parameter_file_name.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (sequence_file_name.count != 0) {
    sprintf(l_string,"Sequence File Name:              %s", sequence_file_name.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (sequence_file_owner.count != 0) {
    sprintf(l_string,"Sequence File Owner:             %s", sequence_file_owner.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (sequence_description.count != 0) {
    sprintf(l_string,"Sequence Description:            %s", sequence_description.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (epi_file_name.count != 0) {
    sprintf(l_string,"EPI File Name:                   %s", epi_file_name.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    return 0;
}





int write_group0021()
{

    sprintf(l_string,"Group Length (0021,0000):         %ld", group_length_0021);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (shadow_owner_code_siemens_ubmed_0021.count != 0) {
    sprintf(l_string,"Shadow Owner (SPI):              %s", shadow_owner_code_siemens_ubmed_0021.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (shadow_owner_code_siemens_cms_0021.count != 0) {
    sprintf(l_string,"Shadow Owner Code (CMS):         %s", shadow_owner_code_siemens_cms_0021.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (shadow_owner_code_siemens_ct_0021.count != 0) {
    sprintf(l_string,"Shadow Owner Code (CT):          %s", shadow_owner_code_siemens_ct_0021.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (shadow_owner_code_siemens_mr_0021.count != 0) {
    sprintf(l_string,"Shadow Owner Code (MR):          %s", shadow_owner_code_siemens_mr_0021.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (shadow_owner_code_siemens_ct_raw_0021.count != 0) {
    sprintf(l_string,"Shadow Owner Code (CT RAW):      %s", shadow_owner_code_siemens_ct_raw_0021.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (shadow_owner_code_siemens_mr_spect_0021.count != 0) {
    sprintf(l_string,"Shadow Owner Code (MR SPECT):    %s", shadow_owner_code_siemens_mr_spect_0021.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (zoom.count != 0) {
    sprintf(l_string,"Zoom:                            %s", zoom.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (target.count != 0) {
    sprintf(l_string,"Target:                          %s", target.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    sprintf(l_string,"ROI Mask:             %d", roi_mask);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (field_of_view.count != 0) {
    sprintf(l_string,"Field of View:                   %s", field_of_view.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (image_magnification_factor.count != 0) {
    sprintf(l_string,"Image Magnification Factor:                          %s", image_magnification_factor.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (view_direction.count != 0) {
    sprintf(l_string,"View Direction:                  %s", view_direction.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_rest_direction.count != 0) {
    sprintf(l_string,"Patient Rest Direction:          %s", patient_rest_direction.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (image_position.count != 0) {
    sprintf(l_string,"Image Position:                  %s", image_position.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (image_normal.count != 0) {
    sprintf(l_string,"Image Normal:                    %s", image_normal.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (image_distance.count != 0) {
    sprintf(l_string,"Image Distance:                  %s", image_distance.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    sprintf(l_string,"Image Positioning History Mask:    %d", image_positioning_history_mask);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (image_row.count != 0) {
    sprintf(l_string,"Image Row:                       %s", image_row.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (image_column.count != 0) {
    sprintf(l_string,"Image Column:                    %s", image_column.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_orientation_set_1.count != 0) {
    sprintf(l_string,"Patient Orientation Set 1:       %s", patient_orientation_set_1.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (patient_orientation_set_2.count != 0) {
    sprintf(l_string,"Patient Orientation Set 2:       %s", patient_orientation_set_2.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (study_name.count != 0) {
    sprintf(l_string,"Study Name:                      %s", study_name.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (study_type.count != 0) {
    sprintf(l_string,"Study Type:                      %s", study_type.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (phase_correction_rows_sequence.count != 0) {
    sprintf(l_string,"Phase Correction Rows sequence:  %s", phase_correction_rows_sequence.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (phase_correction_columns_sequence.count != 0) {
    sprintf(l_string,"Phase Correction Columns sequence:%s", phase_correction_columns_sequence.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (phase_correction_rows_reconstruction.count != 0) {
    sprintf(l_string,"Phase Corrections rows - rec.:   %s", phase_correction_rows_reconstruction.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (phase_correction_columns_reconstruction.count != 0) {
    sprintf(l_string,"Phase Corrections Columns - rec.:%s", phase_correction_columns_reconstruction.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (number_of_3d_raw_partitions_nominal.count != 0) {
    sprintf(l_string,"Number of 3D Raw Partitions -nom.:%s", number_of_3d_raw_partitions_nominal.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (number_of_3d_raw_partitions_current.count != 0) {
    sprintf(l_string,"Number of 3D Raw Partitions -cur.:%s", number_of_3d_raw_partitions_current.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (number_of_3d_image_partitions.count != 0) {
    sprintf(l_string,"Number of 3D Image Partitions :  %s", number_of_3d_image_partitions.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (actual_3d_image_partition_number.count != 0) {
    sprintf(l_string,"Actual 3D Image Partition Number:%s", actual_3d_image_partition_number.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (slab_thickness.count != 0) {
    sprintf(l_string,"Slab Thickness:                  %s", slab_thickness.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (number_of_slices_nominal.count != 0) {
    sprintf(l_string,"Number of Slices - nominal:      %s", number_of_slices_nominal.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (number_of_slices_current.count != 0) {
    sprintf(l_string,"Number of Slices - current:      %s", number_of_slices_current.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (current_slice_number.count != 0) {
    sprintf(l_string,"Current Slice Number:            %s", current_slice_number.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (current_group_number.count != 0) {
    sprintf(l_string,"Current Group Number:            %s", current_group_number.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (current_slice_distance_factor.count != 0) {
    sprintf(l_string,"Current Slice Distance Factor:   %s", current_slice_distance_factor.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (mip_start_row.count != 0) {
    sprintf(l_string,"MIP Start Row:                   %s", mip_start_row.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (mip_stop_row.count != 0) {
    sprintf(l_string,"MIP Stop Row:                    %s", mip_stop_row.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (mip_start_column.count != 0) {
    sprintf(l_string,"MIP Start Column:                %s", mip_start_column.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (mip_stop_column.count != 0) {
    sprintf(l_string,"MIP Stop Column:                 %s", mip_stop_column.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (mip_start_slice.count != 0) {
    sprintf(l_string,"MIP Start Slice:                 %s", mip_start_slice.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (mip_stop_slice.count != 0) {
    sprintf(l_string,"MIP Stop Slice:                  %s", mip_stop_slice.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (order_of_slices.count != 0) {
    sprintf(l_string,"Order of Slices:                 %s", order_of_slices.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    sprintf(l_string,"Signal Mask:    %ld", signal_mask);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (delay_time_after_trigger.count != 0) {
    sprintf(l_string,"Delay Time After Trigger:        %s", delay_time_after_trigger.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (rr_interval.count != 0) {
    sprintf(l_string,"RR Interval:                     %s", rr_interval.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (number_of_trigger_pulse.count != 0) {
    sprintf(l_string,"Number of Trigger Pulses:        %s", number_of_trigger_pulse.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (repetition_time_effective.count != 0) {
    sprintf(l_string,"Repetition Time Effective:       %s", repetition_time_effective.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (gate_phase.count != 0) {
    sprintf(l_string,"Gate Phase:                      %s", gate_phase.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (gate_threshold.count != 0) {
    sprintf(l_string,"Gate Threshold:                  %s", gate_threshold.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);};

    if (gated_ratio.count != 0) {
    sprintf(l_string,"Gated Ratio:                     %s", gated_ratio.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (number_of_interpolated_images.count != 0) {
    sprintf(l_string,"Number of Interpolated Images:   %s", number_of_interpolated_images.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (number_of_echoes.count != 0) {
    sprintf(l_string,"Number of Echoes:                %s", number_of_echoes.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (second_echo_time.count != 0) {
    sprintf(l_string,"Second Echo Time:                %s", second_echo_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (second_repetition_time.count != 0) {
    sprintf(l_string,"Second Repetition Time:          %s", second_repetition_time.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (cardiac_code.count != 0) {
    sprintf(l_string,"Cardiac Code:                    %s", cardiac_code.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    return 0;
}


int write_group0051()
{

    sprintf(l_string,"Group Length (0051,0000):         %ld", group_length_0051);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (shadow_owner_code_siemens_cms_0051.count != 0) {
    sprintf(l_string,"Shadow Owner Code (CMS):         %s", shadow_owner_code_siemens_cms_0051.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (image_text.count != 0) {
    sprintf(l_string,"Image Text:                      %s", image_text.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };


    return 0;
}
int write_group0029()
{

    sprintf(l_string,"Group Length (0029,0000):         %ld", group_length_0029);
    Iappend_line((IUB_IMAGE) outimage, l_string);

    if (shadow_owner_code_siemens_spi_0029.count != 0) {
    sprintf(l_string,"Shadow Owner (SPI):              %s", shadow_owner_code_siemens_spi_0029.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (shadow_owner_code_siemens_cms_0029.count != 0) {
    sprintf(l_string,"Shadow Owner Code (CMS):         %s", shadow_owner_code_siemens_cms_0029.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (window_style.count != 0) {
    sprintf(l_string,"Window Style:                    %s", window_style.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (pixel_quality_code.count != 0) {
    sprintf(l_string,"Pixel Quality Code:              %s", pixel_quality_code.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (pixel_quality_value.count != 0) {
    sprintf(l_string,"Pixel Quality Value:             %s", pixel_quality_value.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (archive_code.count != 0) {
    sprintf(l_string,"Archive Code:                    %s", archive_code.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (exposure_code.count != 0) {
    sprintf(l_string,"Exposure Code:                   %s", exposure_code.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string);  };

    if (sort_code.count != 0) {
    sprintf(l_string,"Sort Code:                       %s", sort_code.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };

    if (splash.count != 0) {
    sprintf(l_string,"Splash:                          %s", splash.tword);
    Iappend_line((IUB_IMAGE) outimage, l_string); };


    return 0;
}
