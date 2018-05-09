
add_executable(dataset test_dataset.cc)
target_link_libraries(dataset ${OpenCV_LIBS} ${Boost_LIBRARIES} fmt::fmt-header-only lbp_svm)

add_executable(lbp test_lbp.cc)
target_link_libraries(lbp ${OpenCV_LIBS} fmt::fmt-header-only lbp_svm)

add_executable(train test_train.cc)
target_link_libraries(train ${OpenCV_LIBS} ${Boost_LIBRARIES} fmt::fmt-header-only lbp_svm)

