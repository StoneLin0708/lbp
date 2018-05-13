
add_executable(dataset test_dataset.cc)
target_link_libraries(dataset ${OpenCV_LIBS} ${Boost_LIBRARIES} fmt::fmt-header-only lbp_svm)

add_executable(lbp test_lbp.cc)
target_link_libraries(lbp ${OpenCV_LIBS} fmt::fmt-header-only lbp_svm)

add_executable(train test_train.cc)
target_link_libraries(train ${OpenCV_LIBS} ${Boost_LIBRARIES} fmt::fmt-header-only lbp_svm)

add_executable(train_two_part test_train_two_part.cc)
target_link_libraries(train_two_part ${OpenCV_LIBS} ${Boost_LIBRARIES} fmt::fmt-header-only lbp_svm)

add_executable(detect_two_part detect_two_part.cc)
target_link_libraries(detect_two_part ${OpenCV_LIBS} ${Boost_LIBRARIES} fmt::fmt-header-only lbp_svm)

add_executable(two_part_hist_v test_two_part_hist_v.cc)
target_link_libraries(two_part_hist_v ${OpenCV_LIBS} ${Boost_LIBRARIES} fmt::fmt-header-only lbp_svm)

