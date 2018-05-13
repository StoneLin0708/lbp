set(HEADER dataset.h feature.h detect.h)
set(SOURCE dataset.cc)

add_library(lbp_svm ${SOURCE} ${HEADER})
target_link_libraries(lbp_svm PRIVATE ${OpenCV_LIBS} ${Boost_LIBRARIES} fmt::fmt-header-only)

add_executable(detect detect.cc ${HEADER} )
target_link_libraries(detect lbp_svm ${OpenCV_LIBS} fmt::fmt-header-only)

