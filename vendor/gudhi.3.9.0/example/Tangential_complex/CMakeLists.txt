project(Tangential_complex_examples)

if(NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)
  add_executable( Tangential_complex_example_basic example_basic.cpp )
  target_link_libraries(Tangential_complex_example_basic ${CGAL_LIBRARY})
  add_executable( Tangential_complex_example_with_perturb example_with_perturb.cpp )
  target_link_libraries(Tangential_complex_example_with_perturb ${CGAL_LIBRARY})
  if(TARGET TBB::tbb)
    target_link_libraries(Tangential_complex_example_basic TBB::tbb)
    target_link_libraries(Tangential_complex_example_with_perturb TBB::tbb)
  endif()

  add_test(NAME Tangential_complex_example_basic
      COMMAND $<TARGET_FILE:Tangential_complex_example_basic>)
  add_test(NAME Tangential_complex_example_with_perturb
      COMMAND $<TARGET_FILE:Tangential_complex_example_with_perturb>)

endif(NOT CGAL_WITH_EIGEN3_VERSION VERSION_LESS 4.11.0)
