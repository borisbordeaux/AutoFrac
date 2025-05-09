project(Rips_complex_examples)

# Point cloud
add_executable ( Rips_complex_example_from_off example_rips_complex_from_off_file.cpp )

add_executable ( Rips_complex_example_one_skeleton_from_points example_one_skeleton_rips_from_points.cpp )

# Distance matrix
add_executable ( Rips_complex_example_one_skeleton_from_distance_matrix example_one_skeleton_rips_from_distance_matrix.cpp )

add_executable ( Rips_complex_example_from_csv_distance_matrix example_rips_complex_from_csv_distance_matrix_file.cpp )

# Sparse rips from points
add_executable ( Rips_complex_example_sparse example_sparse_rips.cpp )

# Correlation matrix
add_executable (  Rips_complex_example_one_skeleton_rips_from_correlation_matrix example_one_skeleton_rips_from_correlation_matrix.cpp )

if(TARGET TBB::tbb)
  target_link_libraries(Rips_complex_example_from_off TBB::tbb)
  target_link_libraries(Rips_complex_example_one_skeleton_from_points TBB::tbb)
  target_link_libraries(Rips_complex_example_one_skeleton_from_distance_matrix TBB::tbb)
  target_link_libraries(Rips_complex_example_from_csv_distance_matrix TBB::tbb)
  target_link_libraries(Rips_complex_example_sparse TBB::tbb)
  target_link_libraries(Rips_complex_example_one_skeleton_rips_from_correlation_matrix TBB::tbb)
endif()

add_test(NAME Rips_complex_example_one_skeleton_from_points
    COMMAND $<TARGET_FILE:Rips_complex_example_one_skeleton_from_points>)
add_test(NAME Rips_complex_example_one_skeleton_from_distance_matrix
    COMMAND $<TARGET_FILE:Rips_complex_example_one_skeleton_from_distance_matrix>)
add_test(NAME Rips_complex_example_sparse
    COMMAND $<TARGET_FILE:Rips_complex_example_sparse>)
add_test(NAME Rips_complex_example_one_skeleton_rips_from_correlation_matrix
    COMMAND $<TARGET_FILE:Rips_complex_example_one_skeleton_rips_from_correlation_matrix>)

add_test(NAME Rips_complex_example_from_off_doc_12_1 COMMAND $<TARGET_FILE:Rips_complex_example_from_off>
    "${CMAKE_SOURCE_DIR}/data/points/alphacomplexdoc.off" "12.0" "1" "${CMAKE_CURRENT_BINARY_DIR}/ripsoffreader_result_12_1.txt")
add_test(NAME Rips_complex_example_from_off_doc_12_3 COMMAND $<TARGET_FILE:Rips_complex_example_from_off>
    "${CMAKE_SOURCE_DIR}/data/points/alphacomplexdoc.off" "12.0" "3" "${CMAKE_CURRENT_BINARY_DIR}/ripsoffreader_result_12_3.txt")

add_test(NAME Rips_complex_example_from_csv_distance_matrix_doc_12_1 COMMAND $<TARGET_FILE:Rips_complex_example_from_csv_distance_matrix>
    "${CMAKE_SOURCE_DIR}/data/distance_matrix/full_square_distance_matrix.csv" "12.0" "1" "${CMAKE_CURRENT_BINARY_DIR}/ripscsvreader_result_12_1.txt")
add_test(NAME Rips_complex_example_from_csv_distance_matrix_doc_12_3 COMMAND $<TARGET_FILE:Rips_complex_example_from_csv_distance_matrix>
    "${CMAKE_SOURCE_DIR}/data/distance_matrix/full_square_distance_matrix.csv" "12.0" "3" "${CMAKE_CURRENT_BINARY_DIR}/ripscsvreader_result_12_3.txt")


if (DIFF_PATH)
  # Do not forget to copy test results files in current binary dir
  file(COPY "one_skeleton_rips_for_doc.txt" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
  file(COPY "full_skeleton_rips_for_doc.txt" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)

  add_test(Rips_complex_example_from_off_doc_12_1_diff_files ${DIFF_PATH}
      ${CMAKE_CURRENT_BINARY_DIR}/ripsoffreader_result_12_1.txt
      ${CMAKE_CURRENT_BINARY_DIR}/one_skeleton_rips_for_doc.txt)
  set_tests_properties(Rips_complex_example_from_off_doc_12_1_diff_files PROPERTIES DEPENDS Rips_complex_example_from_off_doc_12_1) 

  add_test(Rips_complex_example_from_off_doc_12_3_diff_files ${DIFF_PATH}
      ${CMAKE_CURRENT_BINARY_DIR}/ripsoffreader_result_12_3.txt
      ${CMAKE_CURRENT_BINARY_DIR}/full_skeleton_rips_for_doc.txt)
  set_tests_properties(Rips_complex_example_from_off_doc_12_3_diff_files PROPERTIES DEPENDS Rips_complex_example_from_off_doc_12_3) 

  add_test(Rips_complex_example_from_csv_distance_matrix_doc_12_1_diff_files ${DIFF_PATH}
      ${CMAKE_CURRENT_BINARY_DIR}/ripscsvreader_result_12_1.txt
      ${CMAKE_CURRENT_BINARY_DIR}/one_skeleton_rips_for_doc.txt)
  set_tests_properties(Rips_complex_example_from_csv_distance_matrix_doc_12_1_diff_files PROPERTIES DEPENDS Rips_complex_example_from_csv_distance_matrix_doc_12_1) 

  add_test(Rips_complex_example_from_csv_distance_matrix_doc_12_3_diff_files ${DIFF_PATH}
      ${CMAKE_CURRENT_BINARY_DIR}/ripscsvreader_result_12_3.txt
      ${CMAKE_CURRENT_BINARY_DIR}/full_skeleton_rips_for_doc.txt)
  set_tests_properties(Rips_complex_example_from_csv_distance_matrix_doc_12_3_diff_files PROPERTIES DEPENDS Rips_complex_example_from_csv_distance_matrix_doc_12_3) 

endif()

