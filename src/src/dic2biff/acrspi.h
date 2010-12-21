 extern unsigned char  *buffer1, *buffer2, *buffer3, *buffer4;
 extern FILE *ptr, *ptr1;
 extern long int position_1, position_2;
 extern int max_pixel_value;

 struct tnode {
   char tword[150];
   int32  count;
   };

 /* For Image Text (0051,1010)H */
 struct ttnode {
   char tword[5000];
   int32  count;
   };


  extern IUS_IMAGE         outimage;
  extern char              l_string[256];
  extern time_t            tp;
  extern char              time_stamp[80];
 /*  extern char                     biffile[80];
  extern char                     tiffile[80]; */
  extern char		   title[80]; 
  extern char		   title1[80];

  /* group 0008 variables:                */
  extern int32               group_length_0008;
  extern int32               length_to_end;
  extern struct tnode              recognition_code;
  extern struct tnode              study_date;
  extern struct tnode		     series_date;
  extern struct tnode              acquisition_date;
  extern struct tnode              image_date;
  extern struct tnode              study_time;
  extern struct tnode              series_time;
  extern struct tnode              acquisition_time;
  extern struct tnode              image_time;
  extern int16               data_set_type;
  extern struct tnode              data_set_subtype;
  extern struct tnode              modality;
  extern struct tnode              manufacturer;
  extern struct tnode              institution_id;
  extern struct tnode              referring_physician;
  extern struct tnode	             network_id;
  extern struct tnode              station_id;
  extern struct tnode              procedure_description;
  extern struct tnode              institutional_department;
  extern struct tnode              attending_physician;
  extern struct tnode              radiologist;
  extern struct tnode              operator_identification;
  extern struct tnode              admitting_diagnosis;
  extern struct tnode              manufacturer_model;
  extern struct tnode              comments_0008;


  /* group 0010 variables:                */


  extern int32             group_length_0010;
  extern struct tnode      patient_name;
  extern struct tnode      patient_id;
  extern struct tnode      patient_birthday;

  
  extern struct tnode      patient_sex;
  extern struct tnode	   other_patient_id;
  extern struct tnode	   other_patient_name;
  extern struct tnode      patient_maiden_name;

  extern struct tnode      patient_age;
  extern struct tnode      patient_size;
  extern struct tnode      patient_weight;
  extern struct tnode      patient_address;
  extern struct tnode      insurance_plan_id;
  extern struct tnode      patient_mother_maiden_name;
  extern struct tnode      comments;
 

  /* group 0018 variables:                */

  extern int32                     group_length_0018;
  extern struct tnode              contrast_bolus_agent;
  extern struct tnode              scanning_sequence;
  extern struct tnode              radionuclide;
  extern struct tnode              cine_rate;
  extern struct tnode              slice_thickness;
  extern struct tnode              kvp;
  extern struct tnode              counts_accumulated;
  extern struct tnode              repetition_time;
  extern struct tnode              echo_time;
  extern struct tnode              inversion_time;
  extern struct tnode              number_of_averages;
  extern struct tnode              imaging_frequency;
  extern struct tnode              imaged_nucleus;
  extern struct tnode              echo_number;
  extern struct tnode              data_collection_diameter;
  extern struct tnode              device_serial_number;
  extern struct tnode              film_scanner_id;
  extern struct tnode              software_version;
  extern struct tnode              protocol;
  /* hoppet over endel her ...... */
  extern struct tnode              date_of_last_calibration;
  extern struct tnode              time_of_last_calibration;
  extern struct tnode              data_acquisition_duration;
  extern struct tnode              receiving_coil;
  extern struct tnode              transmitting_coil;
  extern struct tnode              comments_0018;
  extern struct tnode              patient_position;


  /* group 0020 variables:                */

  extern int32                     group_length_0020;
  extern struct tnode              study;
  extern struct tnode              series;
  extern struct tnode              acquisition;
  extern struct tnode              image;
  extern struct tnode              patient_orientation;
  extern struct tnode              image_position;
  extern struct tnode              image_orientation;
  extern struct tnode              location;
  extern struct tnode              laterality;
  extern struct tnode              image_geometry_type;
  extern struct tnode              masking_image;
  extern struct tnode              series_in_study;
  extern struct tnode              acquisitions_in_series;
  extern struct tnode              images_in_acquisition;
  extern struct tnode              reference;
  extern struct tnode              source_image_ids;
  extern struct tnode              modifying_device_id;
 
  extern struct tnode              modifying_device_manufacturer;
  extern struct tnode              modified_image_id;
  extern struct tnode              modified_image_date;
  extern struct tnode              modifying_manufacturer;
  extern struct tnode              modified_image_time;
  extern struct tnode              modified_image_description;
  extern struct tnode              comments_0020;
  extern struct tnode              original_image_identification;
  extern int16                     ioriginal_image_identification;
  extern struct tnode              original_image_identification_nomenclature;


  /* group 0028 variables:                */
  extern int32                     group_length_0028;
  extern int16                     image_dimension;
  extern int16                     rows;
  extern int16                     columns;
  extern struct tnode              pixel_size;
  extern struct tnode              image_format;
  extern struct tnode              manipulated_image;
  extern struct tnode              compression_code;
  extern int16                     bits_allocated;
  extern int16                     bits_stored;
  extern int16                     high_bit;
  extern int16                     pixel_representation;
  extern int16			   smallest_pixel_value;
  extern int16			   largest_pixel_value;
  extern int16                     image_location;
  extern struct tnode              window_center;
  extern struct tnode              window_width;
  extern struct tnode              rescale_intercept;
  extern struct tnode              rescale_slope;
  extern struct tnode              comments_0028;



  /* group 7fe0 variables:  */
  extern int32                     group_length_7fe0;

  /* group 0009 variables:                */
  extern int32                     group_length_0009;
  extern struct tnode              shadow_owner_code_spi_0009;
  extern struct tnode              shadow_owner_code_cms_0009;
  extern struct tnode              shadow_owner_code_label_id_0009;
  extern struct tnode              comments_0009;
  extern struct tnode              unique_identifier;
  extern int16                     data_object_type;
  extern struct tnode              data_object_subtype;

  extern struct tnode              number_of_measurements;
  extern int32                     evaluation_mask_image;

  extern struct tnode              storage_mode;
  extern struct tnode              last_move_date;
  extern struct tnode              last_move_time;

  extern struct tnode              generator_identification_label;
  extern struct tnode              gantry_identification_label;
  extern struct tnode              xray_tube_identification_label;
  extern struct tnode              detector_identification_label;
  extern struct tnode              das_identification_label;
  extern struct tnode              smi_identification_label;
  extern struct tnode              cpu_identification_label;
  extern struct tnode              header_version;


  /* group 0011 variables:                */
  extern int32                  group_length_0011;
  extern struct tnode              shadow_owner_code_spi_0011;
  extern struct tnode              shadow_owner_code_cms_0011;
  extern struct tnode              organ;
  extern struct tnode              registration_date;
  extern struct tnode              registration_time;
  extern struct tnode              used_patient_weight;
  extern struct tnode              organ_code;


  /* group 0019 variables:  */
  extern int32                  group_length_0019;
  extern struct tnode              shadow_owner_code_cms_0019;
  extern struct tnode              shadow_owner_code_siemens_ct_id;
  extern struct tnode              shadow_owner_code_siemens_mr_id;
  extern struct tnode              shadow_owner_code_siemens_ct_adjust_id;
  extern struct tnode              shadow_owner_code_siemens_mr_adjust_id;
  extern struct tnode              shadow_owner_code_siemens_acquisition_id;
  extern struct tnode              net_frequency;
  extern struct tnode              measurement_mode;
  extern struct tnode              calculation_mode;
  extern struct tnode              noise_level;
  extern struct tnode              number_of_data_bytes;
  /* Siemens CT-subgroup skipped for now.... */

  /* Siemens MR-subgroup:  */
  extern struct tnode              total_measurement_time_nominal;
  extern struct tnode              total_measurement_time_current;
  extern struct tnode              start_delay_time;
  extern struct tnode              dwell_time;
  extern struct tnode              number_of_phases;
  extern int32			    sequence_control_mask;
  extern int32			    measurement_status_mask;
  extern struct tnode              number_of_fourier_lines_nominal;
  extern struct tnode              number_of_fourier_lines_current;
  extern struct tnode              number_of_fourier_lines_after_zero;
  extern struct tnode              first_measured_fourier_line;
  extern struct tnode              acquisition_columns;
  extern struct tnode              reconstruction_columns;
  extern struct tnode              number_of_averages_current;
  extern struct tnode              flip_angle;
  extern struct tnode              number_of_prescans;
  extern struct tnode              filter_type_for_raw_data;
  extern struct tnode              filter_parameter_for_raw_data;
  extern struct tnode              filter_type_for_image_data;
  extern struct tnode              filter_parameter_for_image_data;
  extern struct tnode              filter_type_for_phase_correction_data;
  extern struct tnode              filter_parameter_for_phase_correction_data;
  extern struct tnode              number_of_saturation_regions;
  extern struct tnode              image_rotation_angle;
  extern int32                     coil_id_mask;
  extern struct tnode              coil_position;
  extern struct tnode              epi_reconstruction_phase;
  extern struct tnode              epi_reconstruction_slope;
  /* Siemens CT Configuration and Adjust Subgroup skippe for now... */

  /* Siemens MR Configuration and Adjust Subgroup:  */
  extern struct tnode              magnetic_field_strength;
  extern struct tnode              adc_voltage;
  extern struct tnode              adc_offset;
  extern struct tnode              transmitter_amplitude;
  extern struct tnode              number_of_transmitter_amplitudes;
  extern struct tnode              transmitter_attenuator;
  extern struct tnode              transmitter_calibration;
  extern struct tnode              transmitter_reference;
  extern struct tnode              receiver_total_gain;
  extern struct tnode              receiver_amplifier_gain;
  extern struct tnode              receiver_preamplifier_gain;
  extern struct tnode              receiver_cable_attenuation;
  extern struct tnode              receiver_reference_gain;
  extern struct tnode              receiver_filter_frequency;
  extern struct tnode              reconstruction_scale_factor;
  extern struct tnode              reference_scale_factor;
  extern struct tnode              phase_gradient_amplitude;
  extern struct tnode              readout_gradient_amplitude;
  extern struct tnode              selection_gradient_amplitude;
  extern struct tnode              gradient_delay_time;
  extern struct tnode              total_gradient_delay_time;
  extern struct tnode              sensitivity_correction_lable;
  extern int32                     rf_watchdog_mask;
  extern struct tnode              rf_power_error_indicator;
  extern struct tnode              sar_whole_body;
  extern struct tnode              specific_energy_dose;
  extern int32			    adjustment_status_mask;
  extern struct tnode              epi_capacity;
  extern struct tnode              epi_inductance;
  extern struct tnode              epi_switch_configuration_code;
  extern struct tnode              epi_switch_hardware_code;
  extern struct tnode              epi_switch_delay_time;
  /* Siemens Acquisition Subgroup:      */
  extern struct tnode              parameter_file_name;
  extern struct tnode              sequence_file_name;
  extern struct tnode              sequence_file_owner;
  extern struct tnode              sequence_description;
  extern struct tnode              epi_file_name;

  /* group 0021 variables:  */
  extern int32                  group_length_0021;
  extern struct tnode		    shadow_owner_code_siemens_ubmed_0021;
  extern struct tnode              shadow_owner_code_siemens_cms_0021;
  extern struct tnode              shadow_owner_code_siemens_ct_0021;
  extern struct tnode              shadow_owner_code_siemens_mr_0021;
  extern struct tnode              shadow_owner_code_siemens_ct_raw_0021;
  extern struct tnode              shadow_owner_code_siemens_mr_raw_0021;
  extern struct tnode              shadow_owner_code_siemens_mr_spect_0021;
  extern struct tnode              zoom;
  extern struct tnode              target;
  extern int16                     roi_mask;
  /* Siemens CMS Subgroup */
  extern struct tnode              field_of_view;
  extern struct tnode              image_magnification_factor;
  extern struct tnode              view_direction;
  extern struct tnode              patient_rest_direction;
  extern struct tnode              image_position;
  extern struct tnode              image_normal;
  extern struct tnode              image_distance;
  extern int16                     image_positioning_history_mask;
  extern struct tnode              image_row;
  extern struct tnode              image_column;
  extern struct tnode              patient_orientation_set_1;
  extern struct tnode              patient_orientation_set_2;
  extern struct tnode              study_name;
  extern struct tnode              study_type;
  /* Siemens CT Id Subgroup Skipped for now.... */
  /* Siemens MR Id Subgroup:  */
  extern struct tnode              phase_correction_rows_sequence;
  extern struct tnode              phase_correction_columns_sequence;
  extern struct tnode              phase_correction_rows_reconstruction;
  extern struct tnode              phase_correction_columns_reconstruction;
  extern struct tnode              number_of_3d_raw_partitions_nominal;
  extern struct tnode              number_of_3d_raw_partitions_current;
  extern struct tnode              number_of_3d_image_partitions;
  extern struct tnode              actual_3d_image_partition_number;
  extern struct tnode              slab_thickness;
  extern struct tnode              number_of_slices_nominal;
  extern struct tnode              number_of_slices_current;
  extern struct tnode              current_slice_number;
  extern struct tnode              current_group_number;
  extern struct tnode              current_slice_distance_factor;
  extern struct tnode              mip_start_row;
  extern struct tnode              mip_stop_row;
  extern struct tnode              mip_start_column;
  extern struct tnode              mip_stop_column;
  extern struct tnode              mip_start_slice;
  extern struct tnode              mip_stop_slice;
  extern struct tnode              order_of_slices;
  extern int32                            signal_mask;
  extern struct tnode              delay_time_after_trigger;
  extern struct tnode              rr_interval;
  extern struct tnode              number_of_trigger_pulse;
  extern struct tnode              repetition_time_effective;
  extern struct tnode              gate_phase;
  extern struct tnode              gate_threshold;
  extern struct tnode              gated_ratio;
  extern struct tnode              number_of_interpolated_images;
  extern struct tnode              number_of_echoes;
  extern struct tnode              second_echo_time;
  extern struct tnode              second_repetition_time;
  extern struct tnode              cardiac_code;
  /* Siemens CT RAW Data Subgroup skipped for now .....*/
  /* Siemens MR RAW Data Subgroup does not seem to exist .....*/



  /* group 0029 variables:  */
  extern int32                  group_length_0029;
  extern struct tnode		   shadow_owner_code_siemens_spi_0029;
  extern struct tnode              shadow_owner_code_siemens_cms_0029;
  extern struct tnode              window_style;
  extern struct tnode              pixel_quality_code;
  extern struct tnode              pixel_quality_value;
  extern struct tnode              archive_code;
  extern struct tnode              exposure_code;
  extern struct tnode              sort_code;
  extern struct tnode              splash;

  /* group 0051 variables:  */
  extern int32                  group_length_0051;
  extern struct tnode              shadow_owner_code_siemens_cms_0051;
  extern struct ttnode             image_text;






